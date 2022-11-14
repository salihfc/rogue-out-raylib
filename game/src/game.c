#include "utils.c"
#include "player.c"
#include "board.c"
#include "ball.c"
#include "geo_utils.c"

#include "particle_generator.c"
#include "sound.c"
#include "camera.c"

#define BALL_COUNT 5
#define BALL_SIZE 5

typedef 
struct Game
{
	int frame;
	Player player;
	Board board;
	Ball* balls;

	ParticleGenerator particle_generator;
	SoundManager sound_manager;
	CameraManager camera_manager;

} Game;

static
void HandleCollisionBall(Game* game, Ball* ball, float delta);
static
void HandleCollisions(Game* game, float delta);


static
void InitGame(Game* game)
{
	game->frame = 0;
	ResetPlayerPosition(&(game->player));
	InitBoard(&(game->board), (Vector2) {200, 100});
	InitParticleGenerator(&game->particle_generator, PARTICLE_GENERATOR_STARTING_CAPACITY);
	InitSoundManager(&game->sound_manager);
	{
		float screenWidth = 1.0f * GetScreenWidth();
		float screenHeight = 1.0f * GetScreenHeight();

		Camera2D camera = { 0 };
		camera.target = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
		camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;

		game->camera_manager.camera = camera;
	}


	game->balls = MemAlloc(BALL_COUNT * sizeof(Ball));
	Ball* ball = game->balls;

	for (int i = 0; i < BALL_COUNT; i++)
	{
		InitBall(ball, (Vector2) {200 + 50.0 * i, 100}, (Vector2) {300, 120}, BALL_SIZE);
		ball++;
	}
}


static
void UpdateGame(Game* game, float delta)
{
	TickCameraManager(&game->camera_manager, delta);
	MovePlayer(&(game->player), delta);
	TickParticleGenerator(&game->particle_generator, delta);

	Ball* ball = game->balls;
	for (int i = 0; i < BALL_COUNT; i++)
	{
		MoveBall(ball, delta);
		ball++;
	}

	HandleCollisions(game, delta);
}


static
void DrawGame(Game* game, float delta)
{
	BeginMode2D(GetModifiedCamera(&game->camera_manager));

	// DRAW PLAYER
	Player* player = &(game->player);
	DrawPlayer(player);

	// DRAW BOARD
	Board* board = &(game->board);
	DrawBoard(board);

	// DRAW BALL
	Ball* ball = game->balls;
	for (int i = 0; i < BALL_COUNT; i++)
	{
		DrawBall(ball);

		ball++;
	}

	// DRAW PARTICLES
	DrawParticleGenerator(&game->particle_generator);

	EndMode2D();
}



static
void HandleCollisions(Game* game, float delta)
{
	Player* player = &game->player;

	{
		// Ball collisions
		Ball* ball = game->balls;
		for (int i = 0; i < BALL_COUNT; i++)
		{
			HandleCollisionBall(game, ball, delta);
			ball++;
		}
	}

	{
		// Particle collisions
		Particle* particles = game->particle_generator.particles;
		int* active_particles = game->particle_generator.active_particles;
		int particle_count = 0;

		while (particle_count < game->particle_generator.particle_count)
		{
			Particle* particle = particles + active_particles[particle_count];

			// 
			Brick* it = &game->board.bricks; 
			int brick_ct = 0;

			while (brick_ct++ < BOARD_ROW * BOARD_COL)
			{
				Brick* brick = it++;
				if (IsBrickDestroyed(brick))
					continue;

				bool particle_collided_with_brick = CheckCollisionCircleRec(
						particle->position, particle->size, 
						ContractRectangle(GetBrickRect(brick), 4, 2)
				);

				
			
				if (particle_collided_with_brick)
				{
					// particle->color.a = 100.0;

					if (IsParticleImmune(particle) == false)
					{
						particle->life = 0;
						particle->life_t = 0;
					}

					// MarkParticleFree(&game->particle_generator, active_particles[particle_count]);
					break;
				}
			}

			particle_count++;
		}
	}
}


static
void HandleCollisionBall(Game* game, Ball* ball, float delta)
{
	Player* player = &game->player;
	bool ball_collided_with_player = CheckCollisionCircleRec(ball->position, ball->radius, GetPlayerRect(player));
	player->tint = (Color[]) {GREEN, WHITE} [ball_collided_with_player];

	if (ball_collided_with_player && ball->velocity.y > 0)
	{
		ball->velocity.y *= -1;
		ball->position = VectorSum(ball->position, VectorScaled(ball->velocity, delta));
	}

	Brick* it = &(game->board.bricks);

	int brick_ct = 0;

	while (brick_ct++ < BOARD_ROW * BOARD_COL)
	{
		Brick* brick = it++;

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

			AddParticleGroup(&game->particle_generator, 
				(Particle) {
					.type = CIRCLE,

					.position = ball->position,
					// .position = VectorDif(ball->position, VectorScaled(direction, 5.0)),
					.size = 3,
					.color = BLUE,
					.lifetime = 2.0,
					.damping = 0.7
				},
				10,
				direction
			);

			if (!InRange(ball->position.x, brick->position.x, brick->position.x + BRICK_WIDTH)
				|| InRange(ball->position.y, brick->position.y, brick->position.y + BRICK_HEIGHT))
			{
				ball->velocity.x *= -1;
			}
			else
				ball->velocity.y *= -1;
			
			ball->position = VectorSum(ball->position, VectorScaled(ball->velocity, delta));

			brick->remainingHp -= 1;
			break;
		}
	}
}
