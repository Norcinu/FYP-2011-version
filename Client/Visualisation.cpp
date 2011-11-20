#include "Visualisation.h"
#include "Sprite.h"
#include <cassert>
#include <iostream>

Visualisation* Singleton<Visualisation>::m_instance = 0;

#pragma warning(disable:4244)

Visualisation::Visualisation(void)
{
	screen = NULL;
	screenWidth  = 512;
	screenHeight = 512;
	screenDepth  = 24;
}

Visualisation::~Visualisation(void)
{
	SDL_FreeSurface(screen);
	Quit(0);
}

bool Visualisation::Initialise() 
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		return false;

	screen = SDL_SetVideoMode(screenWidth, screenHeight, screenDepth, 
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ASYNCBLIT);
	
	SDL_ShowCursor(SDL_DISABLE);
	
	SDL_WM_SetCaption("Steven Gleed FYP 2010", NULL);
	
	return true;
}

void Visualisation::AddSprite( int * id, const std::string& file )
{
	boost::shared_ptr<Sprite> spr(new Sprite);
	
	if (!spr->Load(file))
	{
		std::string fail_message = "Error Loading Sprite File : " + file;
		throw fail_message;
	}

	sprites.push_back(spr);
	*id = static_cast<int>(sprites.size() - 1);
	
	printf("sprite added ok. ID = %d\n", *id);
}

void Visualisation::ClearScreen()
{
	CalculateFPS();		
	SDL_FillRect(SDL_GetVideoSurface(), NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 0,0,0));
	SDL_Flip(screen);
}

void Visualisation::ClearScreen( Uint32 colour )
{
	SDL_FillRect(screen, NULL, colour);
}

void Visualisation::CalculateFPS()
{	
	static unsigned long lastTime = SDL_GetTicks();
	static long frames = 0;
	static long fps = 0;

	int newTime = SDL_GetTicks();

	if(newTime - lastTime > 1000)
	{
		unsigned long newFPS = (frames * 1000 / (newTime - lastTime));
		printf("FPS : %d\n", newFPS);
		lastTime = newTime;
		frames = 0;
	}
	frames++;
}

void Visualisation::FillRectangle( const SDL_Rect & rec, const std::string col )
{
	//ColourMap::
	//SDL_FillRect (SDL_GetVideoSurface(), &rec, SDL_MapRGB(SDL_GetVideoSurface()->format, ColourMap::GetColour(col)));
}

void Visualisation::Quit( const int exit_code )
{
	printf("Quitting with code : %d\n", exit_code);
}

void Visualisation::BeginBatch()
{
	CalculateFPS();

	if(SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	ClearScreen(SDL_MapRGB(screen->format, 128, 128, 128)); 
	
	if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
}

void Visualisation::EndBatch()
{
	SDL_Flip(screen);
}

void Visualisation::DrawSprite( const int id, const math::Vector2& pos )
{
	sprites[id]->Render(screen, pos);
}

void Visualisation::DrawLine( math::Vector2& p1, math::Vector2& p2, Uint32 colour )
{
	if (colour != 0)
		printf("colour is not 0\n");

	double slope = static_cast<double>((p2.y - p1.y)) / static_cast<double>((p2.x - p1.x));
	for (int x = p1.x; x <= p2.x; x++)
	{
		int y = static_cast<int>((slope * (x - p1.x)) + p1.y);
		SetPixel(x, y, colour);
	}
}

void Visualisation::SetPixel( int x, int y, const Uint32 colour )
{
	int bpp = screen->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = colour;
		break;

	case 2:
		*(Uint16 *)p = colour;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
		{
			p[0] = (colour >> 16) & 0xff;
			p[1] = (colour >> 8) & 0xff;
			p[2] = colour & 0xff;
		} 
		else 
		{
			p[0] = colour & 0xff;
			p[1] = (colour >> 8) & 0xff;
			p[2] = (colour >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = colour;
		break;
	}
}

SDL_Surface * Visualisation::Surface(const int id) const
{
	return sprites[id]->RawSprite();
}

SDL_Rect Visualisation::Rectangle(const int id) const
{
	return sprites[id]->SpriteRectangle();
}
