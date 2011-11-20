#include "BaseEntity.h"
//#include "Visualisation.h"
//#include "SDL_Collide.h"

#include <SDL.h>

#pragma warning(disable:4244)

void BaseEntity::SetLevelData(Level * l)
{
}

bool BaseEntity::CheckCollision( entity_ptr ent )
{
	assert( ent );

	/*if (SDL_BoundingCollide(VIS.Surface(sprite_id), position.x, position.y, 
		VIS.Surface(ent->SpriteID()), ent->Position().x, ent->Position().y) == 1)
		return true;
	else */
		return false;

	/*steve::Rectangle me;
	steve::Rectangle them;
	
	me.left += Position().x; 
	me.right += Position().x;

	me.top += Position().y;
	me.bottom += Position().y;

	them.left += ent->Position().x;
	them.right += ent->Position().x;

	them.top += ent->Position().y;
	them.bottom += ent->Position().y;


	if ((me.right<them.left || me.right>them.right) || (me.bottom<them.top || me.top>them.bottom))
		return false;
	else 
		return true;*/
}