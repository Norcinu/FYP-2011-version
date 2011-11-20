#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "baseentity.h"

class PlayerEntity : public BaseEntity
{
public:
	PlayerEntity(void);
	~PlayerEntity(void);

	void OnCollision(entity_ptr ent);
	void Update();
	void Render(const float dt);
};

#endif
