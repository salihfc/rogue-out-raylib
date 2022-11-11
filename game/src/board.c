#include "raylib.h"
#include "brick.c"
#include "utils.c"

#define BOARD_ROW 6
#define BOARD_COL 6

#define PAD_ROW 20
#define PAD_COL 40

// static const int BOARD_ROW = 4;
// static const int BOARD_COL = 4;

typedef
struct Board
{
	Brick bricks[BOARD_ROW][BOARD_COL];

} Board;


static
void InitBoard(Board* board, Vector2 board_top_left)
{
	for (int row = 0; row < BOARD_ROW; row++)
	{
		for (int col = 0; col < BOARD_COL; col++)
		{
			Brick* brick = &(board->bricks[row][col]);

			brick->remainingHp = 1;
			brick->position = VectorSum(
				board_top_left,
				(Vector2) {
					col * BRICK_WIDTH + col * PAD_COL,
					row * BRICK_HEIGHT + row * PAD_ROW
				}
			);
		}
	}

}


static
void DrawBoard(Board* board)
{

	for (int row = 0; row < BOARD_ROW; row++)
	{
		for (int col = 0; col < BOARD_COL; col++)
		{
			DrawBrick(&(board->bricks[row][col]));
		}
	}
	
}