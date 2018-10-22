#include"RBMathUtility.h"
#include<math.h>
#include<stdlib.h>

namespace RebornEngine
{
	namespace RBMATH
	{
		// Returns random float in [0, 1].
		float RandF()
		{
			return (float)(rand()) / (float)RAND_MAX;
		}

		// Returns random float in [a, b].
		float RandF(float a, float b)
		{
			return a + RandF()*(b - a);
		}
	}
}


