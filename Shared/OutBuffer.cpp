#include "OutBuffer.h"
#include <sstream>

OutBuffer* Singleton<OutBuffer>::m_instance = 0;

OutBuffer::OutBuffer(void)
{
}

OutBuffer::~OutBuffer(void)
{
	if (!buffer.empty())
		buffer.clear();
}

//void OutBuffer::PushMessage( const entity_ptr ent )
//{
//	Message temp(ent);
//	std::ostringstream stream;
//	stream << temp;
//	std::string string_message = stream.str() + "\r\n";
//	
//	boost::mutex::scoped_lock lock(buffer_mutex);
//	bool const was_empty = buffer.empty();
//	buffer.push_back(string_message);
//	
//	lock.unlock();
//	if (was_empty)
//		buffer_condition.notify_one();
//}

std::string OutBuffer::PopMessage()
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	std::string r = buffer.front();
	buffer.clear();
	return std::move(r);
}

//void OutBuffer::Wait()
//{
//	boost::mutex::scoped_lock lock(buffer_mutex);
//	while(buffer.empty())
//		condition.wait(lock);
//}

bool OutBuffer::Empty() const
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	return buffer.empty();
}

std::string OutBuffer::Front()
{
	boost::mutex::scoped_lock lock(buffer_mutex);
	std::string ret_val(buffer.front());
	buffer.pop_front();
	return ret_val;
}

void OutBuffer::WaitAndPop( std::string& popped_value )
{	
	boost::mutex::scoped_lock lock(buffer_mutex);
	while(buffer.empty())
		buffer_condition.wait(lock);

	popped_value = buffer.front();
	buffer.pop_front();
}