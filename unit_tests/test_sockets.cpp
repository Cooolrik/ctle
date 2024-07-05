// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/sockets.h"

#include "unit_tests.h"

#include <future>
#include <thread>

using namespace ctle;

static std::unique_ptr<ctle::server_socket> basic_server_socket;

static status basic_server_thread()
{
	return basic_server_socket->start(13584, []( stream_socket incoming ) -> status
		{
			// read the incoming socket message
			size_t recvd = 0;
			std::vector<char> buffer(4096);
			auto result = incoming.recv(buffer.data(), buffer.size(), recvd);
			if( !result )
				return result;

			// if message is "stop", tell the server to stop
			// in all cases, prepend the message
			std::string message = std::string( buffer.data(), recvd );
			if( message == "stop" )
				basic_server_socket->stop();
			message = std::string("answer:") + message;

			// reply back on the connected socket
			size_t sent;
			const auto send_result = incoming.send(message.data(), message.size(), sent);
			if( !send_result )
				return send_result;
			
			return status::ok;
		}
	);
}

static void connect_and_test_results( const std::string &message )
{
	const auto connection_result = stream_socket::connect("",13584);
	ASSERT_EQ( connection_result.status(), status::ok );

	const auto &socket = connection_result.value();

	// send a message
	size_t sent;
	const auto send_result = socket->send(message.data(), message.size(), sent);
	ASSERT_EQ( send_result, status::ok );

	// read the incoming socket message
	size_t recvd = 0;
	std::vector<char> buffer(4096);
	const auto recv_result = socket->recv(buffer.data(), buffer.size(), recvd);
	ASSERT_EQ( recv_result, status::ok );

	// make sure it matches expected message
	std::string recv_message = std::string( buffer.data(), recvd );
	ASSERT_EQ( std::string("answer:") + message , recv_message );
}

TEST( sockets, basic_server_test )
{
	// create a server socket and thread which just repeats the input back, unil "exit" is received
	basic_server_socket = std::unique_ptr<ctle::server_socket>( new ctle::server_socket );
	auto server_fut = std::async( basic_server_thread );
	
	// wait for server to start, give it 3 seconds
	ASSERT_TRUE( run_function_with_timeout( []() { return basic_server_socket->get_server_state() == ctle::server_socket::server_state::running; }, 3000 ) );

	// connect, send, and expect result
	connect_and_test_results("hello");
	connect_and_test_results("how much wood would a woodchuck chuck if a woodchuck could chuck wood?");
	connect_and_test_results("350kgs");
	connect_and_test_results("stop");

	// wait for server to stop, give it 3 seconds
	ASSERT_TRUE( run_function_with_timeout( []() { return basic_server_socket->get_server_state() == ctle::server_socket::server_state::stopped; }, 3000 ) );
}
