#include "maths.h"

namespace steve {

	class Rectangle
	{
	public:
		Rectangle() : left(0), right(32), bottom(32), top(0)
		{
		}

		Rectangle(float l, float r, float b, float t) : left(l), right(r),
			bottom(b), top(t)
		{
		}

		Rectangle(const Rectangle& copy) : left(copy.left), right(copy.right),
			bottom(copy.bottom), top(copy.top)
		{
		}

		const float Width() const { return left - right; }
		const float Height() const { return bottom - top; }

		void Clip(const Rectangle& screenRec, math::Vector2& v)
		{
			left += v.x;
			right += v.x;
			top += v.y;
			bottom += v.y;

			if ( left < screenRec.left )
				left = screenRec.left;
			if ( right > screenRec.right )
				right = screenRec.right;
			if ( top < screenRec.top )
				top = screenRec.top;
			if ( bottom > screenRec.bottom )
				bottom = screenRec.bottom;

			left -= v.x;
			right -= v.x;
			top -= v.y;
			bottom -= v.y;
		}

		bool CompletelyContains(const int x, const int y, const Rectangle& other)
		{		
			if (x > 0 && x < right - other.Width() && y > 0 && y < bottom - other.Height())
				return true;
			else
				return false;
		}

		bool CompletelyOutside(const int x, const int y, const Rectangle& other)
		{
			if (x > right)
				return true;
			if (y > bottom)
				return true;
			if (x < left - other.Width())
				return true;
			if (y < top - other.Height())
				return true;

			return false;
		}

		float left, right, bottom, top;
	};
}