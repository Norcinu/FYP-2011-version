#include "ProjectileEntity.h"
//#include "Visualisation.h"

ProjectileEntity::ProjectileEntity(void) 
{
	health = 1;
	giveDamage = 5;
}

ProjectileEntity::~ProjectileEntity(void)
{
}

void ProjectileEntity::OnCollision( entity_ptr ent )
{

}

void ProjectileEntity::Update()
{
	// keep moving until collision or off screen.
	if (position.x > 512 || position.x < 0) active = false;
	if (position.y > 512 || position.y < 0) active = false;
}

void ProjectileEntity::Render( const float dt )
{	
	//if (active)
		//VIS.DrawSprite(sprite_id, position);
}

