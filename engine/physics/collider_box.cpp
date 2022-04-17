#include "collider_box.h"
#include <raymath.h>

#include "collider_plane.h"
#include "collider_sphere.h"

namespace Skiff
{
	SKF_BoundingBox::SKF_BoundingBox(Vector3 min_, Vector3 max_)
	{
		min = min_;
		max = max_;

		type = 2;
	}
	
	SKF_CollisionInfo SKF_BoundingBox::Test(SKF_Collider* collider)
	{
		return collider->Test(this);
	}
	SKF_CollisionInfo SKF_BoundingBox::Test(SKF_Plane* collider)
	{
		return CheckBoxAndPlane(*this, *collider);
	}
	SKF_CollisionInfo SKF_BoundingBox::Test(SKF_BoundingSphere* collider)
	{
		return CheckSphereAndBox(*collider, *this);
	}
	SKF_CollisionInfo SKF_BoundingBox::Test(SKF_BoundingBox* collider)
	{
		return CheckBoxes(*this, *collider);
	}

	void SKF_BoundingBox::Transform(Vector3 translation)
	{
		min = Vector3Add(min, translation);
		max = Vector3Add(max, translation);
	}
	Vector3 SKF_BoundingBox::GetCenter()
	{
		return Vector3Scale(Vector3Add(max, min), 0.5f);
	}
}