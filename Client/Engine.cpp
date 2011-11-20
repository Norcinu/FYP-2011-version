#include "Engine.h"
//#include "Visualisation.h"

bool Engine::running = false;

Engine::Engine(void) //: running(false)
{
}

Engine::~Engine(void)
{
}

void Engine::Run()
{
	if (!running)
		running = true;
}