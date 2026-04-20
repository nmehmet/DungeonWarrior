#include "raylib.h"

int createWindiwExample() {
	//Create a window
	InitWindow(800, 600, "Hello Raylib!");

	//Set Fps
	SetTargetFPS(60);

	//Main game loop

	while (!WindowShouldClose()) {
		//Start Drawing;
		BeginDrawing();

		//Clear the background
		ClearBackground(RAYWHITE);

		//Draw some text
		DrawText("Congrats! You created your first window!", 190, 200, 20, DARKGRAY);

		//End Drawing
		EndDrawing();
	}
	//Close window and OpenGL context
	CloseWindow();

	return 0;
}