#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

class Input
{
public:
	Input(void);
	~Input(void);

	void Update();

private:
	bool forward;
	bool backward;
	bool left;
	bool right;
};

#endif
