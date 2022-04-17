#include "physics.h"
#include <raymath.h>

#include "collider_plane.h"
#include "collider_sphere.h"
#include "collider_box.h"

namespace Skiff
{
	float SKF_CollisionInfo::GetDistance()
	{
		return Vector3Length(normal);
	}

	Vector3 SKF_CollisionInfo::GetNormal()
	{
		return Vector3Normalize(normal);
	}



	SKF_CollisionInfo CheckBoxes(SKF_BoundingBox a, SKF_BoundingBox b)
	{
		Vector3 d;
		d.x = fmaxf(a.min.x - b.max.x, b.min.x - a.max.x);
		d.y = fmaxf(a.min.y - b.max.y, b.min.y - a.max.y);
		d.z = fmaxf(a.min.z - b.max.z, b.min.z - a.max.z);
	
		float dist = fmaxf(d.x, fmaxf(d.y, d.z));
	
		return SKF_CollisionInfo(dist < 0, Vector3Scale(Vector3Normalize(d), dist));
	}
	
	SKF_CollisionInfo CheckSpheres(SKF_BoundingSphere a, SKF_BoundingSphere b)
	{
		Vector3 d = Vector3Subtract(b.position, a.position);
	
		float dist = Vector3Length(d) - (a.radius + b.radius);
	
		return SKF_CollisionInfo(dist < 0, Vector3Scale(Vector3Normalize(d), dist));
	}
	
	SKF_CollisionInfo CheckSphereAndBox(SKF_BoundingSphere a, SKF_BoundingBox b)
	{
		Vector3 d;
		d.x = fmaxf(b.min.x, fminf(a.position.x, b.max.x));
		d.y = fmaxf(b.min.y, fminf(a.position.y, b.max.y));
		d.z = fmaxf(b.min.z, fminf(a.position.z, b.max.z));
	
		float dist = Vector3Length(Vector3Subtract(d, a.position)) - a.radius;
	
		return SKF_CollisionInfo(dist < 0, Vector3Scale(Vector3Normalize(d), dist));
	}
	
	SKF_CollisionInfo CheckSphereAndPlane(SKF_BoundingSphere a, SKF_Plane b)
	{
		float sphereDist = Vector3DotProduct(b.normal, a.position);
		float dist = sphereDist - b.distance - a.radius;

		return SKF_CollisionInfo(dist < 0, Vector3Scale(b.normal, dist));
	}
	
	SKF_CollisionInfo CheckBoxAndPlane(SKF_BoundingBox a, SKF_Plane b)
	{
		Vector3 center = Vector3Scale(Vector3Add(a.max, a.min), 0.5f);
		Vector3 extents = Vector3Subtract(a.max, center);
	
		float r = fabsf(Vector3DotProduct(extents, b.normal));
		float dist = Vector3DotProduct(b.normal, center) - b.distance - r;
	
		return SKF_CollisionInfo(dist < 0, Vector3Scale(b.normal, dist));
	}
}
