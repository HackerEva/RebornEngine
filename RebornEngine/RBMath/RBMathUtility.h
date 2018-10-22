#ifndef RBMATHUTILITY_h
#define RBMATHUTILITY_h
#include <float.h>


namespace RebornEngine
{
	namespace RBMATH
	{
		#define FLT_EQUAL(a,b)			(fabsf(a-b)<FLT_EPSILON)
		#define FLT_EQUAL_ZERO(a)		(fabsf(a)<FLT_EPSILON)
		#define PI 3.1415926f
		#define DEG_TO_RAD(deg)			(PI/180.0f*(deg))
		#define RAD_TO_DEG(rad)			((rad)*180.0f/PI)

		#ifndef max
		#define max(a,b)            (((a) > (b)) ? (a) : (b))
		#endif

		#ifndef min
		#define min(a,b)            (((a) < (b)) ? (a) : (b))
		#endif



		// Returns random float in [0, 1).
		float RandF();

		// Returns random float in [a, b).
		float RandF(float a, float b);

		template<typename T>
		const T& Max(const T& a, const T& b) { return (a > b) ? a : b; }

		template<typename T>
		const T& Min(const T& a, const T& b) { return (a < b) ? a : b; }

		template<typename T>
		const T& ABS(const T& _num) { return (_num >= 0) ? _num : _num * -1; }

		inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }


	}
}
#endif