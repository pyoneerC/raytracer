#include "raylib.h"
#include "raymath.h"
#include "fmt/core.h"

int main() {
	
	constexpr int screen_width = 1000;
	constexpr int screen_height = 500;

	InitWindow(screen_width, screen_height, "RayLib");

	SetTargetFPS(60);

	Camera3D camera; 
	camera.position = { 4.0f, 2.0f, 4.0f };
	camera.target = {};
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.projection = CAMERA_PERSPECTIVE;
	camera.fovy = 70.0f;                   

	//const Model bunny = LoadModel("bunny.obj");

	SetMousePosition(screen_width / 2, screen_height / 2);
	DisableCursor(); 

	while (!WindowShouldClose()) 
	{

		UpdateCamera(&camera, CAMERA_FREE);

		DrawText("Press Enter to reset camera position", 10, screen_height - 20, 20, BLACK);
		DrawFPS(10, 10);

		//return to default position
		if (IsKeyPressed(KEY_ENTER)) 
		{
			camera.position = { 4.0f, 2.0f, 4.0f };
			camera.target = { 0.0f, 1.5f, 0.0f };
			camera.up = { 0.0f, 1.0f, 0.0f };
		}

		/*************************/
		BeginDrawing();
		/*************************/

		ClearBackground(GRAY);

		///////////////////////////
		BeginMode3D(camera);
		///////////////////////////

		Ray camera_ray = { camera.position, Vector3Subtract(camera.target, camera.position) };
		camera_ray.direction = Vector3Normalize(camera_ray.direction);

		constexpr Vector3 sphere_position = { 0, 1, 0 };
		constexpr float sphere_radius = 1.0f;

		const RayCollision sphere_collision = GetRayCollisionSphere(camera_ray, sphere_position, sphere_radius);

		const bool hit = sphere_collision.hit && sphere_collision.distance < 10;

		if (hit) 
		{

			DrawSphere(sphere_collision.point, 0.1f, MAGENTA);
			DrawLine3D(Vector3Add(camera.position, {0.0f,0.1f,0.0f}), sphere_collision.point, RED);

			const Vector3 vec_direction = Vector3Subtract(sphere_collision.point, camera.position);

			Vector3 reflected_direction = Vector3Reflect(vec_direction, sphere_collision.normal);
  
			reflected_direction = Vector3Normalize(reflected_direction);

			const Vector3 reflected_end_point = Vector3Add(sphere_collision.point, Vector3Scale(reflected_direction, 5));

			DrawLine3D(sphere_collision.point, reflected_end_point, MAGENTA);
		}

		DrawGrid(10, 1.0f);

		DrawSphere({0, 1, 0}, 1, BLUE);

		//if (IsModelReady(bunny))
		//{
			//DrawModel(bunny, { 0, -0.2f, 0 }, 10.0f, WHITE);
		//}
		
		///////////////////////////
		EndMode3D();
		///////////////////////////

		if (hit) 
		{
			DrawText("Hit!", 10, screen_height - 40, 20, RED);
		}

		/*************************/
		EndDrawing();
		/*************************/
	}

	//UnloadModel(bunny);
	CloseWindow();

	return 0;
}
