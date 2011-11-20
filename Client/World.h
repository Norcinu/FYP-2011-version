#ifndef WORLD_H
#define WORLD_H

#include <boost/foreach.hpp>
#include <boost/random.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include "../Shared/Singleton.h"
#include "../Shared/types.h"
#include "../Shared/Timer.h"
#include "../Shared/maths.h"
#include "../Shared/Message.h"

#define for_each BOOST_FOREACH 

class World : public Singleton<World>
{
public:
	World(void);
	~World(void);

	bool Initialise();
	bool CreateEntity(const Message& new_player);
	bool CreateEntity(const int local_id, const Message new_player);
	bool IsCollideable(const entity_ptr ent1, const entity_ptr ent2) const;

	void Update();

	void UpdateAnEntity();
	void MoveEntity(math::Vector2& direction) const;
	void FireProjectile(const int id); 
	void CreateProjectile(const int number);
	
	math::Vector2 RandomisePosition(const int upper_bound, const int lower_bound);
	bool CanEntitySeeOther(const entity_ptr ent1, const entity_ptr ent2) const;
	void CheckActiveProjectiles();

	double GetWorldTimeMS() const { return world_timer->Milliseconds(); }
	double GetWorldTimeSeconds() const { return world_timer->Seconds(); }
	
	void Running(bool v) { running = v; } 
	bool Running() const { return running; }

	int PlayerID() const { return player_id; }
	void PlayerID(int val) { player_id = val; }

private:
	std::vector<entity_ptr> entities;
	std::vector<entity_ptr> projectiles;
	typedef std::vector<entity_ptr>::iterator ent_itor;
	bool CheckAllocation(entity_ptr ent);

private:
	bool running;
	boost::shared_ptr<Timer> world_timer;
	boost::mt19937 gen;
	int tick_rate;
	int player_id;
	unsigned long previous_update;
};

#define CREATE_WORLD World::CreateInstance()
#define WORLD World::GetInstance()
#define DELETE_WORLD World::DeleteInstance()

#endif
