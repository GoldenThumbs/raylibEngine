#include "collider_plane.h"
#include <raymath.h>

#include "collider_sphere.h"
#include "collider_box.h"

namespace Skiff
{
	SKF_Plane::SKF_Plane(Vector3 n, float d)
	{
		normal = n;
		distance = d;

		type = 0;
	}

	SKF_CollisionInfo SKF_Plane::Test(SKF_Collider* collider)
	{
		return collider->Test(this);
	}
	SKF_CollisionInfo SKF_Plane::Test(SKF_Plane* collider)
	{ return SKF_CollisionInfo(false, Vector3{ 0, 0, 0 }); }
	SKF_CollisionInfo SKF_Plane::Test(SKF_BoundingSphere* collider)
	{
		return CheckSphereAndPlane(*collider, *this);
	}
	SKF_CollisionInfo SKF_Plane::Test(SKF_BoundingBox* collider)
	{
		return CheckBoxAndPlane(*collider, *this);
	}

	void SKF_Plane::Transform(Vector3 translation)
	{
		distance += translation.y;
	}
	Vector3 SKF_Plane::GetCenter()
	{
		return Vector3{ 0, distance, 0 };
	}
}