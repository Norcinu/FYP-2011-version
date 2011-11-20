#include "InBuffer.h"
//#include <boost/tr1/memory.hpp>
#include <strstream>
#include <sstream>

InBuffer* Singleton<InBuffer>::m_instance = 0;

InBuffer::InBuffer(void)
{
}

InBuffer::~InBuffer(void)
{
}

void InBuffer::PushMessage( const std::string& msg )
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	bool const was_empty = buffer.empty();
	buffer.push_back(msg);

	lock.unlock();

	if (was_empty)
		buffer_condition.notify_one();
}

Message InBuffer::PopMessage()
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	std::cout << "INBUFFER::PopMessage" << std::endl;
	
	std::istringstream stream(buffer.front());
	Message temp;
	stream >> temp;
	buffer.pop_front();
	
	return std::move(temp); // added std::move
}

bool InBuffer::Empty() const
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	return buffer.empty();
}

Message InBuffer::Front()
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	std::string s = buffer.front();
	std::istringstream stream(s);
	buffer.pop_front();
	
	Message temp;
	stream >> temp;
	
	return std::move(temp);
	//return temp;
}

void InBuffer::WaitAndPop( std::string& popped_value )
{	
	boost::mutex::scoped_lock lock(buffer_mutex);
	while(buffer.empty())
		buffer_condition.wait(lock);

	popped_value = buffer.front();
	buffer.pop_front();
}

bool InBuffer::TryPop( std::string * popped_value )
{
	return true;
}
