#pragma once

#include "collider.h"

namespace Skiff
{
	struct SKF_BoundingBox : public SKF_Collider
	{
		Vector3 min;
		Vector3 max;

		SKF_BoundingBox(Vector3 min_, Vector3 max_);

		SKF_CollisionInfo Test(SKF_Collider* collider) override;
		SKF_CollisionInfo Test(SKF_Plane* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingSphere* collider) override;
		SKF_CollisionInfo Test(SKF_BoundingBox* collider) override;

		void Transform(Vector3 translation) override;
		Vector3 GetCenter() override;
	};
}