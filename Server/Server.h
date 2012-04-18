#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>

#include "../Shared/Message.h"
#include "../Shared/Buffers.h"
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
		TCPServer(io_service& service, const tcp::endpoint& ep) :
		  m_service(service), 
		  m_acceptor(service, ep) { StartAccept(); }
		
		void StartAccept() 
		{
			ChatSessionPtr new_session(new ChatSession(m_service, m_room));
			m_acceptor.async_accept(new_session->Socket(), 
				boost::bind(&TCPServer::HandleAccept, this, new_session,
				boost::asio::placeholders::error));
		}
		
		void HandleAccept(ChatSessionPtr session, 
			const boost::system::error_code& ec)
		{
			if (!ec)
			{
				session->Start();
			}
			StartAccept();
		}

	private:
		boost::asio::io_service& m_service;
		tcp::acceptor m_acceptor;
		ChatRoom m_room;
	};

	typedef boost::shared_ptr<TCPServer> ChatServerPtr;
    
	class UDPServer
	{
	public:
		typedef std::vector<udp::endpoint>::const_iterator const_itor;
		UDPServer(boost::asio::io_service& service) : socket(service, udp::endpoint(udp::v4(), 7000))
		{
			std::cout << "Starting UDP Server." << std::endl;
			StartReceive();
		}

        UDPServer(boost::asio::io_service& service, MessageBuffer<Message> * in, 
            MessageBuffer<std::string> * out) :
            socket(service, udp::endpoint(udp::v4(), 7000)),
            out_buffer(out),
            in_buffer(in)
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
			}
			else
				std::cout << "Found address." << std::endl;
		}

		void StartReceive()
		{
			socket.async_receive_from(boost::asio::buffer(receiveBuffer), endPoint, 
                boost::bind(&UDPServer::HandleReceive, this, boost::asio::placeholders::error));

		}

		void HandleReceive(const boost::system::error_code& error)
		{
			if (!error || error != boost::asio::error::message_size)
			{
				std::cout << "received : " << (*receiveBuffer.data()) << std::endl;
                Message m(receiveBuffer.data());
                in_buffer->pushMessage(m);
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
            while (!out_buffer->isEmpty())
			{
				for (static const_itor it = clients.begin();it != clients.end(); ++it)
				{
                    static std::string msg = out_buffer->getAndPopFront<std::string>(); 
					std::cout << "sending " << (*it) << std::endl;
					socket.async_send_to(buffer(msg), (*it), 
						boost::bind(&UDPServer::HandleSend,this, msg));
				}
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
		MessageBuffer<std::string> * out_buffer;
		MessageBuffer<Message> * in_buffer;
	};
}

#endif
