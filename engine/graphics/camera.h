#pragma once

#include "../skiff.h"
#include <raylib.h>

namespace Skiff
{
	class SKF_Camera
	{
	private:
		Matrix _proj;
		Matrix _view;
	public:
		float fov;
		float nearClip;
		float farClip;
		bool ortho;

		SKF_Camera();
		SKF_Camera(float fov_, float nearClip_, float farClip_, bool ortho_);

		Matrix& ProjectionMatrix();

		Matrix& ViewMatrix();

		void Calculate(double aspect, Vector3 position, Quaternion rotation);

		void Begin();

		void End();
	};
}