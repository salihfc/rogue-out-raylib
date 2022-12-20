#include "player.c"
#include "board.c"
#include "ball.c"
#include "utils/geo_utils.c"

#include "particles/particle_manager.c"
#include "sound.c"
#include "camera.c"
#include "shader_loader.c"
#include "light/light_manager.c"
#include "sdf/sdf.c"
#include "background.c"

#define DRAW_GAME true
#define DRAW_SDF true

#define BALL_COUNT 5
#define BALL_SIZE 5

static const char *BACKGROUND_TEXTURE_PATH = "game/resources/textures/64x64_white.png";
static const char *PLAYER_TEXTURE_PATH = "game/resources/textures/64x64_white.png";
static const Color BACKGROUND_COLOR_TINT = (Color){25, 25, 50, 200};

typedef struct
{
	int frame;
	Player player;
	Board board;
	Ball *balls;

	ParticleManager particle_manager;
	SoundManager sound_manager;
	CameraManager camera_manager;
	ShaderLoader shader_loader;
	LightManager light_manager;
	Background background;
	SDFObject sdf;

} Game;

static void HandleCollisionBall(Game *game, Ball *ball, float delta);
static void HandleCollisions(Game *game, float delta);

static void InitGame(Game *game)
{
	SetTraceLogLevel(LOG_ERROR);

	game->frame = 0;
	InitPlayer(&game->player);
	ResetPlayerPosition(&(game->player));
	LoadPlayerTexture(&game->player, PLAYER_TEXTURE_PATH);
	InitBoard(&(game->board), (Vector2){200, 100});
	InitParticleManager(&game->particle_manager, PARTICLE_MANAGER_STARTING_CAPACITY);
	InitSoundManager(&game->sound_manager);
	InitShaderLoader(&game->shader_loader);
	InitLightManager(&game->light_manager);

	//
	InitSDF(&game->sdf);
	//

	game->background = (Background){
			.size = (Vector2){GetScreenWidth(), GetScreenHeight()},
			.texture = LoadTexture(BACKGROUND_TEXTURE_PATH)};
	InitBackground(&game->background);

	SetBackgroundTint(&game->background, BACKGROUND_COLOR_TINT);

	{
		float screenWidth = 1.0f * GetScreenWidth();
		float screenHeight = 1.0f * GetScreenHeight();

		Camera2D camera = {0};
		camera.target = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
		camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;

		game->camera_manager.camera = camera;
	}

	game->balls = MemAlloc(BALL_COUNT * sizeof(Ball));
	Ball *ball = game->balls;

	for (int i = 0; i < BALL_COUNT; i++)
	{
		ball->light = GetNewLight(&game->light_manager);
		InitBall(ball, (Vector2){200 + 50.0 * i, 100}, GetRandomVector2(500.0), BALL_SIZE);
		ball++;
	}
}

static void UpdateGame(Game *game, float delta)
{
	TickCameraManager(&game->camera_manager, delta);
	TickPlayer(&(game->player), delta);
	TickParticleManager(&game->particle_manager, delta);

	Ball *ball = game->balls;
	for (int i = 0; i < BALL_COUNT; i++)
	{
		TickBall(ball, delta);
		ball++;
	}

	for (int row = 0; row < BOARD_ROW; row++)
	{
		for (int col = 0; col < BOARD_COL; col++)
		{
			SetShaderLights(&game->light_manager, (game->board.bricks[row][col]).shader_loader.shader);
		}
	}

	SetShaderLights(&game->light_manager, game->background.shader_loader.shader);

	HandleCollisions(game, delta);
}

static void DrawGame(Game *game, float delta)
{
	// BeginBlendMode(BLEND_ALPHA);
	BeginBlendMode(BLEND_ADDITIVE);
	if (DRAW_GAME)
	{
		DrawBackground(&game->background);
		BeginMode2D(GetModifiedCamera(&game->camera_manager));

		{
			// DRAW PLAYER
			Player *player = &(game->player);
			DrawPlayer(player);

			// DRAW BOARD
			Board *board = &(game->board);
			DrawBoard(board);

			// DRAW BALL
			Ball *ball = game->balls;
			for (int i = 0; i < BALL_COUNT; i++)
			{
				DrawBall(ball);

				ball++;
			}

			// DRAW PARTICLES
			DrawParticleManager(&game->particle_manager);
			EndMode2D();
		}
		// EndBlendMode();
	}

	if (DRAW_SDF)
	{
		// BeginBlendMode(BLEND_ADDITIVE);
		{
			BeginMode2D(GetModifiedCamera(&game->camera_manager));
			{
				DrawSDF(&game->sdf);
			}
			EndMode2D();
		}
		// EndBlendMode();
	}
	EndBlendMode();
}

static void HandleCollisions(Game *game, float delta)
{
	Player *player = &game->player;

	{
		// Ball collisions
		Ball *ball = game->balls;
		for (int i = 0; i < BALL_COUNT; i++)
		{
			HandleCollisionBall(game, ball, delta);
			ball++;
		}
	}

	{
		// Particle collisions
		Particle *particles = game->particle_manager.particles;
		int *active_particles = game->particle_manager.active_particles;
		int particle_count = 0;

		while (particle_count < game->particle_manager.particle_count)
		{
			Particle *particle = particles + active_particles[particle_count];

			//
			Brick *it = (Brick *)game->board.bricks;
			int brick_ct = 0;

			while (brick_ct++ < BOARD_ROW * BOARD_COL)
			{
				Brick *brick = it++;
				if (IsBrickDestroyed(brick))
					continue;

				bool particle_collided_with_brick = CheckCollisionCircleRec(
						particle->position, particle->size,
						ContractRectangle(GetBrickRect(brick), 4, 2));

				if (particle_collided_with_brick)
				{
					// particle->color.a = 100.0;

					if (IsParticleImmune(particle) == false)
					{
						particle->life = 0;
						particle->life_t = 0;
					}

					// MarkParticleFree(&game->particle_manager, active_particles[particle_count]);
					break;
				}
			}

			particle_count++;
		}
	}
}

static void HandleCollisionBall(Game *game, Ball *ball, float delta)
{
	Player *player = &game->player;
	bool ball_collided_with_player = CheckCollisionCircleRec(ball->position, ball->radius, GetPlayerRect(player));
	player->tint = (Color[]){GREEN, WHITE}[ball_collided_with_player];

	if (ball_collided_with_player && ball->velocity.y > 0)
	{
		ball->velocity.y *= -1;
		ball->position = VectorSum(ball->position, VectorScaled(ball->velocity, delta));

		CameraAddTrauma(&game->camera_manager, 0.4);

		printf("[PLAYER HIT BALL]\n");
	}

	Brick *it = (Brick *)(game->board.bricks);

	int brick_ct = 0;

	while (brick_ct++ < BOARD_ROW * BOARD_COL)
	{
		Brick *brick = it++;

		if (IsBrickDestroyed(brick))
		{
			continue;
		}

		bool ball_collided_with_brick = CheckCollisionCircleRec(ball->position, ball->radius, GetBrickRect(brick));

		if (ball_collided_with_brick)
		{
			Vector2 direction = VectorDirectionTo(GetBrickCenter(brick), ball->position);

			CameraAddTrauma(&game->camera_manager, 0.05);

			PlaySFXRandomize(&game->sound_manager, BALL_BRICK_COLLISION);

			AddParticleGroup(&game->particle_manager,
											 (Particle){
													 .type = CIRCLE,

													 .position = ball->position,
													 // .position = VectorDif(ball->position, VectorScaled(direction, 5.0)),
													 .base_size = 3,
													 .color = WHITE,
													 .lifetime = 2.0,
													 .damping = 0.7,
													 .size_easing = EASE_CIRCLE4,
											 },
											 10,
											 direction);

			if (!InRange(ball->position.x, brick->position.x, brick->position.x + BRICK_WIDTH) || InRange(ball->position.y, brick->position.y, brick->position.y + BRICK_HEIGHT))
			{
				ball->velocity.x *= -1;
			}
			else
				ball->velocity.y *= -1;

			ball->position = VectorSum(ball->position, VectorScaled(ball->velocity, delta));

			DealDamageToBrick(brick, 1);

			if (IsBrickDestroyed(brick))
			{
				AddParticleGroup(&game->particle_manager,
												 (Particle){
														 .type = SQUARE,

														 .position = brick->position,
														 .base_size = 10,
														 .color = RED,
														 .lifetime = 2.0,
														 .damping = 0.7,
														 .immunity_duration = 1.0,
														 .size_easing = EASE_CIRCLE4,

												 },
												 4,
												 direction);

				AddSpeedToBall(ball, 10);
			}

			break;
		}
	}
}
