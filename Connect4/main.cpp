#include <iostream>
#include <raylib.h>
#include <utility>
#include <string>
#include "game.h"

const std::string version = "1.01";

// board size
const int ROW_COUNT = 6;
const int COL_COUNT = 7;
const int MID_ROW_COUNT = 3;
const int MID_COL_COUNT = 4;
// square size
const int SQUARESIZE = 100;
// piece size
const int RADIUS = 45;
// draw
const int NUMBERROW = 75;
const int TEXTCOL = 250;
const int TEXTROW = 25;
// search
const int WINDOW_LENGTH = 4;
const int MAX_DEPTH = 5;
const int INF = 2147483647;
const int MAXMV = 1000000000;
const int NONE = -1;

// macro's
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define SQR(r, c) ((r) * 7 + (c)) // row, col

enum ePlayer
{
	PLAYER = 0,
	AI = 1,
};

enum ePiece
{
	EMPTY = 0,
	PLAYER_PIECE = 1,
	AI_PIECE = 2,
};

enum eGamestate
{
	RUNNING = 0,
	WINNING = 1,
	TIE = 2,
};

/// @brief Start the program
/// @return result
int main()
{
	// initialize
	const int screenWidth = COL_COUNT * SQUARESIZE;
	const int screenHeight = (ROW_COUNT + 1) * SQUARESIZE;

	int counter = 0;

	std::string title = "Connect Four in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;

	InitWindow(screenWidth, screenHeight, title.c_str());
	Game_t *game = new Game_t();
	SetTargetFPS(20);

	// mainloop
	while (!WindowShouldClose())
	{
		counter += 1;
		// update
		game->Update();

		// draw
		BeginDrawing();
		game->Draw(counter);
		EndDrawing();

		// keypress
		int key = GetKeyPressed();
		if (key > 0)
		{
			// empty buffer
			int keybuf = key;
			while (keybuf > 0)
			{
				keybuf = GetKeyPressed();
			}
			game->KeyPress(key);
		}

		// Mouse Press
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			int x = GetMouseX();
			int y = GetMouseY();
			game->MousePress(x, y);
		}

		// AI move
		if (counter % 25 == 0)
		{
			game->CpuMove();
		}
	}

	// clean up
	delete game;
	CloseWindow();
	return 0;
}

/// @brief Constructor
Game_t::Game_t()
{
	for (int i = 0; i < 42; ++i)
	{
		board[i] = NONE;
		if (i < 5)
			dir_winner[i] = NONE;
	}
	gamestate = RUNNING;
	side2move = PLAYER;
	winner = NONE;
	Init();
}

/// @brief Update the gui
void Game_t::Update()
{
	if (gamestate == RUNNING)
	{
		int tmp[5];
		IsTerminalNode(board, tmp);
		if (tmp[4] == 1)
		{
			gamestate = WINNING;
			winner = PLAYER;
			for (int i = 0; i < 4; ++i)
			{
				dir_winner[i] = tmp[i];
			}
		}
		else if (tmp[4] == 2)
		{
			gamestate = WINNING;
			winner = AI;
			for (int i = 0; i < 4; ++i)
			{
				dir_winner[i] = tmp[i];
			}
		}
		else if (tmp[4] == 3)
		{
			gamestate = TIE;
		}
	}
}

/// @brief Draw the gui
/// @param counter Guicounter
void Game_t::Draw(int counter) const
{
	ClearBackground(RAYWHITE);
	std::string TXT[] = {"1", "2", "3", "4", "5", "6", "7"};

	// draw board
	for (int c = 0; c < COL_COUNT; ++c)
	{
		for (int r = 0; r < ROW_COUNT; ++r)
		{
			DrawRectangle(
				c * SQUARESIZE,
				r * SQUARESIZE + SQUARESIZE,
				SQUARESIZE,
				SQUARESIZE,
				BLUE);
			DrawCircle(
				(int)(c * SQUARESIZE + SQUARESIZE / 2),
				(int)(r * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2),
				(float)(RADIUS),
				BLACK);
			if (board[SQR(5 - r, c)] == PLAYER_PIECE)
			{
				DrawCircle(
					(int)(c * SQUARESIZE + SQUARESIZE / 2),
					(int)(r * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2),
					(float)(RADIUS),
					RED);
			}
			if (board[SQR(5 - r, c)] == AI_PIECE)
			{
				DrawCircle(
					(int)(c * SQUARESIZE + SQUARESIZE / 2),
					(int)(r * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2),
					(float)(RADIUS),
					YELLOW);
			}
			DrawText(TXT[c].c_str(), (int)(c * SQUARESIZE + SQUARESIZE / 2), NUMBERROW, 20, BLACK);
		}
	}
	// draw gamestate
	if (gamestate == RUNNING)
	{
		if (side2move == PLAYER)
		{
			DrawText("Player make your choice 1..7", TEXTCOL, TEXTROW, 20, BLACK);
		}
		else
		{
			DrawText("AI, calculate move", TEXTCOL, TEXTROW, 20, BLACK);
		}
	}
	else if (gamestate == TIE)
	{
		DrawRectangle(651, 1, 49, 49, YELLOW);
		DrawText("R", 675, 25, 20, BLACK);
		DrawText("It's a draw, press r to restart", TEXTCOL, TEXTROW, 20, BLACK);
	}
	else if (gamestate == WINNING)
	{
		DrawRectangle(651, 1, 49, 49, YELLOW);
		DrawText("R", 675, 25, 20, BLACK);
		if (winner == PLAYER)
		{
			DrawText("Player wins!, press r to restart", TEXTCOL, TEXTROW, 20, BLACK);
		}
		else
		{
			DrawText("AI wins!, press r to restart", TEXTCOL, TEXTROW, 20, BLACK);
		}
		for (int i = -1; i <= 1; ++i)
		{
			if (dir_winner[0] != dir_winner[2])
			{
				DrawLine(
					dir_winner[0],
					dir_winner[1] + i,
					dir_winner[2],
					dir_winner[3] + i,
					BLACK);
			}
			else
			{
				DrawLine(
					dir_winner[0] + i,
					dir_winner[1],
					dir_winner[2] + i,
					dir_winner[3],
					BLACK);
			}
		}
	}
#ifndef NDEBUG
	std::string strteller = std::to_string(counter);
	DrawText(strteller.c_str(), 1, 1, 20, RED);
#endif
}

/// @brief Do the human move
/// @param keyval translated keycode
void Game_t::DoHumanMove(int keyval)
{
	if (keyval < 10)
	{
		if (gamestate == RUNNING && side2move == PLAYER)
		{
			int row = GetNextOpenRow(board, keyval - 1);
			if (row != NONE)
			{
				board[SQR(row, keyval - 1)] = PLAYER_PIECE;
				side2move = AI;
#ifndef NDEBUG
				printf("You plays %d\n", keyval);
#endif
			}
		}
	}
	else
	{
		if (gamestate != RUNNING)
		{
#ifndef NDEBUG
			printf("New game\n");
#endif
			Init();
		}
	}
}

/// @brief translate keypress to humanmove
/// @param key keycode
void Game_t::KeyPress(int key)
{
	if (key == KEY_ONE)
	{
		DoHumanMove(1);
	}
	else if (key == KEY_TWO)
	{
		DoHumanMove(2);
	}
	else if (key == KEY_THREE)
	{
		DoHumanMove(3);
	}
	else if (key == KEY_FOUR)
	{
		DoHumanMove(4);
	}
	else if (key == KEY_FIVE)
	{
		DoHumanMove(5);
	}
	else if (key == KEY_SIX)
	{
		DoHumanMove(6);
	}
	else if (key == KEY_SEVEN)
	{
		DoHumanMove(7);
	}
	else if (key == KEY_R)
	{
		DoHumanMove(10);
	}
}

/// @brief translate mousepress to humanmove
/// @param x column
/// @param y row
void Game_t::MousePress(int x, int y)
{
	int n = 0;
	if (x >= 0 && x < 100 && y >= 100 && y < 700)
	{
		n = 1;
	}
	else if (x >= 100 && x < 200 && y >= 100 && y < 700)
	{
		n = 2;
	}
	else if (x >= 200 && x < 300 && y >= 100 && y < 700)
	{
		n = 3;
	}
	else if (x >= 300 && x < 400 && y >= 100 && y < 700)
	{
		n = 4;
	}
	else if (x >= 400 && x < 500 && y >= 0 && y < 700)
	{
		n = 5;
	}
	else if (x >= 500 && x < 600 && y >= 100 && y < 700)
	{
		n = 6;
	}
	else if (x >= 600 && x < 700 && y >= 100 && y < 700)
	{
		n = 7;
	}
	else if (x >= 651 && y <= 49)
	{
		n = 10;
	}
	if (n > 0)
	{
		DoHumanMove(n);
	}
}

/// @brief Calculate the best move for the AI and process that move
void Game_t::CpuMove()
{
	if (side2move == AI && gamestate == RUNNING)
	{
		std::pair<int, int> move = MiniMax(board, MAX_DEPTH, -MAXMV, MAXMV, true);
		if (move.first >= 0 && move.first < 7)
		{
			int row = GetNextOpenRow(board, move.first);
			board[SQR(row, move.first)] = AI_PIECE;
			side2move = PLAYER;
#ifndef NDEBUG
			printf("AI plays %d\n", (move.first + 1));
#endif
		}
		else
		{
#ifndef NDEBUG
			printf("Error, no move found!\n");
#endif
		}
	}
}

/// @brief Initialize the game
void Game_t::Init()
{
	int plr = GetRandomValue(PLAYER, AI);
	for (int r = 0; r < ROW_COUNT; ++r)
	{
		for (int c = 0; c < COL_COUNT; ++c)
		{
			board[SQR(r, c)] = EMPTY;
		}
	}
	gamestate = RUNNING;
	side2move = plr;
	winner = NONE;
	for (int i = 0; i < 5; ++i)
	{
		dir_winner[i] = NONE;
	}
}

/// @brief Get the next row from a column
/// @param brd 6x7 board
/// @param col column
/// @return row or NONE
int Game_t::GetNextOpenRow(int *brd, int col)
{
	for (int row = 0; row < ROW_COUNT; ++row)
	{
		if (brd[SQR(row, col)] == EMPTY)
		{
			return row;
		}
	}
	return NONE;
}

/// @brief Is the column a valid move
/// @param brd 6x7 board
/// @param col column
/// @return true or false
bool Game_t::IsValidLocation(int *brd, int col)
{
	return brd[SQR(ROW_COUNT - 1, col)] == EMPTY;
}

/// @brief Get all the valid locations
/// @param brd 6*7board
/// @param data valid locations
/// @return size of the valid locations
int Game_t::GetValidLocations(int *brd, int *data)
{
	int cnt = 0;
	for (int col = 0; col < COL_COUNT; ++col)
	{
		if (IsValidLocation(brd, col))
		{
			data[cnt] = col;
			++cnt;
		}
	}
	return cnt;
}

/// @brief Is the current node winning,loosing, tied or not
/// @param brd 6*7 board
/// @param data game result
/// @return true or false
bool Game_t::IsTerminalNode(int *brd, int *data)
{
	WinningMove(brd, data, PLAYER_PIECE);
	if (data[0] != NONE)
	{
		data[4] = 1;
		return true;
	}
	WinningMove(brd, data, AI_PIECE);
	if (data[0] != NONE)
	{
		data[4] = 2;
		return true;
	}
	int moves[7] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE};
	int size = GetValidLocations(brd, moves);
	if (size == 0)
	{
		data[4] = 3;
		return true;
	}
	return false;
}

/// @brief Calculate the best move
/// @param brd 6*7 board
/// @param depth search depth
/// @param alpha alpha score
/// @param beta beta score
/// @param maximizingPlayer true => Maximize the score, false => Minimize the score
/// @return <column, value>
std::pair<int, int> Game_t::MiniMax(int *brd, int depth, int alpha, int beta, bool maximizingPlayer)
{
	std::pair<int, int> t = {NONE, NONE};
	int is_terminal[5] = {NONE, NONE, NONE, NONE, NONE};

	bool term = IsTerminalNode(brd, is_terminal);
	int copybrd[42] = {NONE};

	if (depth == 0 || term)
	{
		if (is_terminal[4] > NONE)
		{
			if (is_terminal[4] == 2)
			{
				t.first = NONE;
				t.second = MAXMV;
				return t;
			}
			else if (is_terminal[4] == 1)
			{
				t.first = NONE;
				t.second = -MAXMV;
				return t;
			}
			else
			{
				// Game is over, no more valid moves
				t.first = NONE;
				t.second = 0;
				return t;
			}
		}
		else
		{
			// Depth is zero
			t.first = NONE;
			t.second = ScorePosition(brd, AI_PIECE);
			return t;
		}
	}

	int valid_locations[7] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE};
	int size = GetValidLocations(brd, valid_locations);

	int idx = 0;
	if (size > 0)
	{
		idx = GetRandomValue(0, size - 1);
	}
	int column;
	int value;

	if (maximizingPlayer)
	{
		column = valid_locations[idx];
		value = -MAXMV;

		for (int i = 0; i < size; ++i)
		{
			int col = valid_locations[i];
			int row = GetNextOpenRow(brd, col);
			for (int r = 0; r < ROW_COUNT; ++r)
			{
				for (int c = 0; c < COL_COUNT; ++c)
				{
					copybrd[SQR(r, c)] = brd[SQR(r, c)];
				}
			}
			copybrd[SQR(row, col)] = AI_PIECE;
			t = MiniMax(copybrd, depth - 1, alpha, beta, false);
			if (t.second > value)
			{
				value = t.second;
				column = col;
			}
			alpha = MAX(alpha, value);
			if (alpha >= beta)
			{
				break;
			}
		}
	}
	else
	{
		// Minimizing player
		column = valid_locations[idx];
		value = MAXMV;

		for (int i = 0; i < size; ++i)
		{
			int col = valid_locations[i];
			int row = GetNextOpenRow(brd, col);
			for (int r = 0; r < ROW_COUNT; ++r)
			{
				for (int c = 0; c < COL_COUNT; ++c)
				{
					copybrd[SQR(r, c)] = brd[SQR(r, c)];
				}
			}
			copybrd[SQR(row, col)] = PLAYER_PIECE;
			t = MiniMax(copybrd, depth - 1, alpha, beta, true);
			if (t.second < value)
			{
				value = t.second;
				column = col;
			}
			beta = MIN(beta, value);
			if (alpha >= beta)
			{
				break;
			}
		}
	}

	t.first = column;
	t.second = value;

	return t;
}

/// @brief Is there a winning move?
/// @param brd 6*7 board
/// @param data game result
/// @param piece Human piece or AI piece
void Game_t::WinningMove(int *brd, int *data, int piece)
{
	for (int i = 0; i < 5; ++i)
	{
		data[i] = NONE;
	}

	// Check horizontal locations for win
	// col : 0..3, 1..4, 2..5, 3..6
	// row : 0..5
	for (int r = 0; r < ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			if (brd[SQR(r, c)] == piece &&
				brd[SQR(r, c + 1)] == piece &&
				brd[SQR(r, c + 2)] == piece &&
				brd[SQR(r, c + 3)] == piece)
			{
				data[0] = (int)(c * SQUARESIZE + SQUARESIZE / 2);
				data[1] = (int)((5 - r) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				data[2] = (int)((c + 3) * SQUARESIZE + SQUARESIZE / 2);
				data[3] = (int)((5 - r) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				return;
			}
		}
	}

	// Check vertical locations for win
	// col: 0..6
	// row: 0..3, 1..4, 2..5
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < COL_COUNT; ++c)
		{
			if (brd[SQR(r, c)] == piece &&
				brd[SQR(r + 1, c)] == piece &&
				brd[SQR(r + 2, c)] == piece &&
				brd[SQR(r + 3, c)] == piece)
			{
				data[0] = (int)(c * SQUARESIZE + SQUARESIZE / 2);
				data[1] = (int)((5 - r) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				data[2] = (int)(c * SQUARESIZE + SQUARESIZE / 2);
				data[3] = (int)((5 - (r + 3)) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				return;
			}
		}
	}

	// Check positively sloped diaganols
	// col: 0..3, 1..4, 2..5, 3..6
	// row: 0..3, 1..4, 2..5
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			if (brd[SQR(r, c)] == piece &&
				brd[SQR(r + 1, c + 1)] == piece &&
				brd[SQR(r + 2, c + 2)] == piece &&
				brd[SQR(r + 3, c + 3)] == piece)
			{
				data[0] = (int)(c * SQUARESIZE + SQUARESIZE / 2);
				data[1] = (int)((5 - r) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				data[2] = (int)((c + 3) * SQUARESIZE + SQUARESIZE / 2);
				data[3] = (int)((5 - (r + 3)) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				return;
			}
		}
	}

	// Check negatively sloped diaganols
	// col: 3..0, 4..1, 5..2, 6..3
	// row: 5..2, 4..1, 3..0
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			if (brd[SQR(r + 3, c)] == piece &&
				brd[SQR(r + 2, c + 1)] == piece &&
				brd[SQR(r + 1, c + 2)] == piece &&
				brd[SQR(r, c + 3)] == piece)
			{
				data[0] = (int)(c * SQUARESIZE + SQUARESIZE / 2);
				data[1] = (int)((5 - (r + 3)) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				data[2] = (int)((c + 3) * SQUARESIZE + SQUARESIZE / 2);
				data[3] = (int)((5 - r) * SQUARESIZE + SQUARESIZE + SQUARESIZE / 2);
				return;
			}
		}
	}
}

/// @brief Count the pieces in a window of 4 squares in any direction
/// @param window 4 squares
/// @param piece Human piece, AI piece or Empty
/// @return count
int Game_t::WindowCount(int *window, int piece)
{
	int cnt = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (window[i] == piece)
		{
			++cnt;
		}
	}
	return cnt;
}

/// @brief Evaluate a window of 4 squares in any direction
/// @param window 4 squares
/// @param piece Human piece or AI piece
/// @return score
int Game_t::EvaluateWindow(int *window, int piece)
{
	int score = 0;
	int count_piece = 0;
	int count_opp_piece = 0;
	int count_empty = 0;
	int opp_piece = piece == PLAYER_PIECE ? AI_PIECE : PLAYER_PIECE;

	count_piece = WindowCount(window, piece);
	count_opp_piece = WindowCount(window, opp_piece);
	count_empty = WindowCount(window, EMPTY);

	if (count_piece == 4)
	{
		score += 100;
	}
	else if (count_piece == 3 && count_empty == 1)
	{
		score += 5;
	}
	else if (count_piece == 2 && count_empty == 2)
	{
		score += 2;
	}
	else if (count_opp_piece == 3 && count_empty == 1)
	{
		score -= 4;
	}

	return score;
}

/// @brief Evaluatie the current board position
/// @param brd 6*7 board
/// @param piece Human piece or AI piece
/// @return score
int Game_t::ScorePosition(int *brd, int piece)
{
	int score = 0;
	int center_count = 0;
	int window[4];

	// Score center column
	for (int r = 0; r < ROW_COUNT; ++r)
	{
		if (brd[SQR(r, 3)] == piece)
		{
			center_count += 1;
		}
	}
	score += center_count * 3;

	// Score horizontal locations
	// col : 0..3, 1..4, 2..5, 3..6
	// row : 0..5
	for (int r = 0; r < ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			for (int i = 0; i < WINDOW_LENGTH; ++i)
			{
				window[i] = brd[SQR(r, c + i)];
			}
			score += EvaluateWindow(window, piece);
		}
	}

	// Score vertical locations
	// col: 0..6
	// row: 0..3, 1..4, 2..5
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < COL_COUNT; ++c)
		{
			for (int i = 0; i < WINDOW_LENGTH; ++i)
			{
				window[i] = brd[SQR(r + i, c)];
			}
			score += EvaluateWindow(window, piece);
		}
	}

	// Score positive sloped diagonal locations
	// col: 0..3, 1..4, 2..5, 3..6
	// row: 0..3, 1..4, 2..5
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			for (int i = 0; i < WINDOW_LENGTH; ++i)
			{
				window[i] = brd[SQR(r + i, c + i)];
			}
			score += EvaluateWindow(window, piece);
		}
	}

	// Score negative sloped diagonal locations
	// col: 3..0, 4..1, 5..2, 6..3
	// row: 5..2, 4..1, 3..0
	for (int r = 0; r < MID_ROW_COUNT; ++r)
	{
		for (int c = 0; c < MID_COL_COUNT; ++c)
		{
			for (int i = 0; i < WINDOW_LENGTH; ++i)
			{
				window[i] = brd[SQR(r + 3 - i, c + i)];
			}
			score += EvaluateWindow(window, piece);
		}
	}

	return score;
}
