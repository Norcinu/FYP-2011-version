#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void Run();
		
	bool Running() const { return running; }
	static void Running(bool val) { running = val; }
private:
	static bool running;
};

#endif
