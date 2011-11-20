#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

#include <cassert>
#include <iostream>

#include "UDPClient.h"
#include "World.h"

using boost::asio::ip::tcp;

void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

namespace net 
{
	class TCPConnection
	{
	public:
		TCPConnection(void) : /*once(false),*/ stop_requested(false), clientID(0) {}
		
		TCPConnection(const char *msg) : stop_requested(false), clientID(0)
		{
			assert (msg);
			
			send_buffer.at(0) = msg[0];
			send_buffer.at(1) = msg[1];
			send_buffer.at(2) = msg[2];
			send_buffer.at(3) = msg[3];
			send_buffer.at(4) = '\0';
		}

		~TCPConnection(void) {}

		void Start()
		{
			assert(!m_thread);
			m_thread = boost::shared_ptr<boost::thread> 
				(new boost::thread(boost::bind(&TCPConnection::Connect, this)));
	#ifdef _DEBUG
			std::cout << "*** Starting TCP Connection thread. ***" << std::endl;
	#endif
		}

		void Stop()
		{
			assert(m_thread);
			stop_requested = true;
			m_thread->join();
	#ifdef _DEBUG
			std::cout << "*** TCP Thread Stopped. ***" << std::endl;
	#endif
		}

	int ClientID() const { return clientID; }
	bool Connected() const { return once; }

	private:
		bool once;
		volatile bool stop_requested;
		int clientID;
		boost::array<char, 5> send_buffer;
		boost::shared_ptr<boost::thread> m_thread;
		enum { max_length = 100 }; // 8192

		void Connect()
		{
			while(!stop_requested)
			{
				try 
				{
					boost::asio::io_service io_service_;
					tcp::resolver resolver_(io_service_);
					tcp::resolver::query query_("127.0.0.1", "23");
					
					tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);
					tcp::resolver::iterator end;
					
					std::cout << "host pc : " << boost::asio::ip::host_name() << std::endl; 
					tcp::socket socket_(io_service_);
					
					boost::system::error_code error_ = boost::asio::error::host_not_found;
					while(error_ && endpoint_iterator != end)
					{
						socket_.close();
						socket_.connect(*endpoint_iterator++, error_);
					}
					
					if (error_)
						throw boost::system::system_error(error_);

					
					async_write( socket_, boost::asio::buffer(send_buffer, send_buffer.size()),
						boost::bind(&handle_write, boost::asio::placeholders::error, 
						boost::asio::placeholders::bytes_transferred) );
					
					/*once = true;
					Message msg;*/
					for(;;)
					{
						boost::array<char, 100> buf;
						//boost::array<std::string, 1> buf;
						boost::system::error_code error;
						
						size_t len = socket_.read_some(boost::asio::buffer(buf), error);
						
						/*if (!net::FromString<int>(clientID, buf.data(), std::dec))
							std::cout << "error converting to int" << std::endl;*/
						std::cout << "Client ID " << clientID << std::endl;
						WORLD.PlayerID(clientID);

						/*if (once)
						{
							WORLD.CreateEntity(clientID, msg);
							once = false;
						}*/
						
						if (error == boost::asio::error::eof) 
						{
							std::cout << "CONNECTION TO SERVER LOST : EOF" << std::endl;
							break; // Connection closed cleanly by peer.
						}
						else if (error) 
							throw boost::system::system_error(error); // Some other error.
						
						//std::cout << buf.data() << std::endl;
						std::cout.write(buf.data(), len);	
					}
					//once = false;
					stop_requested = true;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}		
	};
}

// is not working in the namespace net::
void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
	}
	else
		std::cout << "ERROR IN " << __FUNCTION__ << std::endl;
}


#endif
