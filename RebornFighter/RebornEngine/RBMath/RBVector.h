#ifndef RBVECTOR_H
#define RBVECTOR_H
#include "RBMathUtility.h"
#include <math.h>
namespace RebornEngine
{
	namespace RBMATH
	{
		class RBVector2;
		class RBVector3;
		class RBVector4;

		class RBVector2
		{
		public:
			float x, y;

			RBVector2()
			{}

			RBVector2(float _x, float _y)
				: x(_x), y(_y)
			{}

			RBVector2(const float* v)
				: x(v[0]), y(v[1])
			{}

			RBVector2(const RBVector2& rhs)
				: x(rhs.x), y(rhs.y)
			{}

			RBVector2& operator=(const RBVector2& rhs)
			{
				x = rhs.x; y = rhs.y;
				return *this;
			}

			RBVector2 operator-() const { return RBVector2(-x, -y); }

			RBVector2 operator+(const RBVector2& rhs) const { return RBVector2(x + rhs.x, y + rhs.y); }
			RBVector2 operator-(const RBVector2& rhs) const { return RBVector2(x - rhs.x, y - rhs.y); }

			RBVector2 operator*(float val) const { return RBVector2(x * val, y * val); }
			RBVector2 operator/(float val) const { return RBVector2(x / val, y / val); }

			RBVector2& operator+=(const RBVector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
			RBVector2& operator-=(const RBVector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
			RBVector2& operator*=(float val) { x *= val; y *= val; return *this; }
			RBVector2& operator/=(float val) { x /= val; y /= val; return *this; }

			// Get length of vector
			float Magnitude() const
			{
				return sqrtf(x * x + y * y);
			}

			// Make unit vector
			void Normalize()
			{
				float mag = Magnitude();
				if (!FLT_EQUAL_ZERO(mag))
				{
					x /= mag;
					y /= mag;
				}
			}

			// Dot product
			float Dot(const RBVector2& rhs) const
			{
				return x * rhs.x + y * rhs.y;
			}

			// Cross product
			float Cross(const RBVector2& rhs) const
			{
				return x * rhs.y - y * rhs.x;
			}

			static RBVector2 Zero()
			{
				return RBVector2(0.0f, 0.0f);
			}

			static RBVector2 Lerp(const RBVector2& a, const RBVector2& b, float t)
			{
				return RBVector2(RBMATH::Lerp(a.x, b.x, t), RBMATH::Lerp(a.y, b.y, t));
			}
		};

		class RBVector3
		{
		public:
			float x, y, z;

			RBVector3()
			{}

			RBVector3(float _x, float _y, float _z)
				: x(_x), y(_y), z(_z)
			{}

			RBVector3(const float* v)
				: x(v[0]), y(v[1]), z(v[2])
			{}

			RBVector3(const RBVector3& rhs)
				: x(rhs.x), y(rhs.y), z(rhs.z)
			{}

			RBVector3& operator=(const RBVector3& rhs)
			{
				x = rhs.x; y = rhs.y; z = rhs.z;
				return *this;
			}

			RBVector3 operator-() const { return RBVector3(-x, -y, -z); }

			RBVector3 operator+(const RBVector3& rhs) const { return RBVector3(x + rhs.x, y + rhs.y, z + rhs.z); }
			RBVector3 operator-(const RBVector3& rhs) const { return RBVector3(x - rhs.x, y - rhs.y, z - rhs.z); }

			RBVector3 operator*(float val) const { return RBVector3(x * val, y * val, z * val); }
			RBVector3 operator/(float val) const { return RBVector3(x / val, y / val, z / val); }

			RBVector3& operator+=(const RBVector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
			RBVector3& operator-=(const RBVector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
			RBVector3& operator*=(float val) { x *= val; y *= val; z *= val; return *this; }
			RBVector3& operator/=(float val) { x /= val; y /= val; z /= val; return *this; }

			float SquaredMagitude() const
			{
				return x * x + y * y + z * z;
			}

			// Get length of vector
			float Magnitude() const
			{
				return sqrtf(x * x + y * y + z * z);
			}

			// Make unit vector
			void Normalize()
			{
				float mag = Magnitude();
				if (!FLT_EQUAL_ZERO(mag))
				{
					x /= mag;
					y /= mag;
					z /= mag;
				}
			}

			RBVector3 GetNormalizedVec3() const
			{
				float mag = Magnitude();
				RBVector3 n = *this;
				if (!FLT_EQUAL_ZERO(mag))
				{
					float one_over_mag = 1.0f / mag;
					n.x *= one_over_mag;
					n.y *= one_over_mag;
					n.z *= one_over_mag;
				}
				return n;
			}

			// Dot product
			float Dot(const RBVector3& rhs) const
			{
				return x * rhs.x + y * rhs.y + z * rhs.z;
			}

			// Cross product
			RBVector3 Cross(const RBVector3& rhs) const
			{
				return RBVector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
			}

			static RBVector3 Zero()
			{
				return RBVector3(0.0f, 0.0f, 0.0f);
			}

			static RBVector3 Lerp(const RBVector3& a, const RBVector3& b, float t)
			{
				return RBVector3(RBMATH::Lerp(a.x, b.x, t),
					RBMATH::Lerp(a.y, b.y, t),
					RBMATH::Lerp(a.z, b.z, t));
			}
		};

		class RBVector4
		{
		public:
			float x, y, z, w;

			RBVector4()
			{}

			RBVector4(float _x, float _y, float _z, float _w = 1.0f)
				: x(_x), y(_y), z(_z), w(_w)
			{}

			RBVector4(const float* v)
				: x(v[0]), y(v[1]), z(v[2]), w(v[3])
			{}

			RBVector4(const RBVector4& rhs)
				: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
			{}

			RBVector4(const RBVector3 v, float _w = 1.0f)
				: x(v.x), y(v.y), z(v.z), w(_w)
			{}

			RBVector4& operator=(const RBVector4& rhs)
			{
				x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
				return *this;
			}

			RBVector4 operator*(float val) const { return RBVector4(x * val, y * val, z * val, w * val); }
			RBVector4 operator/(float val) const { return RBVector4(x / val, y / val, z / val, w / val); }

			RBVector4& operator*=(float val) { x *= val; y *= val; z *= val; w *= val; return *this; }
			RBVector4& operator/=(float val) { x /= val; y /= val; z /= val; w /= val; return *this; }

			float& operator[](int i) { return (&x)[i]; }

			RBVector3 ToVector3() const
			{
				return RBVector3(x, y, z);
			}

			static RBVector4 Lerp(const RBVector4& a, const RBVector4& b, float t)
			{
				return RBVector4(RBMATH::Lerp(a.x, b.x, t),
					RBMATH::Lerp(a.y, b.y, t),
					RBMATH::Lerp(a.z, b.z, t),
					RBMATH::Lerp(a.w, b.w, t));
			}
		};

	}

}

#endif