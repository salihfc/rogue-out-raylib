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

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1356;
static const int screenHeight = 900;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen();     // Change to screen, no transition effect

static void TransitionToScreen(); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void Input(Game*, float);          // Update and draw one frame
static void Update(Game*, float);          // Update and draw one frame
static void Draw(Game*, float);          // Update and draw one frame

// helpers
static const char* IntToConstChar(int);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice();      // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/ambient.ogg");
    fxCoin = LoadSound("resources/coin.wav");

    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    // Setup and init first screen
    Game game;
    InitGame(&game);


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second	
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		float delta = GetFrameTime();
        Input(&game, delta);
		Update(&game, delta);
        Draw(&game, delta);
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen()
{

}

// Request transition to next screen
static void TransitionToScreen()
{

}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{

}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

static Vector2 GetInputDirection()
{
    Vector2 dir;
    dir.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
    // dir.y = IsKeyDown(KEY_DOWN)  - IsKeyDown(KEY_UP); 
    return dir;
}

static void Input(Game* game, float delta)
{
    Vector2 move_dir = GetInputDirection();
    move_dir.x *= delta;
    MovePlayer(&(game->player), move_dir);
}


static void Update(Game* game, float delta)
{
	++game->frame;

    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music);       // NOTE: Music keeps playing between screens
    if (!onTransition)
    {
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

}

static void Draw(Game* game, float delta)
{
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
	{
        ClearBackground(RAYWHITE);
		DrawGame(game, delta);

        // Draw full screen rectangle in front of everything
        // if (onTransition) DrawTransition();

        DrawFPS(10, 10);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

