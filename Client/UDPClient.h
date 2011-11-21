#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/thread.hpp>

#include <vector>
#include <iostream>
#include <sstream>

#include "../Shared/InBuffer.h"
#include "../Shared/OutBuffer.h"
#include "../Shared/Utils.h"

using boost::asio::ip::udp;

namespace net 
{
	//template <class T>
	//bool FromString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
	//{
	//	std::istringstream iss(s);
	//	return !(iss >> f >> t).fail();
	//}

	template<typename T>
	std::string ToString(T t)
	{
		std::ostringstream s;
		s << t;
		return s.str();
	}

	class UDPClient
	{
	public:

		UDPClient(void) : m_serverIP(""), m_serverPort("")
		{
		}

		UDPClient(const std::string port) : m_serverIP("127.0.0.1"), m_serverPort("7000")
		{
			CreateLocalEndpoint(port);
		}

		UDPClient(const std::string& ip, const std::string& port) : m_serverIP(ip), m_serverPort(port)
		{
			CreateLocalEndpoint(port);
		}
		void CreateLocalEndpoint( const std::string port ) 
		{
			short p;
			utils::str::FromString<short>(p, port, std::dec);
			udp::endpoint temp(boost::asio::ip::address::from_string("127.0.0.1"), p);

			m_localEndpoint = temp;
			std::cout << "temp : " << temp << " | m_localEndPoint : " << m_localEndpoint << std::endl;
		}

		~UDPClient(void)
		{
		}

		void SendUpdateToServer()
		{
			bool active = true;
			while (active)
			{		
				boost::asio::io_service io_service_;
				boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));

				udp::resolver resolver(io_service_);
				udp::resolver::query query(udp::v4(), m_serverIP,  m_serverPort);
				udp::endpoint rec_endpoint = *resolver.resolve(query);

				udp::socket socket_(io_service_);
					
				if (!socket_.is_open()) socket_.open(udp::v4());

				while (!OUT_BUFFER.Empty())
				{
					using boost::asio::buffer;
					static std::string s = OUT_BUFFER.Front(); //OUT_BUFFER.PopMessage();
					std::cout << "sending : " << s << std::endl;
					socket_.send_to(buffer(s), rec_endpoint);
				}
			
				t.join();
			}
		}

		void PopulateMessageVector(std::vector<std::string>& v)
		{
			//m_outMessages = v;
		}

		void ReceiveServerUpdate()
		{
			bool receiving = true;
			//while (receiving)
			//{
				std::cout << "receive thread" << std::endl;
				boost::asio::io_service io_service_;
				boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));

				udp::resolver resolver(io_service_);
				udp::resolver::query query(udp::v4(), m_serverIP,  m_serverPort);
				udp::endpoint rec_endpoint = *resolver.resolve(query);

				udp::socket socket_(io_service_);
				if (!socket_.is_open())
				{
					socket_.open(udp::v4());
					socket_.bind(m_localEndpoint);
				}

				udp::endpoint sender_endpoint;

				std::cout << "Receiving." << std::endl;
				using boost::asio::buffer;

				boost::array<char, 128> rec_buffer;
				size_t len = socket_.receive_from(buffer(rec_buffer), sender_endpoint);
				IN_BUFFER.PushMessage(rec_buffer.data());
				
				//receiving = false;
				t.join();
			//}

			std::cout << "exiting receive thread" << std::endl;
		}

		void handler(const boost::system::error_code& error, size_t bytes_transferred)
		{

		}

		void ServerIP(const std::string val) { m_serverIP = val; }
		void ServerPort(const std::string val) { m_serverPort = val; }

	private:
		std::string m_serverPort;
		std::string m_serverIP;
		udp::endpoint m_localEndpoint;
	};

}
#endif

