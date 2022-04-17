#include "physics_world.h"
#include <raymath.h>

#include "rigidbody.h"

namespace Skiff
{
	void SKF_ImpulseSolver::Solve(std::vector<SKF_Collision>& collisions, float delta)
	{
		const float staticFriction = 0.5f;
		const float dynamicFriction = 0.27f;
		for (SKF_Collision c : collisions)
		{
			SKF_RigidBody* bodyA = c.bodA->isDynamic ? c.bodA : nullptr;
			SKF_RigidBody* bodyB = c.bodB->isDynamic ? c.bodB : nullptr;

			Vector3 velA = bodyA ? bodyA->velocity : Vector3{ 0.0f };
			Vector3 velB = bodyB ? bodyB->velocity : Vector3{ 0.0f };
			Vector3 rVel = Vector3Subtract(velB, velA);
			float nVel = Vector3DotProduct(rVel, c.info.GetNormal());

			float invMassA = bodyA ? bodyA->invMass : 0.0f;
			float invMassB = bodyB ? bodyB->invMass : 0.0f;

			if (nVel >= 0.0f)
				continue;
			
			float e = 0.001f;
			float j = -(1.0f + e) * nVel / (invMassA + invMassB);

			Vector3 impulse = Vector3Scale(c.info.GetNormal(), j);

			if (bodyA)
				velA = Vector3Subtract(velA, Vector3Scale(impulse, invMassA));
			if (bodyB)
				velB = Vector3Add(velB, Vector3Scale(impulse, invMassB));
			
			rVel = Vector3Subtract(velB, velA);
			nVel = Vector3DotProduct(rVel, c.info.GetNormal());

			Vector3 tangent = Vector3Normalize(Vector3Subtract(rVel, Vector3Scale(c.info.GetNormal(), nVel)));
			float fVel = Vector3DotProduct(rVel, tangent);

			float mu = staticFriction;
			float f = -fVel / (invMassA + invMassB);

			Vector3 friction;
			if (fabsf(f) < j*mu)
				friction = Vector3Scale(tangent, f);
			else
			{
				mu = dynamicFriction;
				friction = Vector3Scale(tangent, -j * mu);
			}

			if (bodyA)
				bodyA->velocity = Vector3Subtract(velA, Vector3Scale(friction, invMassA));
			if (bodyB)
				bodyB->velocity = Vector3Subtract(velB, Vector3Scale(friction, invMassB));
 		}
	}

	void SKF_PositionSolver::Solve(std::vector<SKF_Collision>& collisions, float delta)
	{
		const float percent = 0.2f;
		const float slop = 0.01f;
		for (SKF_Collision c : collisions)
		{
			SKF_RigidBody* bodyA = c.bodA->isDynamic ? c.bodA : nullptr;
			SKF_RigidBody* bodyB = c.bodB->isDynamic ? c.bodB : nullptr;

			float invMassA = bodyA ? bodyA->invMass : 0.0f;
			float invMassB = bodyB ? bodyB->invMass : 0.0f;

			Vector3 correction = Vector3Scale(c.info.GetNormal(), fmaxf(c.info.GetDistance() - slop, 0.0f) / (invMassA + invMassB) * percent);
			if (bodyA)
				bodyA->collider->Transform(Vector3Scale(correction,-invMassA));
			if (bodyB)
				bodyB->collider->Transform(Vector3Scale(correction, invMassB));
		}
	}
	
	void SKF_PhysicsWorld::AddBody(SKF_RigidBody* body)
	{
		_bodies.push_back(body);
	}
	void SKF_PhysicsWorld::AddSolver(SKF_Solver* solver)
	{
		_solvers.push_back(solver);
	}
	void SKF_PhysicsWorld::Clear()
	{
		_bodies.clear();
		_solvers.clear();
	}

	void SKF_PhysicsWorld::ApplyGravity()
	{
		for (SKF_RigidBody* body : _bodies)
		{
			if (!body->isDynamic || !body->usesGravity)
				continue;
			body->force = Vector3Add(body->force, Vector3Scale(_gravity, body->mass));
		}
	}
	void SKF_PhysicsWorld::MoveBodies(float delta)
	{
		for (SKF_RigidBody* body : _bodies)
		{
			if (!body->isDynamic)
				continue;
			
			body->velocity = Vector3Add(body->velocity, Vector3Scale(body->force, body->invMass * delta));
			body->collider->Transform(Vector3Scale(body->velocity, delta));
			
			body->force = Vector3{ 0, 0, 0 };
		}
	}

	void SKF_PhysicsWorld::Run(float delta)
	{
		ApplyGravity();
		Resolve(delta);
		MoveBodies(delta);
	}

	void SKF_PhysicsWorld::Resolve(float delta)
	{
		std::vector<SKF_Collision> collsions;
		for (SKF_RigidBody* a : _bodies)
			for (SKF_RigidBody* b : _bodies)
		{
			if (a == b)
				break;
			
			if (!a->collider || !b->collider)
				continue;

			SKF_CollisionInfo info = a->collider->Test(b->collider);

			if (info.hit)
				collsions.emplace_back(a, b, info);
		}

		for (SKF_Solver* solver : _solvers)
		{
			solver->Solve(collsions, delta);
		}
	}
}