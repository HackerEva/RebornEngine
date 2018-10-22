#ifndef RBCOLOR_H
#define RBCOLOR_H
#include "RBVector.h"

namespace RebornEngine
{
	namespace RBMATH
	{
		class RBColor
		{
		public:
			float r, g, b, a;

			RBColor()
			{}

			RBColor(float _r, float _g, float _b, float _a = 1.0f)
				: r(_r), g(_g), b(_b), a(_a)
			{}

			RBColor(const float* c)
				: r(c[0]), g(c[1]), b(c[2]), a(c[3])
			{}

			RBColor(const RBColor& rhs)
				: r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
			{}

			RBColor& operator=(const RBColor& rhs)
			{
				r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a;
				return *this;
			}
		};

	}
}


#endif // RBCOLOR_H
