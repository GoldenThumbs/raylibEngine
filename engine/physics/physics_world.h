#pragma once

#include "../skiff.h"
#include "physics.h"
#include <raylib.h>
#include <vector>

namespace Skiff
{
	struct SKF_RigidBody;

	struct SKF_Collision
	{
		SKF_RigidBody* bodA;
		SKF_RigidBody* bodB;
		SKF_CollisionInfo info;

		SKF_Collision(SKF_RigidBody* a, SKF_RigidBody* b, SKF_CollisionInfo i) :
			bodA(a),
			bodB(b),
			info(i) {}
	};

	struct SKF_Solver 
	{
		virtual void Solve(std::vector<SKF_Collision>& collisions, float delta) = 0;
	};

	struct SKF_ImpulseSolver : SKF_Solver
	{
		void Solve(std::vector<SKF_Collision>& collisions, float delta) override;
	};

	struct SKF_PositionSolver : SKF_Solver
	{
		void Solve(std::vector<SKF_Collision>& collisions, float delta) override;
	};
	
	class SKF_PhysicsWorld
	{
	private:
		std::vector<SKF_RigidBody*> _bodies;
		std::vector<SKF_Solver*> _solvers;
		Vector3 _gravity;
	public:
		SKF_PhysicsWorld(Vector3 gravity) :
			_gravity(gravity) {}
		
		void AddBody(SKF_RigidBody* body);
		void AddSolver(SKF_Solver* solver);
		void Clear();

		void ApplyGravity();
		void MoveBodies(float delta);
		void Run(float delta);
		void Resolve(float delta);
	};
}