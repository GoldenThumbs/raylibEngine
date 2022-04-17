#include "graphics/camera.h"
#include "physics/physics_world.h"
#include "physics/collider_plane.h"
#include "physics/collider_sphere.h"
#include "physics/collider_box.h"
#include "physics/rigidbody.h"
#include <raymath.h>

using namespace Skiff;

#define MAX_LIGHTS 64

enum class SKF_LightType : u8
{
	SKF_LT_NONE = 0,
	SKF_LT_DIRECTIONAL,
	SKF_LT_POINT
};

struct SKF_Light
{
	Vector3 position;
	Vector3 direction;
	Color color;
	float radius;
	SKF_LightType type;

	i32 pLoc;
	i32 dLoc;
	i32 cLoc;
	i32 rLoc;
	i32 tLoc;
};

SKF_Light LoadLight(Shader shader, Vector3 position, Color color, float radius, SKF_LightType type, u32& amount)
{
	SKF_Light light = SKF_Light{ 0 };

	if (amount < MAX_LIGHTS)
	{
		light.position = position;
		light.direction = Vector3{ 0, 0, 1 };
		light.color = color;
		light.radius = radius;
		light.type = type;

		light.pLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", amount));
		light.dLoc = GetShaderLocation(shader, TextFormat("lights[%i].direction", amount));
		light.cLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", amount));
		light.rLoc = GetShaderLocation(shader, TextFormat("lights[%i].radius", amount));
		light.tLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", amount));

		amount++;
	}
	return light;
}

void UpdateLights(Shader shader, SKF_Light light)
{
	float p[3] = { light.position.x, light.position.y, light.position.z };
	SetShaderValue(shader, light.pLoc, p, SHADER_UNIFORM_VEC3);
	float d[3] = { light.direction.x, light.direction.y, light.direction.z };
	SetShaderValue(shader, light.dLoc, d, SHADER_UNIFORM_VEC3);
	float c[4] = {
		(float)light.color.r / 255.0f, (float)light.color.g / 255.0f,
		(float)light.color.b / 255.0f, (float)light.color.a / 255.0f};
	SetShaderValue(shader, light.cLoc, c, SHADER_UNIFORM_VEC4);
	SetShaderValue(shader, light.rLoc, &light.radius, SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, light.tLoc, &light.type, SHADER_UNIFORM_INT);
}

struct SKF_PlayerSettings
{
	float fov = 90.0f;
	float walkSpeed = 5.00f;
	float lookSpeed = 0.25f;
	float maxAngle = 90.0f;
	float minAngle =-80.0f;
	bool invertPitch = false;
	i32 keyForward = KEY_W;
	i32 keyBackward = KEY_S;
	i32 keyLeft = KEY_A;
	i32 keyRight = KEY_D;
};

class SKF_Player
{
private:
	Quaternion _pitch;
	Quaternion _yaw;
public:
	SKF_Camera camera;
	Vector3 position;
	double aspect;
	SKF_PlayerSettings settings;

	SKF_Player()
	{
		settings = SKF_PlayerSettings();
		position = Vector3{ 0, 0, 0 };

		_pitch = Quaternion{ 0, 0, 0, 1 };
		_yaw = Quaternion{ 0, 0, 0, 1 };

		aspect = (double)GetScreenWidth() / (double)GetScreenHeight();

		camera = SKF_Camera(settings.fov, 0.1f, 100.0f, false);
		camera.Calculate(aspect, position, GetRotation());
	}

	SKF_Player(SKF_PlayerSettings settings_, Vector3 position_, double aspect_, Vector2 initView)
	{
		settings = settings_;
		position = position_;
		aspect = aspect_;

		_pitch = QuaternionFromEuler(initView.x * DEG2RAD, 0, 0);
		_yaw = QuaternionFromEuler(0, initView.y * DEG2RAD, 0);

		camera = SKF_Camera(settings.fov, 0.1f, 100.0f, false);
		camera.Calculate(aspect, position, GetRotation());
	}

	void Run(float delta)
	{
		Vector2 mDelta = Vector2Scale(GetMouseDelta(), settings.lookSpeed);

		if (settings.invertPitch)
			mDelta.y *= -1.0f;

		_pitch = QuaternionMultiply(_pitch, QuaternionFromEuler(-mDelta.y * DEG2RAD, 0, 0));
		_yaw = QuaternionMultiply(_yaw, QuaternionFromEuler(0, -mDelta.x * DEG2RAD, 0));

		if (QuaternionToEuler(_pitch).x * RAD2DEG > settings.maxAngle)
			_pitch = QuaternionFromEuler(settings.maxAngle * DEG2RAD, 0, 0);
		else if (QuaternionToEuler(_pitch).x * RAD2DEG < settings.minAngle)
			_pitch = QuaternionFromEuler(settings.minAngle * DEG2RAD, 0, 0);
		
		Vector3 vel = Vector3{ 0, 0, 0 };
		if (IsKeyDown(settings.keyForward))
			vel.z -= 1.0f;
		if (IsKeyDown(settings.keyLeft))
			vel.x -= 1.0f;
		if (IsKeyDown(settings.keyBackward))
			vel.z += 1.0f;
		if (IsKeyDown(settings.keyRight))
			vel.x += 1.0f;
		vel = Vector3Scale(Vector3Normalize(vel), delta * settings.walkSpeed);
		position = Vector3Add(position, Vector3RotateByQuaternion(vel, _yaw));

		camera.Calculate(aspect, position, GetRotation());
	}

	Quaternion GetRotation()
	{
		return QuaternionNormalize(QuaternionMultiply(_yaw, _pitch));
	}
};

int main(void)
{
	const float dt = 0.01f;
	const i32 screenW = 1600;
	const i32 screenH = 900;
	const double aspect = (double)screenW / (double)screenH;
	InitWindow(screenW, screenH, "Raylib Window");

	DisableCursor();
	
	SKF_PhysicsWorld world = SKF_PhysicsWorld(Vector3{ 0, -9.81f, 0 });

	SKF_ImpulseSolver solver1 = SKF_ImpulseSolver();
	SKF_PositionSolver solver2 = SKF_PositionSolver();

	world.AddSolver(&solver1);
	world.AddSolver(&solver2);

	SKF_Plane plane = SKF_Plane(Vector3{ 0, 1, 0 }, -1.0f);
	SKF_RigidBody physicalPlane = SKF_RigidBody(&plane, 0.0f, false, false);

	world.AddBody(&physicalPlane);

	SKF_Player player = SKF_Player(SKF_PlayerSettings(), Vector3{ 0, 1.5f, 0 }, aspect, Vector2{ 0, 0 });

	SKF_BoundingSphere playerBall = SKF_BoundingSphere(player.position, 1.0f);
	SKF_RigidBody playerBody = SKF_RigidBody(&playerBall, 0.0, false, false);

	world.AddBody(&playerBody);

	Shader fill = LoadShader(ASSETS_PATH"shaders/defaultForward.vert", ASSETS_PATH"shaders/defaultForward.frag");

	Texture2D tex = LoadTexture(ASSETS_PATH"textures/Wood_1_albedo.png");
	GenTextureMipmaps(&tex);
	SetTextureFilter(tex, TEXTURE_FILTER_ANISOTROPIC_8X);

	Model object = LoadModel(ASSETS_PATH"models/Boot.obj");
	object.materials[0].shader = fill;

	Model viewObj = LoadModelFromMesh(GenMeshSphere(0.5f, 8, 16));
	viewObj.materials[0].shader = fill;

	Model ground = LoadModelFromMesh(GenMeshPlane(20.0f, 20.0f, 20, 20));
	ground.materials[0].shader = fill;
	ground.materials[0].maps[0].texture = tex;

	SKF_Light lightsArray[MAX_LIGHTS];
	SKF_BoundingSphere spheresArray[MAX_LIGHTS];
	SKF_RigidBody bodyArray[MAX_LIGHTS];
	u32 lightsAmount = 0;

	for (int i=0; i<MAX_LIGHTS; i++)
	{
		i32 _lX = (i32)GetRandomValue(-2, 2) * 3;
		i32 _lY = (i32)GetRandomValue( 0, 512);
		i32 _lZ = (i32)GetRandomValue(-2, 2) * 3;
		Vector3 position = Vector3{ (float)_lX, (float)_lY/256.0f, (float)_lZ };
		u8 _lR = (u8)GetRandomValue(64, 255);
		u8 _lG = (u8)GetRandomValue(64, 255);
		u8 _lB = (u8)GetRandomValue(64, 255);
		Color color = Color{ _lR, _lG, _lB, 255 };
		lightsArray[i] = LoadLight(fill, position, color, 4.0f, SKF_LightType::SKF_LT_POINT, lightsAmount);
		spheresArray[i] = SKF_BoundingSphere(position, 1.0f);
		bodyArray[i] = SKF_RigidBody(&spheresArray[i], 1.0f, true, true);
		world.AddBody(&bodyArray[i]);
	}

	lightsArray[0].color = DARKGRAY;
	lightsArray[0].type = SKF_LightType::SKF_LT_DIRECTIONAL;
	lightsArray[0].direction = Vector3RotateByQuaternion(Vector3{ 0, 0, 1 }, QuaternionFromEuler(-45.0f * DEG2RAD, 30.0f * DEG2RAD, 0));

	float accumulator = 0.0f;
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		float frameTime = GetFrameTime();

		if (IsKeyDown(KEY_SPACE))
		{
			for (i32 i=0; i<MAX_LIGHTS; i++)
			{
				float _rJX = (float)GetRandomValue(-128, 128) / 256.0f;
				float _rJY = (float)GetRandomValue(-128, 128) / 256.0f;
				float _rJZ = (float)GetRandomValue(-128, 128) / 256.0f;
				bodyArray[i].force = Vector3{ _rJX, 50.0f + _rJY, _rJZ };
			}
		}

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			world.Run(dt);
			accumulator -= dt;
		}

		player.Run(frameTime);
		playerBall.position = player.position;

		Vector3 extraPos = Vector3RotateByQuaternion(Vector3{ 1.0f, 0, -2.5f }, player.GetRotation());
		extraPos = Vector3Add(player.position, extraPos);

		BeginDrawing();
			ClearBackground(DARKGRAY);
			
			player.camera.Begin();
				for (i32 i=0; i<MAX_LIGHTS; i++)
				{
					lightsArray[i].position = spheresArray[i].GetCenter();

					UpdateLights(fill, lightsArray[i]);
					DrawSphere(lightsArray[i].position, spheresArray[i].radius, lightsArray[i].color);
				}
				DrawModel(object, Vector3{ 0, 0.5f, 0 }, 6.0f, WHITE);
				DrawModel(ground, Vector3{ 0, plane.GetCenter().y, 0 }, 1.0f, GRAY);
				DrawModel(viewObj, extraPos, 1.0f, YELLOW);
			player.camera.End();
			DrawText("Window", 16, 16, 20, DARKGRAY);
		EndDrawing();
	}

	world.Clear();

	UnloadShader(fill);
	UnloadTexture(tex);
	UnloadModel(object);
	UnloadModel(viewObj);
	UnloadModel(ground);

	CloseWindow();
	return 0;
}
