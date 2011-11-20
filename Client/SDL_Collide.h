#include <cassert>

#pragma warning(disable:4715)

/*returns maximum or minimum of number*/
#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)


bool SDL_TransparentPixel(SDL_Surface *surface , int u , int v)
{
	/*assert that (u,v) offsets lie within surface*/
	assert( !((u < surface->w) || (v < surface->h)) );

	int bpp = surface->format->BytesPerPixel;
	/*here p is the address to the pixel we want to retrieve*/
	Uint8 *p = (Uint8 *)surface->pixels + v * surface->pitch + u * bpp;

	Uint32 pixel_colour;

	switch(bpp)
	{
		case(1):
			pixel_colour = *p;
		break;

		case(2):
			pixel_colour = *(Uint16 *)p;
		break;

		case(3):
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				pixel_colour = p[0] << 16 | p[1] << 8 | p[2];
			else
				pixel_colour = p[0] | p[1] << 8 | p[2] << 16;
		break;

		case(4):
			pixel_colour = *(Uint32 *)p;
		break;
	}

	return (pixel_colour == surface->format->colorkey);
}

/*
	SDL pixel perfect collision test
*/
bool SDL_Collide(SDL_Surface *as , int ax , int ay , SDL_Surface *bs , int bx , int by)
{
	int ax1 = ax + as->w - 1;
	int ay1 = ay + as->h - 1;
	
	int bx1 = bx + bs->w - 1;
	int by1 = by + bs->h - 1;

	if((bx1 < ax) || (ax1 < bx))
		return false;
	if((by1 < ay) || (ay1 < by))
		return false;

	int inter_x0 = SDL_COLLIDE_MAX(ax,bx);
	int inter_x1 = SDL_COLLIDE_MIN(ax1,bx1);

	int inter_y0 = SDL_COLLIDE_MAX(ay,by);
	int inter_y1 = SDL_COLLIDE_MIN(ay1,by1);

	for(int y = inter_y0 ; y <= inter_y1 ; y++)
	{
		for(int x = inter_x0 ; x <= inter_x1 ; x++)
		{
			if((SDL_TransparentPixel(as , x-ax , y-ay)) && 
				(SDL_TransparentPixel(bs , x-bx , y-by)))
				return true;
		}
	}
}

/*
	SDL bounding box collision test
*/
bool SDL_BoundingCollide(SDL_Surface *sa , int ax , int ay , SDL_Surface *sb , int bx , int by)
{
	if(bx + sb->w < ax)	return false;
	if(bx > ax + sa->w)	return false;
						
	if(by + sb->h < ay)	return false;
	if(by > ay + sa->h)	return false;

	return true;
}

/*
	SDL bounding box collision tests (works on SDL_Rect's)
*/
bool SDL_BoundingCollide(SDL_Rect a , SDL_Rect b)
{
	if(b.x + b.w < a.x)	return false;
	if(b.x > a.x + a.w)	return false;
									
	if(b.y + b.h < a.y)	return false;
	if(b.y > a.y + a.h)	return false;

	return true; 
}



