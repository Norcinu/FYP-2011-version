#ifndef OUTBUFFER_H
#define OUTBUFFER_H

#include "singleton.h"
#include "Message.h"
#include <string>
#include <deque>

#include <boost/thread.hpp>

/*
	This class handles messages that are to be passed
	to the network.

	TODO: Make this into an inherited base or make templated.
*/
class OutBuffer : public Singleton<OutBuffer>
{
public:
	OutBuffer(void);
	~OutBuffer(void);

	bool Empty() const;
	std::string Front();
	
	//void PushMessage(const entity_ptr ent);
	std::string PopMessage();
	void WaitAndPop(std::string& popped_value);

private:
	std::deque<std::string> buffer;
	boost::condition_variable buffer_condition;
	mutable boost::mutex buffer_mutex;
};

#define INIT_OUT_BUFFER OutBuffer::CreateInstance()
#define OUT_BUFFER OutBuffer::GetInstance()
#define DESTROY_OUT_BUFFER OutBuffer::DeleteInstance()

#endif
