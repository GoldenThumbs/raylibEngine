#pragma once

#include "collider.h"

namespace Skiff
{
	struct SKF_BoundingSphere : public SKF_Collider
	{
		Vector3 position;
		float radius;

		SKF_CollisionInfo Test(SKF_Collider* collider) override;
		SKF_CollisionInfo Test(SKF_Plane* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingSphere* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingBox* collider) override;

		SKF_BoundingSphere() {}
		SKF_BoundingSphere(Vector3 p, float r);

		void Transform(Vector3 translation) override;
		Vector3 GetCenter() override;
	};
}