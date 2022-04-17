#pragma once

#include "collider.h"

namespace Skiff
{
	struct SKF_Plane : public SKF_Collider
	{
		Vector3 normal;
		float distance;

		SKF_Plane(Vector3 n, float d);

		SKF_CollisionInfo Test(SKF_Collider* collider) override;
		SKF_CollisionInfo Test(SKF_Plane* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingSphere* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingBox* collider) override;

		void Transform(Vector3 translation) override;
		Vector3 GetCenter() override;
	};
}