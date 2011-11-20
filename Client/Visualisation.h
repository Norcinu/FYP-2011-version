#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <string>
#include <vector>
#include <cassert>

#include <SDL.h>

#include "../Shared/types.h"
#include "../Shared/maths.h"
#include "../Shared/Singleton.h"

enum colours_e
{
	WHITE=1,BLACK,BLUE,GREEN,RED,PINK
};

class Sprite;

class Visualisation : public Singleton<Visualisation>
{
public:
	Visualisation(void);
	~Visualisation(void);

	bool Initialise();
	void AddSprite(int * id, const std::string& file); // was bool

	void ClearScreen();
	void ClearScreen(Uint32 colour);
	void FillRectangle(const SDL_Rect & rec, const std::string col); //non-const?
	void SetPixel(int x, int y, const Uint32 colour);
	
	void CalculateFPS();
	void Quit(const int exit_code);

	void DrawSprite(const int id, const math::Vector2& pos);
	void DrawLine(math::Vector2& p1, math::Vector2& p2, Uint32 colour = 0);
	void BeginBatch();
	void EndBatch();

	SDL_Surface * Surface(const int id) const; 
	SDL_Rect Rectangle(const int id) const;

private:
	SDL_Surface * screen;
	int screenWidth,screenHeight,screenDepth;
	std::vector<sprite_ptr> sprites;
};

#define CREATE_VIS Visualisation::CreateInstance()
#define VIS Visualisation::GetInstance()
#define DELETE_VIS Visualisation::DeleteInstance()

#endif
