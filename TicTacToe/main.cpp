#include <iostream>
#include <raylib.h>
#include <string>
#include "game.h"

const std::string version = "1.01";

const int screenWidth = 850;
const int screenHeight = 850;
const int pixelwidth = 250;

// players
enum ePlayer
{
	NONE = 0,
	PLRX = 1,
	PLRO = 2,
};

// game_state
enum eGameState
{
	RUNNING = 0,
	WINNING = 1,
	TIE = 2,
};

const int POS[9][2] = {
	{0, 0},
	{1, 0},
	{2, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{0, 2},
	{1, 2},
	{2, 2} };

const int DIR[8][3] = {
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	{0, 4, 8},
	{2, 4, 6} };

const int WIN[8][4] = {
	{175, 175, 675, 175},
	{175, 425, 675, 425},
	{175, 675, 675, 675},
	{175, 175, 175, 675},
	{425, 175, 425, 675},
	{675, 175, 675, 675},
	{175, 175, 675, 675},
	{675, 175, 175, 675} };

/// @brief Start the program
/// @return result
int main()
{
	int counter = 0;
	std::string title = "TicTacToe in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
	InitWindow(screenWidth, screenHeight, title.c_str());
	Game_t *game = new Game_t();
	SetTargetFPS(20);

	// mainloop
	while (!WindowShouldClose())
	{
		++counter;
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

		// Ai Move
		if (counter % 25 == 0)
		{
			int move = game->CpuMove();
			if (move >= 0)
			{
				game->DoAiMove(move);
			}
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
	LoadImages();
	Init();
}

/// @brief Destructor
Game_t::~Game_t()
{
	UnloadImages();
}

/// @brief Update gui
void Game_t::Update()
{
	for (int i = 0; i < 8; ++i)
	{
		if (board[DIR[i][0]] == board[DIR[i][1]] &&
			board[DIR[i][0]] == board[DIR[i][2]] &&
			board[DIR[i][0]] != NONE)
		{
			state = WINNING;
			dir_winner = i;
			winner = board[DIR[i][0]];
		}
	}
	if (empty_squares == 0 && state == RUNNING)
	{
		state = TIE;
	}
}

/// @brief Draw gui
/// @param counter refresh couter
void Game_t::Draw(int counter)
{
	ClearBackground(RAYWHITE);
	std::string TXT[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	for (int i = 0; i < 9; ++i)
	{
		int x = POS[i][0];
		int y = POS[i][1];

		DrawText(TXT[i].c_str(), x * 250 + 60, y * 250 + 260, 20, BLACK);
		if (board[i] == NONE)
		{
			continue;
		}
		if (board[i] == PLRX)
		{
			DrawTexture(img_plrx, x * 250 + 50, y * 250 + 50, RAYWHITE);
		}
		else
		{
			DrawTexture(img_plro, x * 250 + 50, y * 250 + 50, RAYWHITE);
		}
	}
	DrawLine(300, 50, 300, 800, BLACK);
	DrawLine(550, 50, 550, 800, BLACK);
	DrawLine(50, 300, 800, 300, BLACK);
	DrawLine(50, 550, 800, 550, BLACK);

	// Draw game state
	if (state == RUNNING)
	{
		if (side2move == PLRX)
		{
			DrawText("Player X, make your choice 1..9", 300, 825, 20, BLACK);
		}
		else
		{
			DrawText("Player O, calculate move", 300, 825, 20, BLACK);
		}
	}
	else if (state == TIE)
	{
		DrawRectangle(801, 801, 49, 49, YELLOW);
		DrawText("R", 825, 825, 20, BLACK);
		DrawText("It's a draw, press r to restart", 300, 825, 20, BLACK);
	}
	else if (state == WINNING)
	{
		DrawRectangle(801, 801, 49, 49, YELLOW);
		DrawText("R", 825, 825, 20, BLACK);
		Color col = RAYWHITE;
		if (winner == PLRX)
		{
			DrawText("Player X wins!, press r to restart", 300, 825, 20, BLACK);
			col = DARKGREEN;
		}
		else
		{
			DrawText("Player O wins!, press r to restart", 300, 825, 20, BLACK);
			col = RED;
		}
		for (int i = -1; i <= 1; ++i)
		{
			if (WIN[dir_winner][0] != WIN[dir_winner][2])
			{
				DrawLine(WIN[dir_winner][0], WIN[dir_winner][1] + i, WIN[dir_winner][2], WIN[dir_winner][3] + i, col);
			}
			else
			{
				DrawLine(WIN[dir_winner][0] + i, WIN[dir_winner][1], WIN[dir_winner][2] + i, WIN[dir_winner][3], col);
			}
		}
	}
	std::string strteller = std::to_string(counter);
	DrawText(strteller.c_str(), 1, 1, 20, RED);
}

/// @brief Do human move
/// @param keyval translated keycode
void Game_t::DoHumanMove(int keyval)
{
	if (keyval < 10)
	{
		if (state == RUNNING && side2move == PLRX)
		{
			board[keyval - 1] = side2move;
			empty_squares--;
			side2move = PLRO;
#ifndef NDEBUG
			printf("You plays %d\n", keyval);
#endif
		}
	}
	else
	{
		if (state != RUNNING)
		{
#ifndef NDEBUG
			printf("New game\n");
#endif
			Init();
		}
	}
}

/// @brief Translate keypress to humanmove
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
	else if (key == KEY_EIGHT)
	{
		DoHumanMove(8);
	}
	else if (key == KEY_NINE)
	{
		DoHumanMove(9);
	}
	else if (key == KEY_R)
	{
		DoHumanMove(10);
	}
}

/// @brief Translate mousepress to humanmove
/// @param x Column
/// @param y Row
void Game_t::MousePress(int x, int y)
{
	int n = 0;
	if (x >= 50 && x < 300 && y >= 50 && y < 300)
	{
		n = 1;
	}
	else if (x >= 300 && x < 550 && y >= 50 && y < 300)
	{
		n = 2;
	}
	else if (x >= 550 && x < 800 && y >= 50 && y < 300)
	{
		n = 3;
	}
	else if (x >= 50 && x < 300 && y >= 300 && y < 550)
	{
		n = 4;
	}
	else if (x >= 300 && x < 550 && y >= 300 && y < 550)
	{
		n = 5;
	}
	else if (x >= 550 && x < 800 && y >= 300 && y < 550)
	{
		n = 6;
	}
	else if (x >= 50 && x < 300 && y >= 550 && y < 800)
	{
		n = 7;
	}
	else if (x >= 300 && x < 550 && y >= 550 && y < 800)
	{
		n = 8;
	}
	else if (x >= 550 && x < 800 && y >= 550 && y < 800)
	{
		n = 9;
	}
	else if (x >= 801 && y >= 801)
	{
		n = 10;
	}
	if (n > 0)
	{
		DoHumanMove(n);
	}
}
	
/// @brief Calculate the best move for the AI
/// @return best move
int Game_t::CpuMove()
{
	int copyBoard[9] = { NONE }, possibleMoves[9] = { NONE }, cornersOpen[9] = { NONE }, edgesOpen[9] = { NONE };
	int cntPM = 0, cntCO = 0, cntEO = 0;
	int move = -1;

	if (state == RUNNING && side2move == PLRO)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (board[i] == NONE)
			{
				possibleMoves[cntPM] = i;
				++cntPM;
			}
		}
		// for possible winning move to take or to block opponents winning move
		for (int p = PLRO; p >= PLRX; p--)
		{
			for (int i = 0; i < cntPM; ++i)
			{
				int n = possibleMoves[i];
				for (int j = 0; j < 9; ++j)
				{
					copyBoard[j] = board[j];
				}
				copyBoard[n] = p;
				if (IsWinner(copyBoard, p))
				{
					return n;
				}
			}
		}

		// Try to take one of the corners 0,2,6,8 or the center 4
		for (int i = 0; i < cntPM; ++i)
		{
			int n = possibleMoves[i];
			if (n % 2 == 0)
			{
				cornersOpen[cntCO] = n;
				++cntCO;
			}
		}
		if (cntCO > 0)
		{
			int idx = 0;
			if (cntCO > 1)
			{
				idx = GetRandomValue(0, cntCO - 1);
			}
			move = cornersOpen[idx];
			return move;
		}

		// Try to take the center
		for (int i = 0; i < cntPM; ++i)
		{
			int n = possibleMoves[i];
			if (n == 4)
			{
				return n;
			}
		}

		// Try to take one of the edges 1, 3, 5, 7
		for (int i = 0; i < cntPM; ++i)
		{
			int n = possibleMoves[i];
			if (n % 2 == 1)
			{
				edgesOpen[cntEO++] = n;
			}
		}

		if (cntEO > 0)
		{
			int idx = 0;
			if (cntEO > 1)
			{
				idx = GetRandomValue(0, cntEO - 1);
			}
			move = edgesOpen[idx];
		}
	}

	return move;
}

/// @brief Do the AI move
/// @param move best move
void Game_t::DoAiMove(int move)
{
	board[move] = side2move;
	empty_squares -= 1;
	side2move = PLRX;
#ifndef NDEBUG
	printf("AI plays %d\n", (move + 1));
#endif
}

/// @brief Load the image containers
void Game_t::LoadImages()
{
	img_plro = LoadTexture("assets/images/plro.png");
	img_plrx = LoadTexture("assets/images/plrx.png");
	img_plro.width = pixelwidth;
	img_plro.height = pixelwidth;
	img_plrx.width = pixelwidth;
	img_plrx.height = pixelwidth;
}

/// @brief Unload the image containers
void Game_t::UnloadImages() const
{
	UnloadTexture(img_plrx);
	UnloadTexture(img_plro);
}

/// @brief Initialize the game
void Game_t::Init()
{
	int plr = GetRandomValue(PLRX, PLRO);
	for (int i = 0; i < 9; ++i)
	{
		board[i] = NONE;
	}
	empty_squares = 9;
	side2move = plr;
	state = RUNNING;
	winner = NONE;
	dir_winner = -1;
}

/// @brief Is there a winner?
/// @param brd 3x3 board
/// @param plr human or AI
/// @return true or false
bool Game_t::IsWinner(int* brd, int plr)
{
	for (int i = 0; i < 8; ++i)
	{
		if (brd[DIR[i][0]] == brd[DIR[i][1]] &&
			brd[DIR[i][0]] == brd[DIR[i][2]] &&
			brd[DIR[i][0]] == plr)
		{
			return true;
		}
	}
	return false;
}
