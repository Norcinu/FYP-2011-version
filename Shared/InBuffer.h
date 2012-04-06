#ifndef INBUFFER_H
#define INBUFFER_H

#include "Singleton.h"
#include <deque>
#include "Message.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
//#include <boost/thread.hpp>

/*
	This class handles messages from the network
	and are heading for the world model.

	TODO: Make this into an inherited base or make templated.
*/
class InBuffer : public Singleton<InBuffer>
{
public:
	InBuffer(void);
	~InBuffer(void);

	bool Empty() const;
	Message Front();

	void PushMessage(const std::string& msg);
	Message PopMessage();
	
	bool TryPop(std::string * popped_value);
	void WaitAndPop(std::string& popped_value);

private:
	std::deque<std::string> buffer;
	boost::condition_variable buffer_condition;
	mutable boost::mutex buffer_mutex;
};

#define INIT_IN_BUFFER InBuffer::CreateInstance()
#define IN_BUFFER InBuffer::GetInstance()
#define DESTROY_IN_BUFFER InBuffer::DeleteInstance()

#endif
