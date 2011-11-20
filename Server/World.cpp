#include "World.h"
#include "../Shared/BaseEntity.h"
#include "../Shared/PlayerEntity.h"
#include "../Shared/OutBuffer.h"
#include "../Shared/InBuffer.h"

#pragma  warning(disable:4244)


bool CompareID(const entity_ptr ent, const int id)
{
	return ent->ID() == id;
}

World::World(void) : running(true), world_timer(new Timer)
{
	entities.reserve(4);
	INIT_IN_BUFFER;
	INIT_OUT_BUFFER;
}

World::~World(void)
{
	DESTROY_IN_BUFFER;
	DESTROY_OUT_BUFFER;
}

bool World::CreateEntity()
{
	entity_ptr player(new PlayerEntity);
	
	if (!player)
		return false;

	player->ID(entities.size());
	player->Position(RandomPosition(0, 448));
	entities.push_back(player);
	
	OUT_BUFFER.PushMessage(entities.back());

	return true;
}

bool World::IsCollideable( const entity_ptr ent1, const entity_ptr ent2 ) const
{
	return true;
}

void World::Update()
{
	while (running)
	{
		if (!IN_BUFFER.Empty()) // was while
		{
			Message first_update(IN_BUFFER.Front());
			UpdateEntity(first_update);
			
			ent_itor it = std::find_if(entities.begin(), entities.end(),
				std::bind2nd(std::ptr_fun(CompareID), first_update.id));
						
			if (it != entities.end())
				OUT_BUFFER.PushMessage((*it));
		}
	}
}

math::Vector2 World::RandomPosition( const int upper_bound, const int lower_bound )
{
	boost::uniform_real<> dist(upper_bound, lower_bound);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<>> x(generator, dist);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<>> y(generator, dist);

	math::Vector2 pos(x(), y());

	return pos;
}

void World::UpdateEntity( const Message& msg )
{
	for (ent_itor it = entities.begin(); it!=entities.end(); ++it)
	{
		if ((*it)->ID() == msg.id)
		{
			DoUpdate(it, msg);
			break;
		}
	}
}

void World::DoUpdate( ent_itor it, const Message& temp )
{
	(*it)->Active(temp.active);
	(*it)->Health(temp.health);
	
	math::Vector2 p(temp.x, temp.y);
	(*it)->Position(p);	
}