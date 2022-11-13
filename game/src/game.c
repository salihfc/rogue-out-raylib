#include "utils.c"
#include "player.c"
#include "board.c"
#include "ball.c"
#include "geo_utils.c"
#include "particle_generator.c"

#define BALL_COUNT 5

typedef 
struct Game
{
	int frame;
	Player player;
	Board board;
	Ball* balls;

	ParticleGenerator particle_generator;

} Game;

static
void HandleCollisionBall(Game* game, Ball* ball, float delta);

static
void HandleCollisions(Game* game, float delta)
{
	Player* player = &game->player;

	{
		Ball* ball = game->balls;
		for (int i = 0; i < BALL_COUNT; i++)
		{
			HandleCollisionBall(game, ball, delta);
			ball++;
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
			AddParticleGroup(&game->particle_generator, 
				(Particle) {
					.type = SQUARE,

					.position = ball->position,
					.size = 3,
					.color = BLUE,
					.lifetime = 2.0,
					.damping = 0.7
				},
				10
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

static
void InitGame(Game* game)
{
	game->frame = 0;
	ResetPlayerPosition(&(game->player));
	InitBoard(&(game->board), (Vector2) {200, 100});
	InitParticleGenerator(&game->particle_generator, PARTICLE_GENERATOR_STARTING_CAPACITY);

	game->balls = MemAlloc(BALL_COUNT * sizeof(Ball));
	Ball* ball = game->balls;

	for (int i = 0; i < BALL_COUNT; i++)
	{
		InitBall(ball, (Vector2) {200 + 50.0 * i, 100}, (Vector2) {300, 120}, 5.0);
		ball++;
	}
}


static
void UpdateGame(Game* game, float delta)
{
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
}