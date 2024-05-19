
#define CTLE_IMPLEMENTATION

#include "ctle/sockets.h"

#include <future>
#include <thread>

using namespace ctle;

status tcpthread(stream_socket threadsocket)
{
	size_t sent = 0;
	size_t recvd = 0;
	srand(time(nullptr));
	std::vector<char> buffer(4096);

	std::string val = std::string("Hej") + std::to_string(rand());

	auto result = status::ok;
	while (result == status::ok)
	{
		result = threadsocket.recv(buffer.data(), buffer.size(), recvd);
		std::cout << std::this_thread::get_id() << " received" << std::string(buffer.data(), buffer.size()) << "\n";
		if (recvd == 0)
			break;
		result = threadsocket.send(val.data(), val.size(), sent);
		std::cout << std::this_thread::get_id() << " sent" << val << "\n";
	}
	std::cout << std::this_thread::get_id() << " exited\n";
	
	return result;
}

int main(int, char* [])
{
	ctle::server_socket serv;

	serv.start(3580, []( stream_socket &incoming ) -> status
		{
			std::thread(tcpthread, std::move(incoming) ).detach();
			return status::ok;
		}
	);

	return 0;
}
