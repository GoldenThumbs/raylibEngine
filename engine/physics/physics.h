#pragma once

#include "../skiff.h"
#include <raylib.h>
#include <vector>

// Roughly based on this video: https://www.youtube.com/watch?v=-_IspRG548E&t=317s

namespace Skiff
{
	struct SKF_Plane;
	struct SKF_BoundingSphere;
	struct SKF_BoundingBox;
	
	struct SKF_CollisionInfo
	{
		bool hit;
		Vector3 normal;

		SKF_CollisionInfo(bool h, Vector3 n) :
			hit(h),
			normal(n) {}
		
		float GetDistance();
		Vector3 GetNormal();
	};
	
	SKF_CollisionInfo CheckBoxes(SKF_BoundingBox a, SKF_BoundingBox b);
	
	SKF_CollisionInfo CheckSpheres(SKF_BoundingSphere a, SKF_BoundingSphere b);
	
	SKF_CollisionInfo CheckSphereAndBox(SKF_BoundingSphere a, SKF_BoundingBox b);
	
	SKF_CollisionInfo CheckSphereAndPlane(SKF_BoundingSphere a, SKF_Plane b);
	
	SKF_CollisionInfo CheckBoxAndPlane(SKF_BoundingBox a, SKF_Plane b);
}
