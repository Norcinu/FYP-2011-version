#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>
#include "../Shared/maths.h"

class Sprite
{
public:
	Sprite() : sprite(nullptr) {}

	~Sprite();
	
	bool Load(const std::string& filename);
	void Render(SDL_Surface * screen, const math::Vector2& position);
	SDL_Rect SpriteRectangle() const { return rec; }
	SDL_Surface * RawSprite() const { return sprite/*.get()*/; }

private:
	SDL_Surface * sprite;
	SDL_Rect rec;
};

#endif
