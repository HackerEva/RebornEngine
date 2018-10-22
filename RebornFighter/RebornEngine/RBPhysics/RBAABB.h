#ifndef RBAABB_H
#define RBAABB_H
#include "../RBMath/RBMath.h"
namespace RebornEngine
{
	namespace RBPHYSICS
	{
		class RBAABB
		{
		public:

			RBVector3 pMin;
			RBVector3 pMax;

			RBAABB();
			inline void Expand(const RBVector3& p)
			{
				if (p.x < pMin.x) pMin.x = p.x;
				if (p.y < pMin.y) pMin.y = p.y;
				if (p.z < pMin.z) pMin.z = p.z;
				if (p.x > pMax.x) pMax.x = p.x;
				if (p.y > pMax.y) pMax.y = p.y;
				if (p.z > pMax.z) pMax.z = p.z;
			}

			inline void Expand(const RBAABB& aabb)
			{
				Expand(aabb.pMin);
				Expand(aabb.pMax);
			}

			inline void ExpandBySphere(const RBVector3& center, float radius)
			{
				if (center.x - radius < pMin.x) pMin.x = center.x - radius;
				if (center.y - radius < pMin.y) pMin.y = center.y - radius;
				if (center.z - radius < pMin.z) pMin.z = center.z - radius;
				if (center.x + radius > pMax.x) pMax.x = center.x + radius;
				if (center.y + radius > pMax.y) pMax.y = center.y + radius;
				if (center.z + radius > pMax.z) pMax.z = center.z + radius;
			}

			bool IsValid() const;

			RBAABB GetTransformedAabb(const RBMatrix4x4& m) const;
			RBAABB GetSweptAabb(const RBVector3& moveVec) const;

			bool TestPointInsideAabb(const RBVector3& point) const;
			bool TestIntersectionWithAabb(const RBAABB& aabb) const;

			// Check movement collision with another aabb.
			// Returns corrected movement vector result.
			RBVector3 TestDynamicCollisionWithAabb(const RBVector3& moveVec, const RBAABB& aabb) const;

			static RBAABB Default;
		};
	}

}
#endif // !RBAABB

