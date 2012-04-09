#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include "../Shared/Utils.h"
#include <functional>

using namespace boost::asio;
using boost::asio::ip::tcp;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:

	typedef boost::shared_ptr<TCPConnection> connection_ptr;

	~TCPConnection(void)
	{
		socket.close();
	}

	static connection_ptr Create(boost::asio::io_service& service)
	{
		return connection_ptr(new TCPConnection(service));
	}

	tcp::socket& Socket() { return socket; }
	
	void Start()
	{
		socket.async_read_some(boost::asio::buffer(data, max_length), boost::bind(&TCPConnection::HandleRead, this,		
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		std::cout << "SENDING CLIENT ID : " << client_id << std::endl;
		//static std::string ref_ = utils::toString(reference_);
		//client_id = client_id_;
		
		// Dont wanna do this but my conversion does not seem to work.
		static std::string reference_id;
		switch (client_id)
		{
		case 0:
			reference_id = "0\r\n";
			break;
		case 1:
			reference_id = "1\r\n";
			break;
		case 2:
			reference_id = "2\r\n";
			break;
		case 3:
			reference_id = "3\r\n";
			break;
		}

		std::cout << "REFERENCE ID : " << reference_id << std::endl;
		//reference_id + "\r\n";

		socket.async_send(boost::asio::buffer(reference_id, reference_id.size()), 
			boost::bind(&TCPConnection::HandleWrite, this, placeholders::error, placeholders::bytes_transferred));
	}

	void SendMessage(const std::string msg)
	{
		static std::string meh = msg;
		socket.async_send(boost::asio::buffer(meh, sizeof(meh)), boost::bind(&TCPConnection::HandleWrite, this, 
			placeholders::error, placeholders::bytes_transferred));
	}

	const std::string GetAddress() const
	{
		return socket.remote_endpoint().address().to_string();
	}

	int ClientID() const { return client_id; }
	void ClientID(const int id) { client_id = id; }

private:	
	TCPConnection(boost::asio::io_service& service) : socket(service), udp_remote_port(0),
		client_id(0)
	{

	}

	void HandleRead(const boost::system::error_code& error, size_t bytes_transferred) 
	{
		if (!error)
		{
			std::cout << data << std::endl;
		}

		if (error == boost::asio::error::connection_reset ||
			error == boost::asio::error::bad_descriptor  ||
			error == boost::asio::error::eof)
		{
			socket.close();
		}	
	}

	void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error == boost::asio::error::connection_reset ||
			error == boost::asio::error::bad_descriptor  ||
			error == boost::asio::error::eof)
		{
			socket.close();
		}

		if (!error)
			std::cout << __FUNCTION__ << " OK." << std::endl;
		else
			std::cout << __FUNCTION__ << " error." << std::endl;
	} 

	bool IsErrorDisconnect(const boost::system::error_code& error)
	{
		return (error == boost::asio::error::connection_reset) ||
			(error == boost::asio::error::bad_descriptor)   ||
			(error == boost::asio::error::eof);
	}
	
	short udp_remote_port;
	int client_id;
	tcp::socket socket;
	std::string message;
	enum { max_length = 5 };
	boost::array<std::string, 1> rec_buffer;
	char data[max_length];
};

#endif
