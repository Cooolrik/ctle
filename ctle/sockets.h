// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include <functional>

#include "status.h"

namespace ctle
{

enum class socket_protocol_family
{
	unspecified,
	ipv4,
	ipv6,
};

// base class for sockets. not used directly, implements generic functionality shared by derived classes
class socket
{
public:
	~socket();

protected:
	socket();
	socket(int&&);
	
	int socket_file_desc = -1;
	void close_socket();
	void move_socket_from(socket&&);	
};

class stream_socket : public socket
{
public:
	stream_socket(int&&);
	stream_socket(stream_socket&&);
	stream_socket& operator=(stream_socket&&);
	~stream_socket();

	// send a message on a socket
	// buf - data buffer to copy from
	// buflen - number of bytes to send 
	// sent - actual number of bytes sent
	status send(const void* buf, size_t buflen, size_t& sent);

	// receive a message on a socket
	// buf - data buffer to copy to
	// buflen - number of bytes to receive 
	// received - actual number of bytes received
	status recv(void* buf, size_t buflen, size_t& received);
	
protected:
	stream_socket();
};

class server_socket : public socket
{
public:
	using serve_func = const std::function<status(stream_socket&)>;
	
	server_socket();
	~server_socket();

	// opens a socket and runs a blocking listen() + accept() loop. 
	// to stop the server, call the stop() function from another thread.
	// to run the server async, wrap the call in an std::async() call.
	// the port is the port or port type (e.g. "http") to listen to
	// serve_function is called for each accepted connection, and is responsible to handle the connection (e.g. spawn a thread to handle the incoming connection)
	// protocol_family is either ip4 or ip6
	// backlog_size is the number of incoming connections to keep in queue when handling the current connection
	status start(uint16_t port, const serve_func& serve_function, socket_protocol_family protocol_family = socket_protocol_family::ipv4, size_t backlog_size = 10);
	status start(const std::string &port, const serve_func& serve_function, socket_protocol_family protocol_family = socket_protocol_family::ipv4, size_t backlog_size = 10);

	// stops the server. this needs to be called from another thread than run(), which will block until the server is signaled to stop
	status stop();

private:
	struct internal_data;
	internal_data* data = nullptr;

	status run_internal(const std::string &port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size);
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <string>
#include <atomic>
#include <utility>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "log.h"
#include "_macros.inl"

namespace ctle
{

inline int protocol_family_to_AF(socket_protocol_family protocol_family)
{
	if (protocol_family == socket_protocol_family::ipv4)
		return AF_INET;
	else if (protocol_family == socket_protocol_family::ipv6)
		return AF_INET6;
	else
		return AF_UNSPEC;
}

inline void *get_inet_addr_pointer(sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((sockaddr_in*)sa)->sin_addr);
	else
		return &(((sockaddr_in6*)sa)->sin6_addr);
}

inline void safe_close_socket( int &socket_fd )
{
	if (socket_fd != -1)
	{
		::close(socket_fd);
		socket_fd = -1;
	}
}

socket::socket()
{
}

socket::socket(int &&socket_fd)
	: socket_file_desc(std::move(socket_fd))
{
}

socket::~socket()
{
	this->close_socket();
}

inline void socket::close_socket()
{
	safe_close_socket(this->socket_file_desc);
}

inline void socket::move_socket_from(socket&& other)
{
	// if we have a socket open, close it
	this->close_socket();

	// move the socket descriptor
	this->socket_file_desc = other.socket_file_desc;
	other.socket_file_desc = -1;
}

stream_socket::stream_socket()
	: socket()
{
}

stream_socket::stream_socket(int &&socket_fd)
	: socket(std::move(socket_fd))
{
}

stream_socket::stream_socket(stream_socket &&other)
	: socket()
{
	this->move_socket_from(std::move(other));
}

stream_socket& stream_socket::operator=(stream_socket &&other)
{
	this->move_socket_from(std::move(other));
	return *this;
}

stream_socket::~stream_socket()
{
}

status stream_socket::send(const void* buf, size_t buflen, size_t& sent)
{
	ctValidate(this->socket_file_desc != -1, status::not_initialized) << "The socked has not been initialized, or has been closed after being initialized." << ctValidateEnd;

	ssize_t result = ::send(this->socket_file_desc, buf, buflen, 0);
	if( result < 0 )
	{
		return status::cant_write;
	}
	sent = result;

	return status::ok;
}

status stream_socket::recv(void* buf, size_t buflen, size_t& received)
{
	ctValidate(this->socket_file_desc != -1, status::not_initialized) << "The socked has not been initialized, or has been closed after being initialized." << ctValidateEnd;

	ssize_t result = ::recv(this->socket_file_desc, buf, buflen, 0);
	if( result < 0 )
	{
		return status::cant_write;
	}
	received = result;
	
	return status::ok;
}

struct server_socket::internal_data
{
	enum class state
	{
		stopped,
		started,
		running,
		stopping
	};
	std::atomic<state> server_state = state::stopped;
};

server_socket::server_socket()
	: socket()
{
	this->data = new internal_data();
}

server_socket::~server_socket()
{
	delete this->data;
}


status server_socket::run_internal(const std::string& port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	ctLogInfo << "server_socket::start(): running server, setting up listen socket" << ctLogEnd;

	int result = {};
	addrinfo hints = {};
	addrinfo* servinfo = {};

	// set up the servinfo structure, use getaddrinfo
	hints.ai_family = protocol_family_to_AF(protocol_family);
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	result = getaddrinfo(nullptr, port.c_str(), &hints, &servinfo);
	ctValidate(result == 0, status::not_found) << "Could not find the specified protocol and set up a local address on port " << port << ctValidateEnd;

	// find a socket type to bind to, use first successful
	bool found_and_bound_socket = false;
	for (addrinfo* p = servinfo; p != nullptr; p = p->ai_next)
	{
		// create a socket file description
		if ((this->socket_file_desc = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			ctLogInfo << "Could not create socket for family: " << p->ai_family << ", protocol: " << p->ai_protocol << ctLogEnd;
			continue;
		}

		// tell sockets api to reuse the address
		const int option_value = 1;
		if (setsockopt(this->socket_file_desc, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) == -1)
		{
			ctLogError << "Could not set the socket option on the socket file descriptor" << ctLogEnd;
			return status::cant_allocate;
		}

		// bind to the socket
		if (bind(this->socket_file_desc, p->ai_addr, p->ai_addrlen) == -1)
		{
			ctLogInfo << "Could not bind socket for for family: " << p->ai_family << ", protocol: " << p->ai_protocol << ctLogEnd;
			close_socket();
			continue;
		}

		// report success, and break out of loop
		ctLogInfo << "Socket successfully bound for family: " << p->ai_family << ", protocol: " << p->ai_protocol << ctLogEnd;
		found_and_bound_socket = true;
		break;
	}

	// dont need the address info anymore
	freeaddrinfo(servinfo);

	// make sure all is set up before continuing
	ctValidate(found_and_bound_socket, status::not_found) << "Could not match the selected protocol and bind successfully to a socket." << ctValidateEnd;

	// start listening to the socket
	if (listen(this->socket_file_desc, (int)backlog_size) == -1)
	{
		ctLogError << "Could not set the socket to listen to port" << port << ctLogEnd;
		return status::cant_allocate;
	}
	ctLogInfo << "Waiting for connections, listening on port: " << port << ctLogEnd;
	this->data->server_state = internal_data::state::running;
	
	// blocking accept loop
	while (this->data->server_state == internal_data::state::running)
	{
		// accept a connection to the listening socket
		sockaddr_storage remote_addr = {};
		socklen_t remote_addr_size = sizeof( remote_addr );
		int incoming_file_desc = accept(this->socket_file_desc, (struct sockaddr*)&remote_addr, &remote_addr_size);
		ctValidate(incoming_file_desc != -1, status::invalid) << "Call to socket accept() failed. errno=" << errno << ctValidateEnd;

		// get the address of the remote process, log it
		char remote_address[INET6_ADDRSTRLEN];
		inet_ntop(
			remote_addr.ss_family,
			get_inet_addr_pointer((sockaddr*)&remote_addr),
			remote_address,
			sizeof(remote_address)
		);
		ctLogInfo << "Accepted incoming connection from: " << remote_address << ctLogEnd;

		// call the provided function, to handle the incoming socket
		stream_socket incoming_socket(std::move(incoming_file_desc));
		ctStatusCall(serve_function(incoming_socket));
	}

	// we are done, close the socket and return
	close_socket();

	return status::ok;
}

status server_socket::start(const std::string& port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	// make sure the server is not already running, and change state to running
	if (this->data->server_state != internal_data::state::stopped)
		return status::already_initialized;
	this->data->server_state = internal_data::state::started;

	auto result = this->run_internal(port, serve_function, protocol_family, backlog_size);

	// clean up, change state to stopped, and make sure the socket is closed
	this->close_socket();
	this->data->server_state = internal_data::state::stopped;

	return result;
}
	
status server_socket::start(uint16_t port, const serve_func & serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	return this->start(std::to_string(port), serve_function, protocol_family, backlog_size);
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION