/*******************************************************************************************
 *
 *   raylib game template
 *
 *   <Game title>
 *   <Game description>
 *
 *   This game has been created using raylib (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2021 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <stdio.h>
#include "raylib.h"
#include "game.c"
#include "input.c"
#include "draw_utils.c"

#define DELTA (0.0166666666f)

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
Font font = {0};
Music music = {0};
Sound fxCoin = {0};

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1356;
static const int screenHeight = 900;

// Required variables to manage screen transitions (fade-in, fade-out)
//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void Input(Game *, Camera2D *, float);	 // Update and draw one frame
static void Update(Game *, Camera2D *, float); // Update and draw one frame
static void Draw(Game *, Camera2D *, float);	 // Update and draw one frame

// helpers

// Setup camera
Camera2D camera = {0};

// Setup and init first screen
Game game;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//---------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "raylib game template");

	InitAudioDevice(); // Initialize audio device

	// Load global data (assets that must be available in all screens, i.e. font)
	font = LoadFont("resources/mecha.png");
	music = LoadMusicStream("resources/ambient.ogg");
	fxCoin = LoadSound("resources/coin.wav");

	SetMusicVolume(music, 0.0f);
	PlayMusicStream(music);

	SetTraceLogLevel(LOG_INFO);
	camera.target = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
	camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	InitGame(&game);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		float delta = GetFrameTime();
		Input(&game, &camera, delta);
		Update(&game, &camera, delta);
		Draw(&game, &camera, delta);
	}
#endif

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload current screen data before closing

	// Unload global data loaded
	UnloadFont(font);
	UnloadMusicStream(music);
	UnloadSound(fxCoin);

	CloseAudioDevice(); // Close audio context

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
static Vector2 GetInputDirection()
{
	Vector2 dir;
	dir.x = IsMoveRightDown() - IsMoveLeftDown();
	// dir.y = IsKeyDown(KEY_DOWN)  - IsKeyDown(KEY_UP);
	return dir;
}

static void Input(Game *game, Camera2D *camera, float delta)
{
	Player *player = &(game->player);
	Vector2 force = GetInputDirection();
	force.x *= delta;
	force.y *= delta;

	ApplyForceToPlayer(player, force);

	// camera->target = VectorSum(camera->target, VectorScaled(force, 10.0));

	if (IsKeyPressed(KEY_E))
	{
		Vector2 mouse = GetMousePosition();
		printf("%d %d\n", (int)mouse.x, (int)mouse.y);
		printf("%s %s\n", IntToConstChar((int)mouse.x), IntToConstChar((int)mouse.y));
	}
}

static void Update(Game *game, Camera2D *camera, float delta)
{
	++game->frame;
	UpdateGame(game, delta);

	// Update
	//----------------------------------------------------------------------------------
	UpdateMusicStream(music); // NOTE: Music keeps playing between screens
														//----------------------------------------------------------------------------------
}

static void Draw(Game *game, Camera2D *camera, float delta)
{
	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();
	ClearBackground(BLACK);

	{
		DrawGame(game, delta);
	}

	BeginMode2D(*camera);
	{
		DrawCircleV(
				(Vector2){0, 0},
				// (Vector2) {screenWidth / 2, screenHeight / 2},
				100.0,
				BLACK);
	}
	EndMode2D();

	// DRAW DEBUG
	DrawFPS(10, 10);
	DrawInt(game->frame / 60, (Vector2){10, 50}, 32, RED);

	Vector2 mouse = GetMousePosition();
	DrawVector2(mouse, (Vector2){10, 140}, 32, BLUE);

	// Player Debug
	Player *player = &game->player;
	DrawVector2(player->body.position, (Vector2){10, 80}, 32, DARKGREEN);
	DrawVector2(player->body.size, (Vector2){10, 110}, 32, DARKGREEN);

	EndDrawing();
	//----------------------------------------------------------------------------------
}

static void UpdateDrawFrame()
{
	Input(&game, &camera, DELTA);
	Update(&game, &camera, DELTA);
	Draw(&game, &camera, DELTA);
}