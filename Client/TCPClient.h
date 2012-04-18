#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <cstdio>
#include <cstring>

class chat_message
{
public:
  enum { header_length = 4 };
  enum { max_body_length = 512 };

  chat_message()
    : body_length_(0)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  size_t body_length() const
  {
    return body_length_;
  }

  void body_length(size_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    using namespace std; // For strncat and atoi.
    char header[header_length + 1] = "";
    strncat(header, data_, header_length);
    body_length_ = atoi(header);
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    using namespace std; // For sprintf and memcpy.
    char header[header_length + 1] = "";
    sprintf(header, "%4d", body_length_);
    memcpy(data_, header, header_length);
  }

private:
  char data_[header_length + max_body_length];
  size_t body_length_;
};



using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
      socket_(io_service)
  {
    boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&chat_client::handle_connect, this,
          boost::asio::placeholders::error));
  }

  void write(const chat_message& msg)
  {
    io_service_.post(boost::bind(&chat_client::do_write, this, msg));
  }

  void close()
  {
    io_service_.post(boost::bind(&chat_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.data(), chat_message::header_length),
          boost::bind(&chat_client::handle_read_header, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_read_header(const boost::system::error_code& error)
  {
    if (!error && read_msg_.decode_header())
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
          boost::bind(&chat_client::handle_read_body, this,
            boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void handle_read_body(const boost::system::error_code& error)
  {
    if (!error)
    {
      std::cout.write(read_msg_.body(), read_msg_.body_length());
      std::cout << "\n";
      boost::asio::async_read(socket_,
          boost::asio::buffer(read_msg_.data(), chat_message::header_length),
          boost::bind(&chat_client::handle_read_header, this,
            boost::asio::placeholders::error));
    }
    else
    {
      do_close();
    }
  }

  void do_write(chat_message msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(write_msgs_.front().data(),
            write_msgs_.front().length()),
          boost::bind(&chat_client::handle_write, this,
            boost::asio::placeholders::error));
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      write_msgs_.pop_front();
      if (!write_msgs_.empty())
      {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
              write_msgs_.front().length()),
            boost::bind(&chat_client::handle_write, this,
              boost::asio::placeholders::error));
      }
    }
    else
    {
      do_close();
    }
  }

  void do_close()
  {
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};
#endif
//#ifndef TCPCLIENT_H
//#define TCPCLIENT_H
//
//#include <boost/asio.hpp>
//#include <boost/thread.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/bind.hpp>
//#include <boost/array.hpp>
//
//#include <cassert>
//#include <iostream>
//
//#include "UDPClient.h"
//#include "World.h"
//
//using boost::asio::ip::tcp;
//
//void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
//
//namespace net 
//{
//	class TCPConnection
//	{
//	public:
//		TCPConnection(void) : /*once(false),*/ stop_requested(false), clientID(0) {}
//		
//		TCPConnection(const char *msg) : stop_requested(false), clientID(0)
//		{
//			assert (msg);
//			
//			send_buffer.at(0) = msg[0];
//			send_buffer.at(1) = msg[1];
//			send_buffer.at(2) = msg[2];
//			send_buffer.at(3) = msg[3];
//			send_buffer.at(4) = '\0';
//		}
//
//		~TCPConnection(void) {}
//
//		void Start()
//		{
//			assert(!m_thread);
//			m_thread = boost::shared_ptr<boost::thread> 
//				(new boost::thread(boost::bind(&TCPConnection::Connect, this)));
//	#ifdef _DEBUG
//			std::cout << "*** Starting TCP Connection thread. ***" << std::endl;
//	#endif
//		}
//
//		void Stop()
//		{
//			assert(m_thread);
//			stop_requested = true;
//			m_thread->join();
//	#ifdef _DEBUG
//			std::cout << "*** TCP Thread Stopped. ***" << std::endl;
//	#endif
//		}
//
//	int ClientID() const { return clientID; }
//	bool Connected() const { return once; }
//
//	private:
//		bool once;
//		volatile bool stop_requested;
//		int clientID;
//		boost::array<char, 5> send_buffer;
//		boost::shared_ptr<boost::thread> m_thread;
//		enum { max_length = 100 }; // 8192
//
//		void Connect()
//		{
//			while(!stop_requested)
//			{
//				try 
//				{
//					boost::asio::io_service io_service_;
//					tcp::resolver resolver_(io_service_);
//					tcp::resolver::query query_("127.0.0.1", "23");
//					
//					tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);
//					tcp::resolver::iterator end;
//					
//					std::cout << "host pc : " << boost::asio::ip::host_name() << std::endl; 
//					tcp::socket socket_(io_service_);
//					
//					boost::system::error_code error_ = boost::asio::error::host_not_found;
//					while(error_ && endpoint_iterator != end)
//					{
//						socket_.close();
//						socket_.connect(*endpoint_iterator++, error_);
//					}
//					
//					if (error_)
//						throw boost::system::system_error(error_);
//
//					
//					async_write( socket_, boost::asio::buffer(send_buffer, send_buffer.size()),
//						boost::bind(&handle_write, boost::asio::placeholders::error, 
//						boost::asio::placeholders::bytes_transferred) );
//					
//					/*once = true;
//					Message msg;*/
//					for(;;)
//					{
//						boost::array<char, 100> buf;
//						//boost::array<std::string, 1> buf;
//						boost::system::error_code error;
//						
//						size_t len = socket_.read_some(boost::asio::buffer(buf), error);
//						
//						/*if (!net::FromString<int>(clientID, buf.data(), std::dec))
//							std::cout << "error converting to int" << std::endl;*/
//						std::cout << "Client ID " << clientID << std::endl;
//						WORLD.PlayerID(clientID);
//
//						/*if (once)
//						{
//							WORLD.CreateEntity(clientID, msg);
//							once = false;
//						}*/
//						
//						if (error == boost::asio::error::eof) 
//						{
//							std::cout << "CONNECTION TO SERVER LOST : EOF" << std::endl;
//							break; // Connection closed cleanly by peer.
//						}
//						else if (error) 
//							throw boost::system::system_error(error); // Some other error.
//						
//						//std::cout << buf.data() << std::endl;
//						std::cout.write(buf.data(), len);	
//					}
//					//once = false;
//					stop_requested = true;
//				}
//				catch (std::exception& e)
//				{
//					std::cout << e.what() << std::endl;
//				}
//			}
//		}		
//	};
//}
//
//// is not working in the namespace net::
//void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
//{
//	if (!error)
//	{
//	}
//	else
//		std::cout << "ERROR IN " << __FUNCTION__ << std::endl;
//}
//
//
//#endif
