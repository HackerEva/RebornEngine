#ifndef RERAY_H
#define RERAY_H

#include "RBAABB.h"
namespace RebornEngine
{
	namespace RBPHYSICS
	{
		class RBRay
		{
		public:
		public:
			RBVector3 Origin;
			RBVector3 Direction;
			float Distance;

			RBRay();
			RBRay(const RBVector3& _origin, const RBVector3& _dir, float _dist);
			RBRay(const RBVector3& _start, const RBVector3& _end);

			RBRay Transform(const RBMatrix4x4& mat) const;
			bool TestAabbIntersection(const RBAABB& aabb, float* t = nullptr) const;

		};
	}
}




#endif
