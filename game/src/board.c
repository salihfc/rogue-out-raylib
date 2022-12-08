#include "raylib.h"
#include "brick.c"

#define BOARD_ROW 8
#define BOARD_COL 12

#define PAD_ROW 10
#define PAD_COL 10

// static const int BOARD_ROW = 4;
// static const int BOARD_COL = 4;

typedef struct Board
{
	Brick bricks[BOARD_ROW][BOARD_COL];

} Board;

static void InitBoard(Board *board, Vector2 board_top_left)
{
	for (int row = 0; row < BOARD_ROW; row++)
	{
		for (int col = 0; col < BOARD_COL; col++)
		{
			Brick *brick = &(board->bricks[row][col]);

			brick->remainingHp = 3;
			brick->position = VectorSum(
					board_top_left,
					(Vector2){
							col * BRICK_WIDTH + col * PAD_COL,
							row * BRICK_HEIGHT + row * PAD_ROW});
			InitBrick(brick);
		}
	}
}

static void DrawBoard(Board *board)
{

	for (int row = 0; row < BOARD_ROW; row++)
	{
		for (int col = 0; col < BOARD_COL; col++)
		{
			DrawBrick(&(board->bricks[row][col]));
		}
	}
}