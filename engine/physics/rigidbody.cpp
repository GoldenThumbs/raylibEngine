#include "rigidbody.h"

namespace Skiff
{
	SKF_RigidBody::SKF_RigidBody(SKF_Collider* collider_, float mass_, bool gravity, bool isDynamic_)
	{
		velocity = Vector3{ 0, 0, 0 };
		force = Vector3{ 0, 0, 0 };
		mass = mass_;
		if (mass == 0)
			invMass = 0;
		else
			invMass = 1.0f / mass;
		collider = collider_;
		usesGravity = gravity;
		isDynamic = isDynamic_;
	}
}