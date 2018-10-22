#include "RBRay.h"
using namespace RebornEngine::RBPHYSICS;
RBRay::RBRay()
{
}

RBRay::RBRay(const RBVector3 & _origin, const RBVector3 & _dir, float _dist)
	: Origin(_origin), Direction(_dir.GetNormalizedVec3()), Distance(_dist)
{
}

RBRay::RBRay(const RBVector3 & _start, const RBVector3 & _end)
	: Origin(_start), Direction((_end - _start).GetNormalizedVec3()), Distance((_end - _start).Magnitude())

{
}

RBRay RBRay::Transform(const RBMatrix4x4 & mat) const
{
	return RBRay((RBVector4(Origin, 1.0f) * mat).ToVector3(), (RBVector4(Direction, 0.0f) * mat).ToVector3(), Distance);
}

bool RBRay::TestAabbIntersection(const RBAABB & aabb, float * t) const
{
	return false;
}
