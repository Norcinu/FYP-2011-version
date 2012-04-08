#include <iostream>
#include "Server.h"
#include "../Shared/Timer.h"
#include "../Shared/Utils.h"
#include "../Shared/Buffers.h"
#include "World.h"
#include <string>

#include "../Shared/TimerMacros.h"

int main(int argc, char *argv[])
{
	unsigned long start_time = GET_TIME;
	int client_count = 0;
	unsigned long frequency = 50;

	MessageBuffer<std::string> out_buffer_;
	MessageBuffer<Message> in_buffer_;

	try
	{
		boost::shared_ptr<Timer> game_timer(new Timer);
		boost::shared_ptr<World> world(new World);

		boost::asio::io_service service;
		net::TCPServer tcp_server(service);
		net::UDPServer udp_server(service);

		boost::thread network_service(boost::bind(&boost::asio::io_service::run, &service));

		static auto last_time = 0;
		while (world->Running())
		{
            SLEEP(frequency);
		    auto time_now = timing::ElapsedTime(start_time);

        	    if (tcp_server.Size() > client_count)
	            {
		        if (!world->CreateEntity())
			{
			    std::cerr << "**** Error creating entity ****" << std::endl;
			}
			else
			{
			    std::string n = "new client created";
			    std::cout << n << " : " << "\nplayer count : " << client_count << std::endl;
			    ++client_count;

			    std::string address = tcp_server.RemoteAddress();
    			short port = tcp_server.NewRemotePort();
			    udp_server.AddEndPoint(address, port);
			}
		    }

		    if (time_now > frequency && !OUT_BUFFER.Empty())
		    {
		        udp_server.Send(); // change timeGetTime() usage here.
			    std::cout << "[Sending UDP update] : [timestamp = " << GET_TIME << "]" << std::endl;
			    last_time = time_now;
		    }
		}
		
		network_service.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
