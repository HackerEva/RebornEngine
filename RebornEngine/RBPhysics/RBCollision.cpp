#include "RBCollision.h"
using namespace RebornEngine;
RBPHYSICS::RBPlane::RBPlane(const RBVector3 & va, const RBVector3 & vb, const RBVector3 & vc)
{
	normal = (vb - va).Cross(vc - va);
	normal.Normalize();
	offset = normal.Dot(va);
}

void RBPHYSICS::RBFrustum::BuildPlanesFromCorners()
{
	planes[0] = RBPlane(corners[RB_FC_NBR], corners[RB_FC_NTL], corners[RB_FC_NBL]);
	planes[1] = RBPlane(corners[RB_FC_FBL], corners[RB_FC_FTR], corners[RB_FC_FBR]);
	planes[2] = RBPlane(corners[RB_FC_NBL], corners[RB_FC_FTL], corners[RB_FC_FBL]);
	planes[3] = RBPlane(corners[RB_FC_FBR], corners[RB_FC_NTR], corners[RB_FC_NBR]);
	planes[4] = RBPlane(corners[RB_FC_NTR], corners[RB_FC_FTL], corners[RB_FC_NTL]);
	planes[5] = RBPlane(corners[RB_FC_NBL], corners[RB_FC_FBR], corners[RB_FC_NBR]);

}

RBPHYSICS::BRPlaneSpace RBPHYSICS::RBCollision::TestSphereToPlane(const RBPlane & plane, const RBSphere & sphere)
{
	float dist = sphere.center.Dot(plane.normal) - plane.offset;
	if (dist > sphere.radius)
		return RBPHYSICS::RBPlaneSpace_Front;
	else if (dist < -sphere.radius)
		return RBPlaneSpace_Back;
	else
		return RBPlaneSpace_Intersecting;

}

RBPHYSICS::BRPlaneSpace RBPHYSICS::RBCollision::TestAABBToPlane(const RBPlane & plane, const RBAABB & aabb)
{
	RBSphere s;
	s.center = (aabb.pMax + aabb.pMin) * 0.5f;
	RBVector3 e = aabb.pMax - s.center;
	s.radius = e.x * ABS(plane.normal.x) + e.y * ABS(plane.normal.y) + e.z * ABS(plane.normal.z);

	return TestSphereToPlane(plane, s);
}

bool RBPHYSICS::RBCollision::TestAABBInsideFrustum(const RBFrustum & frustum, const RBAABB & aabb)
{
	for (int i = 0; i < 6; i++)
	{
		if (TestAABBToPlane(frustum.planes[i], aabb) == RBPlaneSpace_Back)
			return false;
	}

	return true;

}

bool RBPHYSICS::RBCollision::TestSphereWithCapsule(const RBSphere & sphere, const RBCapsule & capsule)
{
	RBVector3 v = capsule.end - capsule.start;
	RBVector3 vn = v.GetNormalizedVec3();
	RBVector3 pt = sphere.center - capsule.start;
	float scale = pt.Dot(vn) / v.Dot(vn);
	RBVector3 cloest_pt;

	if (scale < 0)
		scale = 0;
	else if (scale > 1)
		scale = 1;

	cloest_pt = capsule.start + v * scale;

	float sqrdRadius = sphere.radius + capsule.radius;
	sqrdRadius *= sqrdRadius;

	return (sphere.center - cloest_pt).SquaredMagitude() < sqrdRadius;
}
