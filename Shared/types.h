#ifndef TYPES_H
#define TYPES_H

#include <boost/shared_ptr.hpp>
#include <SDL.h>

class Sprite;
class BaseEntity;

typedef boost::shared_ptr<SDL_Surface> surface_ptr;
typedef boost::shared_ptr<Sprite> sprite_ptr;
typedef boost::shared_ptr<BaseEntity> entity_ptr;

#endif
