#include "Sprite.h"
#include <cassert>

Sprite::~Sprite()
{
	printf("deleting sprite\n");
	if (sprite) 
	{
		SDL_FreeSurface(sprite);
		sprite = NULL;
	}
}

bool Sprite::Load( const std::string& filename )
{
	sprite = SDL_LoadBMP(filename.c_str());

	if (!sprite) 
		return false;

	return true;
}

void Sprite::Render( SDL_Surface * screen, const math::Vector2& position )
{
	assert(screen);

	//SDL_Rect r;
	rec.x = static_cast<int>(position.x);
	rec.y = static_cast<int>(position.y);
	SDL_BlitSurface(sprite/*.get()*/, NULL, screen, &rec);
}
