#ifndef BUFFERS_H
#define BUFFERS_H

#include <deque>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

template <class MessageType>
class MessageBuffer
{
public:
	MessageBuffer() : buffer_size(0) {}
	~MessageBuffer() 
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while(!isEmpty())
		{
			popFront();
		}
		lock.unlock();
	}
	
	template <class MessageType>
	void pushMessage(const MessageType& m)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		buffer_.push_back(m);
		++buffer_size;
		
		lock.unlock();

		if (isEmpty())
			condition_.notify_one();
	}
	
	// do serialisation and de-serialisation elsewhere
	template <class MessageType>
	const MessageType& getFront() const
	{
		boost::mutex::scoped_lock lock(mutex_);
		return buffer_.front();
	}
	 
	template <class MessageType>
    MessageType getAndPopFront()
	{
		boost::mutex::scoped_lock lock(mutex_);
		auto temp = buffer_.front();
		buffer_.pop_front();
		--buffer_size;
		return std::move(temp);
	}

	bool isEmpty() const 
	{ 
		boost::mutex::scoped_lock lock(mutex_);
		return buffer_.empty(); 
	}

	const unsigned int getCurrentSize() const 
	{ 
		boost::mutex::scoped_lock lock(mutex_);
		return buffer_size;
	}
	
	void popFront() 
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		buffer_.pop_front();
		--buffer_size;
		lock.unlock();
	}

private:
	unsigned int buffer_size;
	std::deque<MessageType> buffer_;
	mutable boost::mutex mutex_;
	boost::condition_variable condition_;
};

#endif
