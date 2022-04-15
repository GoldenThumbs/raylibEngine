#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "skiff.h"

using namespace Skiff;

class SKF_GBuffer
{
private:
	u32 _width;
	u32 _height;
public:
	u32 id;
	Texture2D colorTex; // Color
	Texture2D specTex; // Specular Information (Spec Color, Gloss)
	Texture2D normTex; // Viewspace Normal
	Texture2D depthTex; // Hardware Depth

	SKF_GBuffer(u32 width, u32 height)
	{
		_width = width;
		_height = height;

		id = rlLoadFramebuffer(_width, _height);

		if (id > 0)
		{
			rlEnableFramebuffer(id);

			colorTex.id = rlLoadTexture(NULL, _width, _height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			colorTex.width = _width;
			colorTex.height = _height;
			colorTex.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			colorTex.mipmaps = 1;

			specTex.id = rlLoadTexture(NULL, _width, _height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			specTex.width = _width;
			specTex.height = _height;
			specTex.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			specTex.mipmaps = 1;

			normTex.id = rlLoadTexture(NULL, _width, _height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			normTex.width = _width;
			normTex.height = _height;
			normTex.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			normTex.mipmaps = 1;

			depthTex.id = rlLoadTextureDepth(_width, _height, false);
			depthTex.width = _width;
			depthTex.height = _height;
			depthTex.format = 19;
			depthTex.mipmaps = 1;

			rlFramebufferAttach(id, colorTex.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, specTex.id, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, normTex.id, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
			rlFramebufferAttach(id, depthTex.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

			rlActiveDrawBuffers(3);

			if (rlFramebufferComplete(id))
				TraceLog(LOG_INFO, "GBUFFER: [ID %i] Creation succeeded", id);
			else
				TraceLog(LOG_WARNING, "GBUFFER: [ID %i] Creation Failed", id);

			rlDisableFramebuffer();
		} else
			TraceLog(LOG_WARNING, "GBUFFER: Creation Failed, no id assigned");
	}

	~SKF_GBuffer()
	{
		Unload();
	}

	void Begin()
	{
		rlDrawRenderBatchActive();
		rlEnableFramebuffer(id);

		rlViewport(0, 0, _width, _height);

		rlMatrixMode(RL_PROJECTION);
		rlLoadIdentity();

		rlOrtho(0.0, (double)_width, (double)_height, 0.0, 0.0, 1.0);

		rlMatrixMode(RL_MODELVIEW);
		rlLoadIdentity();

		rlClearColor(0, 0, 0, 255);
		rlClearScreenBuffers();

		rlDisableColorBlend();
	}

	void End(u32 width, u32 height)
	{
		rlDrawRenderBatchActive();
		rlDisableFramebuffer();

		rlViewport(0, 0, width, height);

		rlMatrixMode(RL_PROJECTION);
		rlLoadIdentity();

		rlOrtho(0.0, (double)width, (double)height, 0.0, 0.0, 1.0);

		rlMatrixMode(RL_MODELVIEW);
		rlLoadIdentity();

		rlEnableColorBlend();
	}

	void Unload()
	{
		rlUnloadTexture(colorTex.id);
		rlUnloadTexture(specTex.id);
		rlUnloadTexture(normTex.id);
		rlUnloadFramebuffer(id);
	}
};

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

	SKF_Camera(float fov_, float nearClip_, float farClip_, bool ortho_)
	{
		fov = fov_;
		nearClip = nearClip_;
		farClip = farClip_;
		ortho = ortho_;
	}

	Matrix& ProjectionMatrix()
	{
		return _proj;
	}

	Matrix& ViewMatrix()
	{
		return _view;
	}

	void Calculate(double aspect, Vector3 position, Quaternion rotation)
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

	void Begin()
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

	void End()
	{
		EndMode3D();
	}
};

int main(void)
{
	const double aspect = 800.0 / 600.0;
	InitWindow(800, 600, "Raylib Window");

	Shader fill = LoadShader(ASSETS_PATH"shaders/default.vert", ASSETS_PATH"shaders/default.frag");

	SKF_Camera cam = SKF_Camera(90.0f, 0.1f, 100.0f, false);
	SKF_GBuffer buf = SKF_GBuffer(800, 600);

	Model object = LoadModel(ASSETS_PATH"models/tstmdl.obj");
	object.materials[0].shader = fill;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		
		cam.Calculate(aspect, Vector3{ 5.0f, 0.5f, 5.0f }, QuaternionFromEuler(0.0f * DEG2RAD, 45.0f * DEG2RAD, 0.0f * DEG2RAD));

		buf.Begin();
			cam.Begin();
				DrawModel(object, Vector3{ 0, 0, 0 }, 1.0f, WHITE);
			cam.End();
		buf.End((u32)GetScreenWidth(), (u32)GetScreenHeight());

		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			DrawTexture(buf.normTex, 0, 0, WHITE);
			DrawText("Window", 16, 16, 20, DARKGRAY);
		EndDrawing();
	}

	UnloadShader(fill);
	buf.Unload();
	UnloadModel(object);

	CloseWindow();
	return 0;
}
