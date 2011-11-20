#include "Input.h"
#include "Engine.h"
#include "World.h"

Input::Input(void) 
{
	forward = backward = left = right = false;
}

Input::~Input(void)
{
}

void Input::Update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
			case SDLK_UP:
				printf("up arrow is down.\n");
				break;
			case SDLK_DOWN:
				printf("down arrow is down.\n");
				break;
			case SDLK_w:
				WORLD.MoveEntity(math::Vector2(0,-1));
				break;
			case SDLK_s:
				WORLD.MoveEntity(math::Vector2(0,1));
				break;
			case SDLK_a:
				WORLD.MoveEntity(math::Vector2(-1,0));
				break;
			case SDLK_d:
				WORLD.MoveEntity(math::Vector2(1,0));
				break;				
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch(event.key.keysym.sym)
			{	
			case SDLK_UP:
				printf("up arrow is up.\n");
				break;
			case SDLK_DOWN:
				printf("down arrow is up.\n");
				break;
			}
		}

		if (event.type == SDL_QUIT)
			WORLD.Running(false);
	}
}