#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include "../Shared/Utils.h"

#include <functional>
#include <set>
#include <deque>

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace boost::asio;
using boost::asio::ip::tcp;

class ChatMessage
{
public:
	enum { HEADER_LENGTH = 4 };
	enum { MAX_BODY_LENGTH = 512 };

	ChatMessage() 
		: body_length(0) 
	{}

	const char * Data() const { return data; }
	char * Data() { return data; }
	size_t Length() const { return HEADER_LENGTH + body_length; }
	const char * Body() const { return data + HEADER_LENGTH; }
	char * Body() { return data + HEADER_LENGTH; }
	size_t BodyLength() const { return body_length; }
	
	void BodyLength(size_t new_length)
	{
		body_length = new_length;
		if (body_length > MAX_BODY_LENGTH)
			body_length = MAX_BODY_LENGTH;
	}

	bool DecodeHeader()
	{
		using namespace std;
		char header[HEADER_LENGTH + 1] = "";
		
		strncat(header, data, HEADER_LENGTH);
		body_length = atoi(header);
		
		if (body_length > MAX_BODY_LENGTH)
		{
			body_length = 0;
			return false;
		}
		
		return true;
	}

	void EncodeHeader()
	{
		using namespace std;
		char header[HEADER_LENGTH + 1] = "";
		sprintf(header, "%4d", body_length);
		memcpy(data, header, HEADER_LENGTH);
	}

private:
	char data[HEADER_LENGTH + MAX_BODY_LENGTH];
	size_t body_length;
};

typedef std::deque<ChatMessage> ChatMessageQueue;

//-----------
class ChatParticipant
{
public:
	virtual ~ChatParticipant() {}
	virtual void Deliver(const ChatMessage& msg) = 0;
};

typedef boost::shared_ptr<ChatParticipant> ChatParticipantPtr;

//----------
class ChatRoom
{
public:
	ChatRoom() {}
	~ChatRoom() {}

	void Join(ChatParticipantPtr p)
	{
		participants.insert(p);
		std::for_each(recent_messages.begin(), recent_messages.end(),
			boost::bind(&ChatParticipant::Deliver, p, _1));
	}

	void Leave(ChatParticipantPtr p)
	{
		participants.erase(p);
	}

	void Deliver(const ChatMessage& msg)
	{
		recent_messages.push_back(msg);
		while (recent_messages.size() > MAX_RECENT_MESSAGES)
			recent_messages.pop_front();

		std::for_each(participants.begin(), participants.end(), 
			boost::bind(&ChatParticipant::Deliver, _1, boost::ref(msg)));
	}

private:
	std::set<ChatParticipantPtr> participants;
	enum { MAX_RECENT_MESSAGES = 100 };
	ChatMessageQueue recent_messages;
};

class ChatSession : public ChatParticipant,
	public boost::enable_shared_from_this<ChatSession>
{
public:
	ChatSession(io_service& ios, ChatRoom& r) :
	  socket(ios), 
	  room(r) 
	  {}

	  tcp::socket& Socket() { return socket; }

	  void Start()
	  {
		  room.Join(shared_from_this());
		  async_read(socket, buffer(read_msg.Data(), ChatMessage::HEADER_LENGTH),
			  boost::bind(&ChatSession::HandleReadHeader, shared_from_this(),
			  boost::asio::placeholders::error));
	  }

	  void Deliver(const ChatMessage& msg) 
	  {
		  bool writing = !write_msgs.empty();
		  write_msgs.push_back(msg);
		  if (!writing)
		  {
			  async_write(socket, buffer(write_msgs.front().Data(), write_msgs.front().Length()),
				  boost::bind(&ChatSession::HandleWrite, shared_from_this(),
				  placeholders::error));
		  }
	  }
	  
	  void HandleReadHeader(const boost::system::error_code& ec)
	  {
		  if (!ec && read_msg.DecodeHeader())
		  {
			  async_read(socket, buffer(read_msg.Body(), read_msg.Length()),
				  boost::bind(&ChatSession::HandleReadBody, shared_from_this(),
				  boost::asio::placeholders::error));
		  }
		  else
		  {
			  room.Leave(shared_from_this());
		  }
	  }

	  void HandleReadBody(const boost::system::error_code& ec) 
	  {
		  if (!ec)
		  {
			  std::cout << read_msg.Data() << std::endl;
			  room.Deliver(read_msg);
			  async_read(socket, buffer(read_msg.Data(), ChatMessage::HEADER_LENGTH),
				  boost::bind(&ChatSession::HandleReadHeader, shared_from_this(),
				  placeholders::error));
		  }
	  }

	  void HandleWrite(const boost::system::error_code& ec)
	  {
		  if (!ec)
		  {
			  write_msgs.pop_front();
			  if (!write_msgs.empty())
			  {
				  async_write(socket, buffer(write_msgs.front().Data(), write_msgs.front().Length()),
					  boost::bind(&ChatSession::HandleWrite, shared_from_this(),
					  placeholders::error));
			  }
		  }
		  else
			  room.Leave(shared_from_this());
	  }

private:
	tcp::socket socket;
	ChatRoom& room;
	ChatMessage read_msg;
	ChatMessageQueue write_msgs;
};

typedef boost::shared_ptr<ChatSession> ChatSessionPtr;

#endif
