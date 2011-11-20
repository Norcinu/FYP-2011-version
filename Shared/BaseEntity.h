#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "maths.h"
#include "types.h"
#include "Rectangle.h"

#include <cassert>

enum ESide 
{
	PLAYER=1,
	ENEMY,
	NEUTRAL
};

enum EMovingState
{
	MOVING=1,
	IDLE
};

class Level;

class BaseEntity
{
public:
	BaseEntity(void) : active(true), moving(false), id(0), health(100), giveDamage(5), 
		speed(5), position(0,0), previous_pos(0,0), collisionBox()
	{
	}

	BaseEntity(int id_) : active(true), moving(false), id(id_), health(100), giveDamage(5), 
		speed(5), position(0,0), previous_pos(0,0), collisionBox()
	{
	}

	BaseEntity(const bool a, const int i, const int h, const math::Vector2 p) : active(a), moving(false),
		id(i), health(h), speed(5), position(p), previous_pos(0,0), collisionBox()
	{
	}
	
	virtual ~BaseEntity(void) {}

	bool IsSameID(const int id_)
	{
		return this->id == id_;
	}

	bool Active() const { return active; }
	void Active(bool val) { active = val; }
	
	bool Moving() const { return moving; }
	void Moving(bool m) { moving = m; }

	int ID() const { return id; }
	void ID(int val) { id = val; }

	int SpriteID() const { return sprite_id; }
	void SpriteID(int val) { sprite_id = val; }
	
	int Health() const { return health; }
	void Health(int val) { health = val; }
	
	math::Vector2 Position() const { return position; }
	void Position(math::Vector2& val) { position = val; }

	math::Vector2 Destination() const { return destination; }
	void Destination(math::Vector2& dest) { destination = dest; }

	steve::Rectangle CollisionBox() const { return collisionBox; }
	void CollisionBox(steve::Rectangle val) { collisionBox = val; }
	
	int DamageToGive() const { return giveDamage; }
	void DamageToGive(int val) { giveDamage = val; }
	
	ESide Side() const { return side; }
	void Side(ESide s) { side = s; }

	void SetLevelData(Level * l);

	bool CheckCollision(entity_ptr ent);

	virtual void OnCollision(entity_ptr ent) = 0;
	virtual void Update() = 0;
	virtual void Render(const float dt) = 0;
	
protected:
	bool active;
	bool moving;

	int id;
	int sprite_id;
	int health;
	int giveDamage;
	int speed;
	
	math::Vector2 position;
	math::Vector2 destination;
	math::Vector2 previous_pos;
	
	steve::Rectangle collisionBox;
	
	ESide side;
};

#endif
