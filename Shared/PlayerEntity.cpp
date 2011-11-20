#include "PlayerEntity.h"
//#include "Visualisation.h"
#include <iostream>

PlayerEntity::PlayerEntity(void) 
{
	side = PLAYER;
}

PlayerEntity::~PlayerEntity(void)
{
}

void PlayerEntity::OnCollision(entity_ptr ent)
{
	printf("collision\n");
	if (active) 
	{
		//health -= ent->DamageToGive();
	}
}

void PlayerEntity::Update()
{
	if (health <= 0) active = false;

	if (moving) 
	{
		previous_pos.x = position.x;
		previous_pos.y = previous_pos.y;

		position.x += destination.x * 8;
		position.y += destination.y * 8;
		moving = false;
		
		std::cout << "pos = " << position << std::endl;
	}

	
}

void PlayerEntity::Render(const float dt)
{
	if (active) 
	{
		math::Vector2 lerped;
		lerped.Lerp(position, previous_pos, dt);
		//VIS.DrawSprite(id, lerped);
	}
}

