#include "raylib.h"
#include "raymath.h"
#include "fmt/core.h"

#define MAX_RAY_REFLECTIONS 8

struct Sphere {
	Vector3 position;
	float radius;
};

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

	Sphere spheres[] = {
		{ { 0, 1, 0 }, 1 },
		{ { 4, 1, 0 }, 1 },
		{ { 2, 2.5f, 0 }, 0.4f }
	};

	int numSpheres = std::size(spheres);

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

		Ray camera_ray = { camera.position, Vector3Normalize(Vector3Subtract(camera.target, camera.position)) };

		for (int current_reflections = 0; current_reflections < MAX_RAY_REFLECTIONS; ++current_reflections) {
			RayCollision closest_collision = { 0 };
			closest_collision.distance = FLT_MAX; // Use max float value to ensure any real collision will be closer
			int hitSphereIndex = -1;

			// Check collision with all spheres
			for (int i = 0; i < numSpheres; ++i) {
				RayCollision collision = GetRayCollisionSphere(camera_ray, spheres[i].position, spheres[i].radius);
				if (collision.hit && collision.distance < closest_collision.distance) {
					closest_collision = collision;
					hitSphereIndex = i;
				}
			}

			// If there's a collision, process it
			if (hitSphereIndex != -1) {
				DrawSphere(closest_collision.point, 0.1f, MAGENTA); // Visualize collision point
				DrawLine3D(camera_ray.position, closest_collision.point, GetColor(current_reflections * 0x111111FF)); // Visualize ray

				// Reflect the ray
				camera_ray.position = closest_collision.point;
				camera_ray.direction = Vector3Normalize(Vector3Reflect(camera_ray.direction, closest_collision.normal));
			}
			else {
				// No collision, break out of the loop
				break;
			}
		}

		DrawGrid(10, 1.0f);

		DrawSphere({ 0, 1, 0 }, 1, BLUE);
		DrawSphere({ 4, 1, 0 }, 1, YELLOW);
		DrawSphere({ 2, 2.5f, 0 }, 0.4f, LIME);

		//if (IsModelReady(bunny))
		//{
			//DrawModel(bunny, { 0, -0.2f, 0 }, 10.0f, WHITE);
		//}
		
		///////////////////////////
		EndMode3D();
		///////////////////////////

		/*************************/
		EndDrawing();
		/*************************/
	}

	//UnloadModel(bunny);
	CloseWindow();

	return 0;
}
