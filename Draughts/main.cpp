#include <iostream>
#include <raylib.h>
#include <string>
#include <thread>
#include <vector>
#include "board.h"
#include "move.h"

// version
const std::string version = "0.80";

// draw
const int SQUARESIZE = 72;
const int HALFSQUARESIZE = 36;
const int SCREENWIDTH = 1600;
const int SCREENHEIGHT = 72 * 12;
const float RADIUS = 30.0;
const float INNERRADIUS = 20.0;

static Move bestmove;
static int bestval;
static MoveList ml;
static Board board;
static bool taskrunning;
static bool taskready;
static int max_depth;
static int game_level;
static int playercolor[2] = {SWHITE, SBLACK};
static int nodes;

const std::string PLAYERCOLORS[2] = {
	"Wit", "Zwart"};

const std::string GAMELEVELS[4] = {
	"Makkelijk",
	"Gemiddeld",
	"Hard",
	"Extra moeilijk"};

const std::string HELPSTR[19] = {
	"Werking van het programma:",
	"",
	"Eerst kun je de instellingen van kleur en niveau instellen door een letter in het rood te klikken via keyboard of met de muis.",
	"De huidige keuze wordt aangegeven door <=== achter de tekst.",
	"Als de instellingen goed staan kun het spel starten door je de rode letter s te klikken via keyboard of met de muis.",
	"",
	"Als het jouw beurt is om te spelen tijdens het spel zie je naast het dambord een lijst met de mogelijke zetten, bladeren door",
	"deze zetten doe je met de pijltjes-toetsen omhoog of omlaag via keyboard of met de muis op de pijl afbeeldingen te klikken.",
	"De zet die geselecteerd is wordt op het bord getoond, in het veld van een groen en in het veld naar een blauw vierkant getoond,",
	"als het een slagbeurt is worden in de veld(en) van de/het stuk(ken) die geslagen zal/zullen worden een rood vierkant getoond.",
	"Als je de gewenste zet hebt geselecteerd kun je die zetten door op enterkey<--| te klikken of met de muis op de Enter afbeelding",
	"te klikken, de zet wordt uitgevoerd en dan gaat de AI een zet zoeken en zal de beste zet ook uitvoeren waarna het weer jouw",
	"beurt is. Was je niet tevreden over jouw keuze dan kun je met F7 de zet terugnemen.",
	"",
	"Kunnen er geen zetten worden gevonden dan is de partij afgelopen en is de andere speler de winnaar, door het drukken op F5 wordt",
	"er weer een nieuwe partij gestart.",
	"",
	"Als tijdens de partij blijkt dat je verloren staat kun je partij opgeven door op de X-toets te klikken of met de muis op op de ",
	"X-afbeelding te klikken."};

enum ePlayer
{
	DPLAYER,
	AI,
};

void task(Board *b)
{
	taskrunning = true;
	taskready = false;
	bestval = -999999;
#ifndef NDEBUG
	std::cout << "task running" << std::endl;
#endif

	bestval = b->BestEval(0, -MAXEVAL, MAXEVAL);

#ifndef NDEBUG
	std::cout << "task ready" << std::endl;
#endif
	taskready = true;
}

// Main program
int main()
{
	int keuze = 1;
	int guiteller = 0;
	int gamestate = 0;

	std::string title = "Draughts in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
	// initialize
	MoveList game_moves = MoveList();
	board = Board();
	board.InitBoard();
	bestval = board.Evaluate();

	ml = board.Generate(board.GetSide());
	InitWindow(SCREENWIDTH, SCREENHEIGHT, title.c_str());
	Texture2D enterbutton = LoadTexture("./assets/Enter.png");
	enterbutton.width = SQUARESIZE * 2;
	enterbutton.height = SQUARESIZE;
	Texture2D arrowup = LoadTexture("./assets/pijlomhoog.png");
	arrowup.width = SQUARESIZE;
	arrowup.height = SQUARESIZE;
	Texture2D arrowdown = LoadTexture("./assets/pijlomlaag.png");
	arrowdown.width = SQUARESIZE;
	arrowdown.height = SQUARESIZE;
	Texture2D plrx = LoadTexture("./assets/plrx.png");
	plrx.width = SQUARESIZE;
	plrx.height = SQUARESIZE;

	SetTargetFPS(10);

	// mainloop
	while (!WindowShouldClose())
	{
		++guiteller;
		// draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (gamestate == 0)
		{
			DrawText(
				"Dammen",
				100,
				100,
				30,
				DARKBLUE);
			DrawText(
				"Kies kleur:",
				100,
				140,
				20,
				DARKBLUE);
			for (int i = 0; i < 2; ++i)
			{
				std::string txt = PLAYERCOLORS[i];
				if (playercolor[DPLAYER] == i)
				{
					txt += " <===";
				}
				DrawText(
					txt.c_str(),
					140,
					160 + i * 20,
					20,
					DARKBLUE);
				DrawText(
					PLAYERCOLORS[i].substr(0, 1).c_str(),
					140,
					160 + i * 20,
					20,
					RED);
			}
			DrawText(
				"Kies niveau: ",
				100,
				200,
				20,
				DARKBLUE);
			for (int i = 0; i < 4; ++i)
			{
				std::string txt = GAMELEVELS[i];
				if (game_level == i)
				{
					txt += " <===";
				}
				DrawText(
					txt.c_str(),
					140,
					220 + i * 20,
					20,
					DARKBLUE);
				DrawText(
					GAMELEVELS[i].substr(0, 1).c_str(),
					140,
					220 + i * 20,
					20,
					RED);
			}
			DrawText(
				"Start het spel...",
				100,
				300,
				25,
				DARKPURPLE);
			DrawText(
				"S",
				100,
				300,
				25,
				RED);
			for (int i = 0; i < 19; ++i)
			{
				DrawText(
					HELPSTR[i].c_str(),
					50,
					350 + i * 20,
					20,
					BLACK);
			}
			DrawText(
				HELPSTR[0].c_str(),
				50,
				350,
				20,
				BLUE);
		}
		else
		{
			// Draw Board
			DrawRectangle(
				HALFSQUARESIZE,
				HALFSQUARESIZE,
				SQUARESIZE * 11,
				SQUARESIZE * 11,
				DARKGRAY);
			DrawRectangle(
				SQUARESIZE,
				SQUARESIZE,
				SQUARESIZE * 10,
				SQUARESIZE * 10,
				LIGHTGRAY);
			int cntA = board.GetCount(playercolor[AI]);
			int cntP = board.GetCount(playercolor[DPLAYER]);
			DrawText(
				std::to_string(cntA).c_str(),
				SQUARESIZE,
				HALFSQUARESIZE + 10,
				20,
				RAYWHITE);
			DrawText(
				std::to_string(cntP).c_str(),
				SQUARESIZE,
				SQUARESIZE * 11 + 10,
				20,
				RAYWHITE);
			Move m = Move();
			if (ml.size() > 0)
			{
				m = ml[keuze - 1];
			}
			for (int y = 0; y < 10; ++y)
			{
				for (int x = 0; x < 10; ++x)
				{
					if ((x % 2 == 0 && y % 2 == 0) ||
						(x % 2 == 1 && y % 2 == 1))
					{
						continue;
					}
					else
					{
						bool white_pawn = playercolor[DPLAYER] == SWHITE ? board.GetPiece(BOARDNUMBER[y][x]) == PAWN_WHITE : board.GetPiece(ROTATEDBOARDNUMBER[y][x]) == PAWN_WHITE;
						bool black_pawn = playercolor[DPLAYER] == SWHITE ? board.GetPiece(BOARDNUMBER[y][x]) == PAWN_BLACK : board.GetPiece(ROTATEDBOARDNUMBER[y][x]) == PAWN_BLACK;
						bool white_king = playercolor[DPLAYER] == SWHITE ? board.GetPiece(BOARDNUMBER[y][x]) == KING_WHITE : board.GetPiece(ROTATEDBOARDNUMBER[y][x]) == KING_WHITE;
						bool black_king = playercolor[DPLAYER] == SWHITE ? board.GetPiece(BOARDNUMBER[y][x]) == KING_BLACK : board.GetPiece(ROTATEDBOARDNUMBER[y][x]) == KING_BLACK;
						DrawRectangle(
							x * SQUARESIZE + SQUARESIZE,
							y * SQUARESIZE + SQUARESIZE,
							SQUARESIZE,
							SQUARESIZE,
							GRAY);
						if (white_pawn || white_king)
						{
							DrawCircle(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								RADIUS,
								RAYWHITE);
							DrawCircleLines(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								RADIUS,
								BLACK);
							DrawCircleLines(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								INNERRADIUS,
								BLACK);
							if (white_king)
							{
								DrawText(
									"D",
									x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE - 6,
									y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE - 8,
									20,
									BLACK);
							}
						}
						if (black_pawn || black_king)
						{
							DrawCircle(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								RADIUS,
								BLACK);
							DrawCircleLines(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								RADIUS,
								RAYWHITE);
							DrawCircleLines(
								x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE,
								INNERRADIUS,
								RAYWHITE);
							if (black_king)
							{
								DrawText(
									"D",
									x * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE - 6,
									y * SQUARESIZE + SQUARESIZE + HALFSQUARESIZE - 8,
									20,
									RAYWHITE);
							}
						}

						int bn = playercolor[DPLAYER] == SWHITE ? BOARDNUMBER[y][x] : ROTATEDBOARDNUMBER[y][x];
						Color col = GREEN;
						if (bn == m.GetPosition(SQT))
						{
							col = BLUE;
						}
						if ((bn == m.GetPosition(SQF) ||
							 bn == m.GetPosition(SQT)) &&
							board.GetSide() == playercolor[DPLAYER])
						{
							DrawRectangleLines(
								x * SQUARESIZE + SQUARESIZE,
								y * SQUARESIZE + SQUARESIZE,
								SQUARESIZE,
								SQUARESIZE,
								col);
							DrawRectangleLines(
								x * SQUARESIZE + SQUARESIZE + 1,
								y * SQUARESIZE + SQUARESIZE + 1,
								SQUARESIZE - 2,
								SQUARESIZE - 2,
								col);
							DrawRectangleLines(
								x * SQUARESIZE + SQUARESIZE + 2,
								y * SQUARESIZE + SQUARESIZE + 2,
								SQUARESIZE - 4,
								SQUARESIZE - 4,
								col);
						}

						if (m.GetSize() > 0 && board.GetSide() == playercolor[DPLAYER])
						{
							for (int i = 1; i <= m.GetSize(); ++i)
							{
								if (bn == m.GetPosition(SQT + i))
								{
									DrawRectangleLines(
										x * SQUARESIZE + SQUARESIZE,
										y * SQUARESIZE + SQUARESIZE,
										SQUARESIZE,
										SQUARESIZE,
										RED);
									DrawRectangleLines(
										x * SQUARESIZE + SQUARESIZE + 1,
										y * SQUARESIZE + SQUARESIZE + 1,
										SQUARESIZE - 2,
										SQUARESIZE - 2,
										RED);
									DrawRectangleLines(
										x * SQUARESIZE + SQUARESIZE + 2,
										y * SQUARESIZE + SQUARESIZE + 2,
										SQUARESIZE - 4,
										SQUARESIZE - 4,
										RED);
								}
							}
						}
					}
				}
			}

			// Draw zetten
			if (ml.size() > 0 && board.GetSide() == playercolor[DPLAYER] && !board.GetGameState())
			{
				DrawText(
					"F7 : neem een zet terug",
					SQUARESIZE * 12,
					HALFSQUARESIZE - 20,
					20,
					DARKBLUE);
				DrawText(
					"Zetten:",
					SQUARESIZE * 12,
					HALFSQUARESIZE,
					20,
					DARKBLUE);
				DrawTexture(
					arrowup,
					SQUARESIZE * 14,
					HALFSQUARESIZE,
					RAYWHITE);
				DrawTexture(
					arrowdown,
					SQUARESIZE * 14,
					HALFSQUARESIZE + SQUARESIZE,
					RAYWHITE);
				DrawTexture(
					enterbutton,
					SQUARESIZE * 14,
					HALFSQUARESIZE + SQUARESIZE * 2,
					RAYWHITE);
				DrawText(
					" enter = Doe Zet",
					SQUARESIZE * 14,
					HALFSQUARESIZE + SQUARESIZE * 3,
					20,
					DARKBLUE);
				DrawText(
					" enter",
					SQUARESIZE * 14,
					HALFSQUARESIZE + SQUARESIZE * 3,
					20,
					RED);
				DrawRectangle(
					SQUARESIZE * 14 - 1,
					SQUARESIZE * 4 - 1,
					SQUARESIZE + 2,
					SQUARESIZE + 2,
					DARKBLUE);
				DrawTexture(
					plrx,
					SQUARESIZE * 14,
					SQUARESIZE * 4,
					RAYWHITE);
				DrawText(
					" X = Geef op",
					SQUARESIZE * 14,
					SQUARESIZE * 5 + 20,
					20,
					DARKBLUE);
				DrawText(
					" X",
					SQUARESIZE * 14,
					SQUARESIZE * 5 + 20,
					20,
					RED);
				int n = 1;
				for (auto m : ml)
				{
					std::string text = m.ToString();
					if (keuze == n)
					{
						text += " <==";
					}
					DrawText(
						text.c_str(),
						SQUARESIZE * 12,
						HALFSQUARESIZE + n * 20,
						20,
						DARKBLUE);
					n += 1;
				}
			}

			// Draw partij
			DrawText(
				"Partij:",
				SQUARESIZE * 17,
				HALFSQUARESIZE,
				20,
				DARKBLUE);
			DrawText(
				"Wit:",
				SQUARESIZE * 17 + HALFSQUARESIZE,
				HALFSQUARESIZE + 20,
				20,
				DARKBLUE);
			DrawText(
				"Zwart:",
				SQUARESIZE * 19,
				HALFSQUARESIZE + 20,
				20,
				DARKBLUE);
			int minidx = game_moves.size() > 78 ? game_moves.size() - 78 : 0;
			int vert = HALFSQUARESIZE + 20;
			for (int i = minidx; i < game_moves.size(); i += 2)
			{
				std::string num = std::to_string((int)(i / 2) + 1);
				vert += 20;
				DrawText(
					num.c_str(),
					SQUARESIZE * 17,
					vert,
					20,
					DARKBLUE);
				if (game_moves.size() > i)
				{
					Move m = game_moves[i];
					DrawText(
						m.ToString().c_str(),
						SQUARESIZE * 17 + HALFSQUARESIZE,
						vert,
						20,
						DARKBLUE);
				}
				if (game_moves.size() > i + 1)
				{
					Move m = game_moves[i + 1];
					DrawText(
						m.ToString().c_str(),
						SQUARESIZE * 19,
						vert,
						20,
						DARKBLUE);
				}
			}

			if (board.GetGameState())
			{
				std::string winnaar = board.GetSide() == SWHITE ? "Zwart wint" : "Wit wint";
				DrawText(
					winnaar.c_str(),
					SQUARESIZE * 12,
					HALFSQUARESIZE + SQUARESIZE * 10,
					20,
					DARKGREEN);
				DrawText(
					"Druk op F5 voor een nieuw spel",
					SQUARESIZE * 12,
					HALFSQUARESIZE + SQUARESIZE * 10 + 20,
					20,
					DARKGREEN);
			}
#ifndef NDEBUG
			std::string evalstr = std::to_string(bestval);
			DrawText(
				evalstr.c_str(),
				HALFSQUARESIZE,
				HALFSQUARESIZE + 11 * SQUARESIZE + 10,
				20,
				BLUE);
#else
			DrawText(
				"zetten:",
				HALFSQUARESIZE,
				HALFSQUARESIZE + 11 * SQUARESIZE + 10,
				20,
				BLUE);
#endif
			std::string nodestr = std::to_string(nodes);
			DrawText(
				nodestr.c_str(),
				SQUARESIZE + HALFSQUARESIZE,
				HALFSQUARESIZE + 11 * SQUARESIZE + 10,
				20,
				BLUE);
		}

		EndDrawing();

		// keypress
		if (IsKeyPressed(KEY_W) && gamestate == 0)
		{
			playercolor[DPLAYER] = SWHITE;
			playercolor[AI] = SBLACK;
			continue;
		}

		if (IsKeyPressed(KEY_Z) && gamestate == 0)
		{
			playercolor[DPLAYER] = SBLACK;
			playercolor[AI] = SWHITE;
			continue;
		}

		if (IsKeyPressed(KEY_M) && gamestate == 0)
		{
			game_level = 0;
			continue;
		}

		if (IsKeyPressed(KEY_G) && gamestate == 0)
		{
			game_level = 1;
			continue;
		}

		if (IsKeyPressed(KEY_H) && gamestate == 0)
		{
			game_level = 2;
			continue;
		}

		if (IsKeyPressed(KEY_E) && gamestate == 0)
		{
			game_level = 3;
			continue;
		}

		if (IsKeyPressed(KEY_S) && gamestate == 0)
		{
			gamestate = 1;
			continue;
		}

		if (IsKeyPressed(KEY_X))
		{
			board.SetGameState(true);
			continue;
		}

		if (IsKeyPressed(KEY_UP) && gamestate == 1 && !board.GetGameState())
		{
			if (board.GetSide() == playercolor[DPLAYER])
			{
				if (ml.size() > 0 && keuze > 1)
				{
					keuze -= 1;
				}
			}
			continue;
		}

		if (IsKeyPressed(KEY_DOWN) && gamestate == 1 && !board.GetGameState())
		{
			if (board.GetSide() == playercolor[DPLAYER])
			{
				if (ml.size() > 0 && keuze < ml.size())
				{
					keuze += 1;
				}
			}
			continue;
		}

		if (IsKeyPressed(KEY_ENTER) && gamestate == 1 && !board.GetGameState())
		{
			if (board.GetSide() == playercolor[DPLAYER])
			{
				if (ml.size() > 0)
				{
					Move m = ml[keuze - 1];
					game_moves.push_back(m);
					board.DoMove(board.GetSide(), m);
					keuze = 1;
					board.SetSide(board.GetSide() ^ 1);
					ml = board.Generate(board.GetSide());
					taskrunning = false;
				}
				else
				{
					board.SetGameState(true);
				}
			}
			continue;
		}

		if (IsKeyPressed(KEY_F5) && gamestate == 1 && board.GetGameState())
		{
			board.InitBoard();
			ml = board.Generate(board.GetSide());
			game_moves.clear();
			gamestate = 0;
			continue;
		}

		if (IsKeyPressed(KEY_F7) && ml.size() > 0 && game_moves.size() > playercolor[DPLAYER] && board.GetSide() == playercolor[DPLAYER] && gamestate == 1 && !board.GetGameState())
		{
			for (int i = 0; i < 2; ++i)
			{
				Move m = game_moves[game_moves.size() - 1];
				game_moves.pop_back();
				board.UndoMove(m);
			}
			ml = board.Generate(board.GetSide());
			continue;
		}

		// Mouse Press
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			int x = GetMouseX();
			int y = GetMouseY();
#ifndef NDEBUG
			std::cout << "x:" << x << " y:" << y << std::endl;
#endif
			if (gamestate == 0)
			{
				if (x >= 140 && x < 160 && y >= 160 && y < 180)
				{
					playercolor[DPLAYER] = SWHITE;
					playercolor[AI] = SBLACK;
					continue;
				}
				if (x >= 140 && x < 160 && y >= 180 && y < 200)
				{
					playercolor[DPLAYER] = SBLACK;
					playercolor[AI] = SWHITE;
					continue;
				}
				if (x >= 140 && x < 160 && y >= 220 && y < 240)
				{
					game_level = 0;
					continue;
				}
				if (x >= 140 && x < 160 && y >= 240 && y < 260)
				{
					game_level = 1;
					continue;
				}
				if (x >= 140 && x < 160 && y >= 260 && y < 280)
				{
					game_level = 2;
					continue;
				}
				if (x >= 140 && x < 160 && y >= 280 && y < 300)
				{
					game_level = 3;
					continue;
				}
				if (x >= 100 && x < 125 && y >= 300 && y < 325)
				{
					gamestate = 1;
					continue;
				}
			}
			if (gamestate == 1 && !board.GetGameState())
			{
				if (x >= SQUARESIZE * 14 && x < SQUARESIZE * 15 &&
					y >= HALFSQUARESIZE && y < HALFSQUARESIZE + SQUARESIZE)
				{
					if (board.GetSide() == playercolor[DPLAYER])
					{
						if (ml.size() > 0 && keuze > 1)
						{
							keuze -= 1;
						}
					}
				}
				if (x >= SQUARESIZE * 14 && x < SQUARESIZE * 15 &&
					y >= HALFSQUARESIZE + SQUARESIZE && y < HALFSQUARESIZE + SQUARESIZE * 2)
				{
					if (board.GetSide() == playercolor[DPLAYER])
					{
						if (ml.size() > 0 && keuze < ml.size())
						{
							keuze += 1;
						}
					}
				}
				if (x >= SQUARESIZE * 14 && x < SQUARESIZE * 16 &&
					y >= HALFSQUARESIZE + SQUARESIZE * 2 && y < HALFSQUARESIZE + SQUARESIZE * 3)
				{
					if (board.GetSide() == playercolor[DPLAYER])
					{
						if (ml.size() > 0)
						{
							Move m = ml[keuze - 1];
							game_moves.push_back(m);
							board.DoMove(board.GetSide(), m);
							keuze = 1;
							board.SetSide(board.GetSide() ^ 1);
							ml = board.Generate(board.GetSide());
							taskrunning = false;
						}
						else
						{
							board.SetGameState(true);
						}
					}
				}
				if (x >= SQUARESIZE * 14 && x < SQUARESIZE * 15 &&
					y >= SQUARESIZE * 4 && y < SQUARESIZE * 5)
				{
					board.SetGameState(true);
				}
				continue;
			}
		}

		// Ai Move
		if (board.GetSide() == playercolor[AI] && gamestate == 1 && guiteller % 10 == 0)
		{
			if (ml.size() == 0)
			{
				board.SetGameState(true);
			}
			else
			{
				max_depth = 6 + game_level * 2;
				Board b = Board(board);
				nodes = 0;

				if (!taskrunning)
				{
					// task(&b);
					std::thread t(task, &b);
					t.join();
				}
				if (taskready)
				{
					game_moves.push_back(bestmove);
					board.DoMove(board.GetSide(), bestmove);
					keuze = 1;
					board.SetSide(board.GetSide() ^ 1);
					ml = board.Generate(board.GetSide());
					taskready = false;
				}
			}
		}
	}

	// clean up
	UnloadTexture(enterbutton);
	UnloadTexture(arrowup);
	UnloadTexture(arrowdown);
	UnloadTexture(plrx);
	CloseWindow();
	return 0;
}

// ************************** Board *******************************************
Board::Board()
{
	for (int i = 0; i < 51; ++i)
	{
		DraughtsBoard[i] = EMPTY;
	}
}

Board::Board(const Board &obj)
{
	for (int i = 0; i < 51; ++i)
	{
		DraughtsBoard[i] = obj.DraughtsBoard[i];
	}
}

void Board::InitBoard()
{
	DraughtsBoard[0] = EDGE;
	for (int i = 1; i <= 20; ++i)
	{
		DraughtsBoard[i] = PAWN_BLACK;
		if (i <= 10)
		{
			DraughtsBoard[i + 20] = EMPTY;
		}
		DraughtsBoard[i + 30] = PAWN_WHITE;
	}
	side2move = SWHITE;
	gameover = false;
	nodes = 0;
}

int Board::Evaluate()
{
	int evalB = 0;
	int evalW = 0;
	for (int i = 1; i <= 50; ++i)
	{
		int row = ROW[i];
		int col = COL[i];
		char piece = GetPiece(i);
		switch (piece)
		{
		case PAWN_BLACK:
			evalB += EVAL_PAWN + PST_ROW[row] + PST_COL[col];
			break;
		case KING_BLACK:
			evalB += EVAL_KING;
			break;
		case PAWN_WHITE:
			evalW += EVAL_PAWN + PST_ROW[9 - row] + PST_COL[col];
			break;
		case KING_WHITE:
			evalW += EVAL_KING;
			break;
		}
	}
	if (evalB == 0)
	{
		return -MAXEVAL + evalW;
	}
	if (evalW == 0)
	{
		return MAXEVAL - evalB;
	}
	if (playercolor[AI] == SBLACK)
	{
		return evalB - evalW;
	}
	else
	{
		return evalW - evalB;
	}
}

MoveList Board::Generate(int side)
{
	int mindir = side == SWHITE ? 0 : 2;
	int maxdir = side == SWHITE ? 1 : 3;
	bool captureMove = false;
	MoveList ml = MoveList();
	for (int i = 1; i <= 50; ++i)
	{
		if (side == SWHITE)
		{
			if (GetPiece(i) == KING_WHITE)
			{
				for (int j = 0; j < 4; ++j)
				{
					int sqf = i;
					int sqt = DIR[j][i];
					while (GetPiece(sqt) == EMPTY)
					{
						// normal move
						if (!captureMove)
						{
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt));
							ml.push_back(m);
						}
						sqt = DIR[j][sqt];
					}
					if (GetPiece(sqt) == 'p' || GetPiece(sqt) == 'k')
					{
						int sqo = sqt;
						sqt = DIR[j][sqo];
						while (GetPiece(sqt) == EMPTY)
						{
							if (!captureMove)
							{
								ml.clear();
								captureMove = true;
							}
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt), sqo, GetPiece(sqo));
							ml.push_back(m);
							sqt = DIR[j][sqt];
						}
					}
					else
					{
						continue;
					}
				}
			}
			else if (GetPiece(i) == PAWN_WHITE)
			{
				for (int j = 0; j < 4; ++j)
				{
					int sqf = i;
					int sqt = DIR[j][i];
					if (GetPiece(sqt) == 'p' || GetPiece(sqt) == 'k')
					{
						int sqo = sqt;
						sqt = DIR[j][sqo];
						if (GetPiece(sqt) == EMPTY)
						{
							// capture
							if (!captureMove)
							{
								ml.clear();
								captureMove = true;
							}
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt), sqo, GetPiece(sqo));
							ml.push_back(m);
						}
						else
						{
							continue;
						}
					}
					else if (GetPiece(sqt) == EMPTY && !captureMove && j >= mindir && j <= maxdir)
					{
						// normal move
						Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt));
						ml.push_back(m);
					}
					else
					{
						continue;
					}
				}
			}
		}
		else // side == black
		{
			if (GetPiece(i) == KING_BLACK)
			{
				for (int j = 0; j < 4; ++j)
				{
					int sqf = i;
					int sqt = DIR[j][i];
					while (GetPiece(sqt) == EMPTY)
					{
						// normal move
						if (!captureMove)
						{
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt));
							ml.push_back(m);
						}
						sqt = DIR[j][sqt];
					}
					if (GetPiece(sqt) == 'P' || GetPiece(sqt) == 'K')
					{
						int sqo = sqt;
						sqt = DIR[j][sqo];
						while (GetPiece(sqt) == EMPTY)
						{
							if (!captureMove)
							{
								ml.clear();
								captureMove = true;
							}
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt), sqo, GetPiece(sqo));
							ml.push_back(m);
							sqt = DIR[j][sqt];
						}
					}
					else
					{
						continue;
					}
				}
			}
			else if (GetPiece(i) == PAWN_BLACK)
			{
				for (int j = 0; j < 4; ++j)
				{
					int sqf = i;
					int sqt = DIR[j][i];
					if (GetPiece(sqt) == 'P' || GetPiece(sqt) == 'K')
					{
						int sqo = sqt;
						sqt = DIR[j][sqo];
						if (GetPiece(sqt) == EMPTY)
						{
							// capture
							if (!captureMove)
							{
								ml.clear();
								captureMove = true;
							}
							Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt), sqo, GetPiece(sqo));
							ml.push_back(m);
						}
						else
						{
							continue;
						}
					}
					else if (GetPiece(sqt) == EMPTY && !captureMove && j >= mindir && j <= maxdir)
					{
						// normal move
						Move m = Move(sqf, GetPiece(sqf), sqt, GetPiece(sqt));
						ml.push_back(m);
					}
					else
					{
						continue;
					}
				}
			}
		}
	}

	return (captureMove ? Generate_next_captures(side, ml) : ml);
}

MoveList Board::Generate_next_captures(int side, MoveList old)
{
	bool captureMove = false;
	MoveList ml = MoveList();
	for (auto m : old)
	{
		int sqf = m.GetPosition(SQT);
		char piece = GetPiece(m.GetPosition(SQF));
		if (piece == 'K')
		{
			for (int j = 0; j < 4; ++j)
			{
				int sqt = DIR[j][sqf];
				while (GetPiece(sqt) == EMPTY)
				{
					sqt = DIR[j][sqt];
				}
				if ((GetPiece(sqt) == 'p' || GetPiece(sqt) == 'k') && m.Search(sqt) == -1)
				{
					int sqo = sqt;
					sqt = DIR[j][sqo];
					while (GetPiece(sqt) == EMPTY)
					{
						if (!captureMove)
						{
							ml.clear();
							captureMove = true;
						}
						Move nm = Move(m);
						nm.AddCapture(sqt, GetPiece(sqt), sqo, GetPiece(sqo));
						ml.push_back(nm);
						sqt = DIR[j][sqt];
					}
				}
				else
				{
					continue;
				}
			}
		}
		else if (piece == 'P')
		{
			for (int j = 0; j < 4; ++j)
			{
				int sqt = DIR[j][sqf];
				if ((GetPiece(sqt) == 'p' || GetPiece(sqt) == 'k') && m.Search(sqt) == -1)
				{
					int sqo = sqt;
					sqt = DIR[j][sqo];
					if (GetPiece(sqt) == EMPTY)
					{
						// capture
						if (!captureMove)
						{
							ml.clear();
							captureMove = true;
						}
						Move nm = Move(m);
						nm.AddCapture(sqt, GetPiece(sqt), sqo, GetPiece(sqo));
						ml.push_back(nm);
					}
					else
					{
						continue;
					}
				}
			}
		}
		else if (piece == 'k')
		{
			for (int j = 0; j < 4; ++j)
			{
				int sqt = DIR[j][sqf];
				while (GetPiece(sqt) == EMPTY)
				{
					sqt = DIR[j][sqt];
				}
				if ((GetPiece(sqt) == 'P' || GetPiece(sqt) == 'K') && m.Search(sqt) == -1)
				{
					int sqo = sqt;
					sqt = DIR[j][sqo];
					while (GetPiece(sqt) == EMPTY)
					{
						if (!captureMove)
						{
							ml.clear();
							captureMove = true;
						}
						Move nm = Move(m);
						nm.AddCapture(sqt, GetPiece(sqt), sqo, GetPiece(sqo));
						ml.push_back(nm);
						sqt = DIR[j][sqt];
					}
				}
				else
				{
					continue;
				}
			}
		}
		else if (piece == 'p')
		{
			for (int j = 0; j < 4; ++j)
			{
				int sqt = DIR[j][sqf];
				if ((GetPiece(sqt) == 'P' || GetPiece(sqt) == 'K') && m.Search(sqt) == -1)
				{
					int sqo = sqt;
					sqt = DIR[j][sqo];
					if (GetPiece(sqt) == EMPTY)
					{
						// capture
						if (!captureMove)
						{
							ml.clear();
							captureMove = true;
						}
						Move nm = Move(m);
						nm.AddCapture(sqt, GetPiece(sqt), sqo, GetPiece(sqo));
						ml.push_back(nm);
					}
					else
					{
						continue;
					}
				}
			}
		}
	}

	return (captureMove ? Generate_next_captures(side, ml) : old);
}

void Board::DoMove(int side, Move m)
{
	// do step
	DraughtsBoard[m.GetPosition(SQT)] = DraughtsBoard[m.GetPosition(SQF)];
	DraughtsBoard[m.GetPosition(SQF)] = EMPTY;
	// remove captured pieces
	if (m.GetSize() > 0)
	{
		for (int i = 1; i <= m.GetSize(); ++i)
		{
			DraughtsBoard[m.GetPosition(SQT + i)] = EMPTY;
		}
	}
	// Promotion ?
	if ((m.GetPosition(SQT) <= 5 && side == SWHITE))
	{
		DraughtsBoard[m.GetPosition(SQT)] = 'K';
	}
	if (m.GetPosition(SQT) >= 46 && side == SBLACK)
	{
		DraughtsBoard[m.GetPosition(SQT)] = 'k';
	}
}

void Board::UndoMove(Move m)
{
	// correction for promote
	DraughtsBoard[m.GetPosition(SQT)] = m.GetPiece(SQF);
	// undo step
	DraughtsBoard[m.GetPosition(SQF)] = DraughtsBoard[m.GetPosition(SQT)];
	DraughtsBoard[m.GetPosition(SQT)] = EMPTY;
	// put captured pieces back
	if (m.GetSize() > 0)
	{
		for (int i = 1; i <= m.GetSize(); ++i)
		{
			DraughtsBoard[m.GetPosition(SQT + i)] = m.GetPiece(SQT + i);
		}
	}
}

int Board::BestEval(int depth, int alpha, int beta)
{
	int tpAlpha, tpBeta;
	int tpEval;
	bool blReached;
	bool blFirst = true;
	MoveList s;
	if (depth == max_depth)
	{
		return Evaluate();
	}
	else
	{
		++nodes;
		if ((depth % 2) == 0)
		{
			// black to move
			s = Generate(playercolor[AI]);
			if (s.size() == 0)
			{
				// black looses, has no moves possible
				return -MAXEVAL;
			}
			else
			{
				// BestEval => Max from all evals between alpha and beta
				// when eval > beta then ready
				tpAlpha = alpha;
				tpBeta = beta;
				blReached = false; // new alpha reached?
				for (auto m : s)
				{
					if (blFirst)
					{
						if ((depth == 0) && s.size() == 1)
						{
							// only one move possible
							bestmove = Move(m);
							bestval = 0;
							return 0;
						}
						blFirst = false;
					}
					DoMove(playercolor[AI], m);
					tpEval = BestEval(depth + 1, tpAlpha, tpBeta);
					UndoMove(m);
					if (tpEval > beta)
					{
						return MAXEVAL;
					}
					else
					{
						if (tpEval >= tpAlpha)
						{
							// better alpha found
							tpAlpha = tpEval;
#ifndef NDEBUG
							std::cout << "--- New alpha found: " << tpAlpha << " " << m.ToString() << " depth: " << depth << std::endl;
#endif
							blReached = true;
							if (depth == 0)
							{
								bestmove = Move(m);
								bestval = tpAlpha;
							}
						}
					}
				}
				if (blReached)
				{
					return tpAlpha;
				}
				else
				{
					return -MAXEVAL;
				}
			}
		}
		else
		{
			// White to move
			s = Generate(playercolor[DPLAYER]);
			if (s.size() == 0)
			{
				// White looses
				return MAXEVAL;
			}
			else
			{
				// BestEval => Min form all evals between alpha and beta
				// when eval < alpha then ready
				tpAlpha = alpha;
				tpBeta = beta;
				blReached = false; // new beta found
				for (auto m : s)
				{
					DoMove(playercolor[DPLAYER], m);
					tpEval = BestEval(depth + 1, tpAlpha, tpBeta);
					UndoMove(m);
					if (tpEval < alpha)
					{
						return -MAXEVAL;
					}
					else
					{
						if (tpEval <= tpBeta)
						{
							// better beta found
							tpBeta = tpEval;
#ifndef NDEBUG
							std::cout << "New beta found: " << tpBeta << " " << m.ToString() << " depth: " << depth << std::endl;
#endif

							blReached = true;
						}
					}
				}
				if (blReached)
				{
					return tpBeta;
				}
				else
				{
					return MAXEVAL;
				}
			}
		}
	}
}

int Board::GetCount(int side)
{
	int cnt = 0;

	for (int i = 1; i <= 50; ++i)
	{
		if (side == SWHITE)
		{
			if (DraughtsBoard[i] == PAWN_WHITE || DraughtsBoard[i] == KING_WHITE)
			{
				++cnt;
			}
		}
		if (side == SBLACK)
		{
			if (DraughtsBoard[i] == PAWN_BLACK || DraughtsBoard[i] == KING_BLACK)
			{
				++cnt;
			}
		}
	}
	return cnt;
}

// ************************** end Board ***************************************

// ************************** Move ********************************************

Move::Move()
{
	for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			MoveData[i][j] = EMPTY;
		}
	}
	Step = 0;
}

Move::Move(int sqf, char pcf, int sqt, char pct)
{
	for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			MoveData[i][j] = EMPTY;
		}
	}
	MoveData[SQF][SQR] = (char)sqf;
	MoveData[SQF][PSQ] = pcf;
	MoveData[SQT][SQR] = (char)sqt;
	MoveData[SQT][PSQ] = pct;
	Step = 0;
}

Move::Move(int sqf, char pcf, int sqt, char pct, int sqo, char pco)
{
	for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			MoveData[i][j] = EMPTY;
		}
	}
	MoveData[SQF][SQR] = (char)sqf;
	MoveData[SQF][PSQ] = pcf;
	MoveData[SQT][SQR] = (char)sqt;
	MoveData[SQT][PSQ] = pct;
	Step = 1;
	MoveData[SQT + Step][SQR] = (char)sqo;
	MoveData[SQT + Step][PSQ] = pco;
}

Move::Move(const Move &obj)
{
	for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			MoveData[i][j] = obj.MoveData[i][j];
		}
	}
	Step = obj.Step;
}

void Move::AddCapture(int sqt, char pct, int sqo, char pco)
{
	MoveData[SQT][SQR] = (char)sqt;
	MoveData[SQT][PSQ] = pct;
	++Step;
	MoveData[SQT + Step][SQR] = (char)sqo;
	MoveData[SQT + Step][PSQ] = pco;
}

int Move::Search(int sqr)
{
	for (int i = 1; i <= Step; ++i)
	{
		if (MoveData[SQT + i][SQR] == sqr)
		{
			return i;
		}
	}
	return -1;
}

std::string Move::ToString()
{
	if (Step > 0)
	{
		return BOARDTEXT[MoveData[SQF][SQR]] + " x " + BOARDTEXT[MoveData[SQT][SQR]];
	}
	return BOARDTEXT[MoveData[SQF][SQR]] + " - " + BOARDTEXT[MoveData[SQT][SQR]];
}

// ************************** end Move ****************************************
