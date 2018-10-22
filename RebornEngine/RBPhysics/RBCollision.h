#ifndef RBCOLLISION_H
#define RBCOLLISION_H
#include "../RBMath/RBMath.h"
#include "RBAABB.h"

namespace RebornEngine
{
	namespace RBPHYSICS
	{
		struct RBPlane
		{
			RBVector3 normal;
			float offset;

			RBPlane() {}
			RBPlane(const RBVector3& va, const RBVector3& vb, const RBVector3& vc);

		};

		struct RBSphere
		{
			RBVector3 center;
			float radius;
		};

		struct RBCapsule
		{
			RBVector3 start;
			RBVector3 end;
			float radius;
		};

		enum RBFrustumCorners
		{
			RB_FC_FTL,
			RB_FC_FTR,
			RB_FC_FBL,
			RB_FC_FBR,
			RB_FC_NTL,
			RB_FC_NTR,
			RB_FC_NBL,
			RB_FC_NBR,
		};

		struct RBFrustum
		{
			RBPlane planes[6];
			RBVector3 corners[8];

			void BuildPlanesFromCorners();
		};
		enum BRPlaneSpace
		{
			RBPlaneSpace_Front,
			RBPlaneSpace_Back,
			RBPlaneSpace_Intersecting,
		};

		namespace RBCollision
		{
			BRPlaneSpace TestSphereToPlane(const RBPlane& plane, const RBSphere& sphere);
			BRPlaneSpace TestAABBToPlane(const RBPlane& plane, const RBAABB& aabb);
			bool TestAABBInsideFrustum(const RBFrustum& frustum, const RBAABB& aabb);
			bool TestSphereWithCapsule(const RBSphere& sphere, const RBCapsule& capsule);

		}
	};

}

#endif // !RBCOLLISION_H
