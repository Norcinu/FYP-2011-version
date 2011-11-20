#include "World.h"
#include "Visualisation.h"

#include "../Shared/BaseEntity.h"
#include "../Shared/PlayerEntity.h"
#include "../Shared/ProjectileEntity.h"
#include "../Shared/InBuffer.h"
#include "../Shared/OutBuffer.h"

#include <algorithm>

#pragma warning(disable:4244)

World* Singleton<World>::m_instance = 0;

bool CompareID(const entity_ptr ent, const int id)
{
	return ent->ID() == id;
}

World::World(void) : world_timer(new Timer), previous_update(0), tick_rate(100), running(true)
{
	INIT_IN_BUFFER;
	INIT_OUT_BUFFER;
}

World::~World(void)
{
	DESTROY_IN_BUFFER;
	DESTROY_OUT_BUFFER;
	DELETE_VIS;
}

bool World::Initialise()
{
	CREATE_VIS;
	if (!VIS.Initialise())
	{
		std::cout << "Error loading SDL" << std::endl;
		return false;
	}

	const std::string p1 = "data\\player1.bmp";
	const std::string p2 = "data\\player2.bmp";
	const std::string p3 = "data\\player3.bmp";
	const std::string p4 = "data\\player4.bmp";
	int id = 0;

	try 
	{
		VIS.AddSprite(&id, p1);
		VIS.AddSprite(&id, p2);
		VIS.AddSprite(&id, p3);
		VIS.AddSprite(&id, p4);
	}
	catch (std::string& msg) //catch(char *msg)
	{
		std::cout << "error thrown : " << msg << std::endl;
		return false;
	}

	return true;
}


bool World::CreateEntity( const Message& new_player )
{
	entity_ptr p(new PlayerEntity);
			
	if (!CheckAllocation(p))
		return false;
	
	p->Active(new_player.active);
	p->ID(new_player.id);
	p->Health(new_player.health);
	p->Position(math::Vector2(new_player.x, new_player.y));
	
	entities.push_back(p);	
	return true;
}

bool World::CreateEntity( const int local_id, const Message new_player )
{
	std::cout << "creating" << std::endl;
	player_id = local_id;
	entity_ptr p(new PlayerEntity);
	
	if (!CheckAllocation(p))
		return false;

	p->Active(new_player.active);
	p->ID(local_id);
	p->Health(new_player.health);
	p->Position(math::Vector2(new_player.x, new_player.y));

	entities.push_back(p);

	return true;
}

bool World::CheckAllocation( entity_ptr ent )
{
	if (!ent)
		return false;
	return true;
}

void World::Update()
{
	//std::cout << "world update" << std::endl;
	if (world_timer->Milliseconds() - previous_update > tick_rate)
	{
		for(ent_itor it = entities.begin(); it != entities.end(); ++it)
			(*it)->Update();

		previous_update =  world_timer->Milliseconds();
	}

	//UpdateAnEntity();


	/*for (size_t i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i)->Active() && entities.at(i)->Side() != NEUTRAL)
		{
			for(size_t j = i+1; j < entities.size(); ++j)
			{
				if (IsCollideable(entities.at(i), entities.at(j)))
				{
					if (entities.at(i)->CheckCollision(entities.at(j)))
					{
						entities.at(i)->OnCollision(entities.at(j));
						entities.at(j)->OnCollision(entities.at(i));
					}
				}
			}
		}
	}*/


	VIS.BeginBatch();

	float delta = (world_timer->Milliseconds() / previous_update) / tick_rate;

	for_each(entity_ptr & ent, entities)
	{
		if (ent->Active())
			ent->Render(delta); // 1.0f
	}
	
	VIS.EndBatch();
}

void World::MoveEntity( math::Vector2& direction ) const
{
	entities.at(player_id)->Moving(true);
	entities.at(player_id)->Destination(direction);
	//Message send(entities.at(player_id));
	OUT_BUFFER.PushMessage(entities.at(player_id));
}

void World::CreateProjectile(const int number)
{
	const std::string laser = "data\\gfx\\laser.bmp";
	int id = 0;
	VIS.AddSprite(&id, laser);

	for (int i = 0; i < number; i++) 
	{
		entity_ptr p(new ProjectileEntity);
		p->ID(projectiles.size() - 1);
		p->SpriteID(id);
		projectiles.push_back(p);
	}
}

math::Vector2 World::RandomisePosition( const int upper_bound, const int lower_bound )
{
	boost::uniform_int<> dist(upper_bound, lower_bound);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<>> x(gen, dist);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<>> y(gen, dist);
    
	math::Vector2 pos;
	pos.x = x();	
	pos.y = y();
	
	return pos;
}

bool World::IsCollideable( const entity_ptr ent1, const entity_ptr ent2 ) const
{
	assert (ent1 && ent2);

	if (ent1->Side() != ent2->Side())
	{
		if (ent1->Side() != NEUTRAL && ent2->Side() != NEUTRAL)
			return true;
	}

	return false;
}

// id = which entity to fire from.
// projectile is position, side are inherited from entity
// that it is fired from.
void World::FireProjectile( const int id )
{
	ESide side_ = entities.at(id)->Side();
	math::Vector2 pos = entities.at(id)->Position();

	for(ent_itor it=projectiles.begin(); it!=projectiles.end(); ++it)
	{
		if ((*it)->Active())
		{
			(*it)->Side(side_);
			(*it)->Position(pos);
			break;
		}
	}
}

void World::CheckActiveProjectiles()
{
	int active = 0;
	for (ent_itor it=projectiles.begin(); it!=projectiles.end(); ++it)
	{
		if ((*it)->Active())
			++active;
		else
			((*it)->Active(true));
	}

#ifdef _DEBUG
	std::cout << "Active projectiles : " << active << std::endl;
#endif // _DEBUG
}

void World::UpdateAnEntity()
{
	while (!IN_BUFFER.Empty())
	{
	/*if (!IN_BUFFER.Empty())
	{*/
		Message temp(IN_BUFFER.Front() /*PopMessage()*/);

		ent_itor it = std::find_if(entities.begin(), entities.end(),
			std::bind2nd(std::ptr_fun(CompareID), temp.id));

		if(it != entities.end()) 
		{
			std::cout << "Success found entity!\n";
			// update the entity
			(*it)->Active(temp.active);
			(*it)->Health(temp.health);
			(*it)->Position(math::Vector2(temp.x, temp.y));
		}
		else
		{
			CreateEntity(temp);
		}
	}
}