#include "collider_sphere.h"
#include <raymath.h>

#include "collider_plane.h"
#include "collider_box.h"

namespace Skiff
{
	SKF_BoundingSphere::SKF_BoundingSphere(Vector3 p, float r)
	{
		position = p;
		radius = r;

		type = 1;
	}

	SKF_CollisionInfo SKF_BoundingSphere::Test(SKF_Collider* collider)
	{
		return collider->Test(this);
	}
	SKF_CollisionInfo SKF_BoundingSphere::Test(SKF_Plane* collider)
	{
		return CheckSphereAndPlane(*this, *collider);
	}
	SKF_CollisionInfo SKF_BoundingSphere::Test(SKF_BoundingSphere* collider)
	{
		return CheckSpheres(*this, *collider);
	}
	SKF_CollisionInfo SKF_BoundingSphere::Test(SKF_BoundingBox* collider)
	{
		return CheckSphereAndBox(*this, *collider);
	}

	void SKF_BoundingSphere::Transform(Vector3 translation)
	{
		position = Vector3Add(position, translation);
	}
	Vector3 SKF_BoundingSphere::GetCenter()
	{
		return position;
	}
}
