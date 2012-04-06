#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>

//#include "../Shared/Buffers.h"
#include "../Shared/Message.h"
#include "../Shared/InBuffer.h"
#include "../Shared/OutBuffer.h"
#include "../Shared/Utils.h"

#include "TCPConnection.h"

namespace fyp_ip = boost::asio::ip;
using namespace fyp_ip;

namespace net
{
	// pack this into a message as well a struct containing the data?
	enum msgtype_t
	{
		CONNECTING		= 0x01,
		CONNECTED		= 0x02,
		GAME_UPDATE		= 0x04,
		CHAT_MESSAGE	= 0x08,
		DISCONNECTING	= 0x10
	};

	class TCPServer
	{
	public:

		typedef std::list<TCPConnection::connection_ptr>::const_iterator tcp_const_itor;
		typedef std::list<TCPConnection::connection_ptr>::iterator tcp_itor;

		TCPServer(boost::asio::io_service& service) : new_client(false), 
			acceptor(service,tcp::endpoint(tcp::v4(), 23)), count(0)
		{
			std::cout << "Starting TCP Server." << std::endl;
			StartAccept();
		}

		~TCPServer(void)
		{
			std::cout << "Exiting TCPServer" << std::endl;
		}

		void CheckDisconnected()
		{
			for (static tcp_itor it=connections.begin();it != connections.end(); ++it)
			{
				if (!(*it)->Socket().is_open()) {
					//(*it)->Socket().close();
					connections.erase( it );
					std::cout << "active connections = " << connections.size() << std::endl;
				}
			}
		}

		void SendAllMessage(std::string msg)
		{
		//	static std::string msg = "test";
			for (tcp_const_itor it = connections.begin(); it!=connections.end(); ++it)
			{
				(*it)->SendMessage(msg); 
			}
		}

		const int Size() const { /*std::cout << "count " << count << std::endl;*/ return count;}

		const std::string& RemoteAddress() const // was non const.
		{
			return connections.back()->GetAddress();
		}

		short NewRemotePort() const 
		{
			// must be a better way.
			std::string port = "800";
			std::string last_digit = utils::str::ToString(connections.size()-1);
			port += last_digit;
			
			std::cout << port << std::endl;
			
			short p;
			utils::str::FromString<short>(p, port, std::dec);
			return p;
		}

	private:
		bool new_client;
		int count;
		tcp::acceptor acceptor;
		std::list<TCPConnection::connection_ptr> connections;

	private:
		void StartAccept()
		{
			TCPConnection::connection_ptr new_connection = TCPConnection::Create(acceptor.get_io_service());
			acceptor.async_accept(new_connection->Socket(), boost::bind(&TCPServer::HandleAccept, this, 
				new_connection, boost::asio::placeholders::error));
		}

		void HandleAccept(TCPConnection::connection_ptr con, const boost::system::error_code& error)
		{
			if (!error)
			{
				new_client = true;
				con->ClientID(connections.size());
				connections.push_back(con);
				++count;

				std::cout << "New TCP Connection." << std::endl;
				std::cout << "number of clients connected : " << connections.size() << std::endl;
				con->Start();
				StartAccept();
			}
		}
	};

	class UDPServer
	{
	public:
		typedef std::vector<udp::endpoint>::const_iterator const_itor;
		UDPServer(boost::asio::io_service& service) : socket(service, udp::endpoint(udp::v4(), 7000))
		{
			std::cout << "Starting UDP Server." << std::endl;
			StartReceive();
		}

		~UDPServer()
		{
			std::cout << "Exiting UDPServer" << std::endl;
			socket.close();
		}

		void AddEndPoint(const std::string& ip, const short port)
		{
			udp::endpoint new_ep(boost::asio::ip::address::from_string(ip), port);

			if (std::find(clients.begin(), clients.end(), new_ep) == clients.end()) 
			{
				std::cout << "Adding new address : " << new_ep << std::endl;
				clients.push_back(new_ep);
				//clients.push_back(endPoint);
			}
			else
				std::cout << "Found address." << std::endl;
		}

		void StartReceive()
		{
			socket.async_receive_from(boost::asio::buffer(receiveBuffer), endPoint, boost::bind(&UDPServer::HandleReceive, 
				this, boost::asio::placeholders::error));

		}

		void HandleReceive(const boost::system::error_code& error)
		{
			if (!error || error != boost::asio::error::message_size)
			{
				std::cout << "received : " << (*receiveBuffer.data()) << std::endl;
				// add to game buffer here.
				IN_BUFFER.PushMessage(receiveBuffer.data());
				StartReceive();
			}
			else
			{
				std::cerr << "*** ERROR in UDPServer::HandleReceive ***" << std::endl;
				StartReceive();
			}
		}
		
		void Send()
		{
			while (!OUT_BUFFER.Empty()) // might need to copy it.
			{
				for (static const_itor it = clients.begin();it != clients.end(); ++it)
				{
					static std::string msg = OUT_BUFFER.Front();
					std::cout << "sending " << (*it) << std::endl;
					socket.async_send_to(buffer(msg), (*it), 
						boost::bind(&UDPServer::HandleSend,this, msg));
				}
				//OUT_BUFFER.PopMessage();
			}
		}

		void HandleSend(std::string message)
		{
		}
	
	private:
		udp::socket socket;
		udp::endpoint endPoint;
		std::vector<udp::endpoint> clients;
		boost::array<char, 128> receiveBuffer;
		//boost::array<std::string, 1> receiveBuffer;
	};
}

#endif


