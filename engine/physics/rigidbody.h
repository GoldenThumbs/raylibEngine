#pragma once

#include "collider.h"

namespace Skiff
{
	struct SKF_RigidBody
	{
		Vector3 velocity;
		Vector3 force;
		float mass;
		float invMass;

		bool usesGravity;
		bool isDynamic;

		SKF_Collider* collider;

		SKF_RigidBody() {}
		SKF_RigidBody(SKF_Collider* collider_, float mass_, bool gravity, bool isDynamic_);
	};
}