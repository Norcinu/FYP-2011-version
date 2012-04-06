#ifndef WORLD_H
#define WORLD_H

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/random.hpp>
#include <vector>
#include <algorithm>
#include <string>

#include "../Shared/Message.h"
#include "../Shared/Singleton.h"
#include "../Shared/Timer.h"
#include "../Shared/maths.h"
#include "../Shared/Message.h"

#define for_each BOOST_FOREACH 

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

class BaseEntity;
template <class MessageType> class MessageBuffer;//<Message>;
template <class MessageType> class MessageBuffer;//<std::string>;

typedef boost::shared_ptr<BaseEntity> entity_ptr;
typedef std::vector<entity_ptr>::iterator ent_itor;
typedef std::vector<entity_ptr>::const_iterator ent_const_itor;

class World : public Singleton<World>
{
public:
	World(void);
	World(MessageBuffer<Message> * in_buffer_, MessageBuffer<std::string> * out_buffer_);
	~World(void);

	bool CreateEntity();

	std::string Serialize();

	bool IsCollideable(const entity_ptr ent1, const entity_ptr ent2) const;
	math::Vector2 RandomPosition(const int upper_bound, const int lower_bound);
	void Update();
	void UpdateEntity(const Message& msg);
	bool Running() const { return running; }
	void CheckCollisions();

private:
	void DoUpdate( ent_itor it, const Message& temp );

private:
	bool running;
	int tick_rate;
	unsigned long previous_update;
	boost::shared_ptr<Timer> world_timer;
	std::vector<entity_ptr> entities;
	boost::mt19937 generator;

	MessageBuffer<std::string> * out_buffer;
	MessageBuffer<Message> * in_buffer;
};

#endif
