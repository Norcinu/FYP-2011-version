#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include "BaseEntity.h"
#include "types.h"

//typedef boost::shared_ptr<BaseEntity> entity_ptr;

class Message
{
public:

	Message(void)
	{
	}

	Message(entity_ptr ent) : active(ent->Active()), id(ent->ID()), 
		health(ent->Health()), x(ent->Position().x), y(ent->Position().y)
	{
	}

	Message(const Message& copy)
	{
		active = copy.active;
		id = copy.id;
		health = copy.health;
		x = copy.x;
		y = copy.y;
	}

	~Message(void)
	{
	}

	friend std::ostream& operator <<(std::ostream& os, const Message& msg)
	{
		return os << msg.active << " " << msg.id << " " << msg.health << " "
			<< msg.x << " " << msg.y;
	}

	friend std::istream& operator >>(std::istream& is, Message& msg)
	{
		return is >> msg.active >> msg.id >> msg.health >> msg.x >> msg.y;
	}

	bool operator ==(const Message& other) { return id == other.id; }

	bool active;
	int id;
	int health;
	float x;
	float y;
};

#endif
