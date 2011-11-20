#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "baseentity.h"

// need to set a side when fired.
class ProjectileEntity : public BaseEntity
{
public:
	ProjectileEntity(void);
	~ProjectileEntity(void);

	void OnCollision(entity_ptr ent);
	void Update();
	void Render(const float dt);
};

#endif
