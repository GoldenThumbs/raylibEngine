#include "camera.h"
#include <raymath.h>
#include <rlgl.h>

namespace Skiff
{
	SKF_Camera::SKF_Camera()
	{
		fov = 90.0f;
		nearClip = 0.1f;
		farClip = 100.0f;
		ortho = false;
	}

	SKF_Camera::SKF_Camera(float fov_, float nearClip_, float farClip_, bool ortho_)
	{
		fov = fov_;
		nearClip = nearClip_;
		farClip = farClip_;
		ortho = ortho_;
	}

	Matrix& SKF_Camera::ProjectionMatrix()
	{
		return _proj;
	}

	Matrix& SKF_Camera::ViewMatrix()
	{
		return _view;
	}

	void SKF_Camera::Calculate(double aspect, Vector3 position, Quaternion rotation)
	{
		if (ortho)
		{
			double top = (double)fov * 0.5;
			double right = top * aspect;
			_proj = MatrixOrtho(-right, right, -top, top, (double)nearClip, (double)farClip);
		} else
			_proj = MatrixPerspective((double)fov * DEG2RAD, aspect, (double)nearClip, (double)farClip);

		Matrix p = MatrixTranslate(position.x, position.y, position.z);
		Matrix r = QuaternionToMatrix(rotation);
		_view = MatrixInvert(MatrixMultiply(r, p));
	}

	void SKF_Camera::Begin()
	{
		rlDrawRenderBatchActive();

		rlMatrixMode(RL_PROJECTION);
		rlPushMatrix();
		rlLoadIdentity();

		rlMultMatrixf(MatrixToFloat(_proj));

		rlMatrixMode(RL_MODELVIEW);
		rlLoadIdentity();

		rlMultMatrixf(MatrixToFloat(_view));

		rlEnableDepthTest();
	}

	void SKF_Camera::End()
	{
		EndMode3D();
	}
}
