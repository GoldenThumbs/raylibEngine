#pragma once

#include "../skiff.h"
#include "physics.h"
#include <raylib.h>

namespace Skiff
{
	struct SKF_Collider
	{
		u8 type;

		virtual SKF_CollisionInfo Test(SKF_Collider* collider) = 0;
		virtual SKF_CollisionInfo Test(SKF_Plane* collider) = 0;
		virtual SKF_CollisionInfo Test(SKF_BoundingSphere* collider) = 0;
		virtual SKF_CollisionInfo Test(SKF_BoundingBox* collider) = 0;

		virtual void Transform(Vector3 translation) = 0;
		virtual Vector3 GetCenter() = 0;
	};
}