#include <iostream>
#include "raylib.h"
#include <string>
#include "board.h"

// version
const std::string version = "0.20";
const std::string title = "Chess in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
const std::string pieces[13] = {
	"Choice",
	"PawnW",
	"KnightW",
	"BishopW",
	"RookW",
	"QueenW",
	"KingW",
	"PawnB",
	"KnightB",
	"BishopB",
	"RookB",
	"QueenB",
	"KingB",
};

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------

int main()
{
	const int SQUARESIZE = 80;
	const int BOARDSIZE = 80 * 8;
	const int SCREENWIDTH = BOARDSIZE + 40;
	const int SCREENHEIGHT = SQUARESIZE * 9 + 40;
	const int PIECESIZE = 72;
	int DRAWBOARD[64];
	Board brd = Board();
	// initialize
	InitWindow(SCREENWIDTH, SCREENHEIGHT, title.c_str());
	Texture2D table = LoadTexture("./assets/Table.png");
	table.width = SCREENWIDTH;
	table.height = SCREENHEIGHT;
	Texture2D board = LoadTexture("./assets/Board.png");
	board.width = BOARDSIZE;
	board.height = BOARDSIZE;
	Texture2D img_pieces[13];
	for (int i = 0; i < 13; ++i)
	{
		std::string txt = "./assets/" + pieces[i] + ".png";
		img_pieces[i] = LoadTexture(txt.c_str());
		img_pieces[i].width = PIECESIZE;
		img_pieces[i].height = PIECESIZE;
	}

	SetTargetFPS(10);

	// mainloop
	while (!WindowShouldClose())
	{
		// update
		for (int i = 0; i < 64; ++i)
		{
			DRAWBOARD[i] = brd.GetChessBoard(i);
		}

		// draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		// Draw table
		DrawTexture(
			table,
			0,
			0,
			RAYWHITE);
		// Draw board
		DrawRectangle(
			10,
			10,
			BOARDSIZE + 20,
			BOARDSIZE + 20,
			BROWN);
		DrawTexture(
			board,
			20,
			20,
			RAYWHITE);
		// Draw pieces
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				int p = DRAWBOARD[y * 8 + x];
				if (p >= 1 && p <= 12)
				{
					DrawTexture(
						img_pieces[p],
						x * SQUARESIZE + 24,
						y * SQUARESIZE + 24,
						RAYWHITE);
				}
			}
		}
		EndDrawing();

		// keypress
		if (IsKeyPressed(KEY_ENTER))
		{
		}
		else if (IsKeyPressed(KEY_F5))
		{
		}

		// Mouse Press
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			int x = GetMouseX();
			int y = GetMouseY();
		}

		// Ai Move
	}

	// clean up
	for (int i = 0; i < 13; ++i)
	{
		UnloadTexture(img_pieces[i]);
	}
	UnloadTexture(table);
	UnloadTexture(board);
	CloseWindow();
	return 0;
}

// ------------------------------------------------------------------------------------------------
// End Main
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Board
// ------------------------------------------------------------------------------------------------

Board::Board()
{
	const std::string SQUARENAMES[65] = {
		"A8",
		"B8",
		"C8",
		"D8",
		"E8",
		"F8",
		"G8",
		"H8",
		"A7",
		"B7",
		"C7",
		"D7",
		"E7",
		"F7",
		"G7",
		"H7",
		"A6",
		"B6",
		"C6",
		"D6",
		"E6",
		"F6",
		"G6",
		"H6",
		"A5",
		"B5",
		"C5",
		"D5",
		"E5",
		"F5",
		"G5",
		"H5",
		"A4",
		"B4",
		"C4",
		"D4",
		"E4",
		"F4",
		"G4",
		"H4",
		"A3",
		"B3",
		"C3",
		"D3",
		"E3",
		"F3",
		"G3",
		"H3",
		"A2",
		"B2",
		"C2",
		"D2",
		"E2",
		"F2",
		"G2",
		"H2",
		"A1",
		"B1",
		"C1",
		"D1",
		"E1",
		"F1",
		"G1",
		"H1",
		"EDGE",
	};

	// generate move tables
	int sqf, sqt, cnt;
	// knight moves
#ifndef NDEBUG
	std::cout << "const int KNIGHT_MOVES[64][8] = {\n";
#endif
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			sqf = y * 8 + x;
#ifndef NDEBUG
			std::cout << "{";
#endif
			cnt = 0;
			for (int dy = -2; dy <= 2; ++dy)
			{
				for (int dx = -2; dx <= 2; ++dx)
				{
					if (dy == 0 || dx == 0 || ABS(dy) == ABS(dx))
						continue;
					if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
						sqt = (y + dy) * 8 + x + dx;
					else
						sqt = EDGE;
					knight_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << SQUARENAMES[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << SQUARENAMES[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};\n";
#endif
	// bishop moves
#ifndef NDEBUG
	std::cout << "const int BISHOP_MOVES[64][4] = {\n";
#endif
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			sqf = y * 8 + x;
#ifndef NDEBUG
			std::cout << "{";
#endif
			cnt = 0;
			for (int dy = -1; dy <= 1; dy += 2)
			{
				for (int dx = -1; dx <= 1; dx += 2)
				{
					if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
						sqt = (y + dy) * 8 + x + dx;
					else
						sqt = EDGE;
					bishop_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << SQUARENAMES[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << SQUARENAMES[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};\n";
#endif
	// rook moves
#ifndef NDEBUG
	std::cout << "const int ROOK_MOVES[64][4] = {\n";
#endif
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			sqf = y * 8 + x;
#ifndef NDEBUG
			std::cout << "{";
#endif
			cnt = 0;
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dx = -1; dx <= 1; ++dx)
				{
					if (ABS(dy) == ABS(dx))
						continue;
					if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
						sqt = (y + dy) * 8 + x + dx;
					else
						sqt = EDGE;
					rook_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << SQUARENAMES[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << SQUARENAMES[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};\n";
#endif
	// white pawn moves
#ifndef NDEBUG
	std::cout << "const int WHITE PAWN_MOVES[64][4] = {\n";
#endif
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			sqf = y * 8 + x;
#ifndef NDEBUG
			std::cout << "{";
#endif
			int dy = -1;
			cnt = 0;
			for (int dx = -1; dx <= 1; ++dx)
			{
				if (y <= 6 && y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
					sqt = (y + dy) * 8 + x + dx;
				else
					sqt = EDGE;
				pawn_moves[sqf][SWHITE][cnt++] = sqt;
#ifndef NDEBUG
				std::cout << SQUARENAMES[sqt] << ",";
#endif
			}
			if (y == 6)
			{
				sqt = (y - 2) * 8 + x;
			}
			else
			{
				sqt = EDGE;
			}
			pawn_moves[sqf][SWHITE][3] = sqt;
#ifndef NDEBUG
			std::cout << SQUARENAMES[sqt] << ",";
			std::cout << "}, //" << SQUARENAMES[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};\n";
#endif
	// black pawn moves
#ifndef NDEBUG
	std::cout << "const int BLACK PAWN_MOVES[64][4] = {\n";
#endif
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			sqf = y * 8 + x;
#ifndef NDEBUG
			std::cout << "{";
#endif
			int dy = 1;
			cnt = 0;
			for (int dx = -1; dx <= 1; ++dx)
			{
				if (y >= 1 && y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
					sqt = (y + dy) * 8 + x + dx;
				else
					sqt = EDGE;
				pawn_moves[sqf][SBLACK][cnt++] = sqt;
#ifndef NDEBUG
				std::cout << SQUARENAMES[sqt] << ",";
#endif
			}
			if (y == 1)
			{
				sqt = (y + 2) * 8 + x;
			}
			else
			{
				sqt = EDGE;
			}
			pawn_moves[sqf][SBLACK][3] = sqt;
#ifndef NDEBUG
			std::cout << SQUARENAMES[sqt] << ",";
			std::cout << "}, //" << SQUARENAMES[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};\n";
#endif
	for (int i = 0; i < 32; ++i)
	{
		moves[i] = MoveList();
	}
	New_Game();
}

void Board::New_Game()
{
	const int TOPROW[8] = {
		Br, Bn, Bb, Bq, Bk, Bb, Bn, Br};
	const int BOTROW[8] = {
		WR, WN, WB, WQ, WK, WB, WN, WR};

	for (int i = 0; i < 64; ++i)
	{
		if (i < 8)
		{
			chess_board[i] = TOPROW[i];
		}
		else if (i < 16)
		{
			chess_board[i] = Bp;
		}
		else if (i < 48)
		{
			chess_board[i] = empty;
		}
		else if (i < 56)
		{
			chess_board[i] = WP;
		}
		else
		{
			chess_board[i] = BOTROW[i - 56];
		}
	}
	kingloc[SWHITE] = e1;
	kingloc[SBLACK] = e8;
	side2move = SWHITE;
	epsquare = -1;
	castle_options = WKS | WQS | BKS | BQS;
}

void Board::PawnMoves(int sqf, int piece, int *test_board)
{
	Move m;
	int king, sqt;

	if (piece == WP) // piece = white pawn
	{
		king = kingloc[SWHITE];
		// normal move
		sqt = pawn_moves[sqf][SWHITE][1];
		if (test_board[sqt] == empty)
		{
			if (TestMove(test_board, m, king, sqf, sqt, (NORMAL | PAWN_MOVE)))
			{
				if (SQROW(sqt) == WHITEPROMOTEROW)
				{
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
					for (int i = 0; i < 4; ++i)
					{
						m.promote = promotepieces[SWHITE][i];
						moves[ply].push_back(m);
					}
				}
				else
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
			if (SQROW(sqf) == WHITEPAWNROW)
			{
				sqt = pawn_moves[sqf][SWHITE][3];
				if (test_board[sqt] == empty && TestMove(test_board, m, king, sqf, sqt, (NORMAL | PAWN_MOVE | PUSH_PAWN_2_SQUARES)))
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
		}
		// captures
		for (int d = 0; d <= 2; d += 2)
		{
			sqt = pawn_moves[sqf][SWHITE][d];
			if (PCOLOR(test_board[sqt]) == SBLACK && TestMove(test_board, m, king, sqf, sqt, (CAPTURE | PAWN_MOVE)))
			{
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
				if (SQROW(sqt) == WHITEPROMOTEROW)
				{
					for (int i = 0; i < 4; ++i)
					{
						m.promote = promotepieces[SWHITE][i];
						moves[ply].push_back(m);
					}
				}
				else
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
			else if (sqt == epsquare)
			{
				if (TestMove(test_board, m, king, sqf, sqt, (CAPTURE | ENPASSANT_CAPTURE | PAWN_MOVE)))
				{
					// capture enpassant
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
		}
	}
	else // piece = black pawn
	{
		king = kingloc[SBLACK];
		// normal move
		sqt = pawn_moves[sqf][SBLACK][1];
		if (test_board[sqt] == empty)
		{
			if (TestMove(test_board, m, king, sqf, sqt, (NORMAL | PAWN_MOVE)))
			{
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
				if (SQROW(sqt) == BLACKPROMOTEROW)
				{
					for (int i = 0; i < 4; ++i)
					{
						m.promote = promotepieces[SBLACK][i];
						moves[ply].push_back(m);
					}
				}
				else
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
		}
		// captures
		for (int d = 0; d <= 2; d += 2)
		{
			sqt = pawn_moves[sqf][SBLACK][d];
			if (PCOLOR(test_board[sqt]) == SWHITE && TestMove(test_board, m, king, sqf, sqt, (CAPTURE | PAWN_MOVE)))
			{
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
				if (SQROW(sqt) == BLACKPROMOTEROW)
				{
					for (int i = 0; i < 4; ++i)
					{
						m.promote = promotepieces[SBLACK][i];
						moves[ply].push_back(m);
					}
				}
				else
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
			else if (sqt == epsquare)
			{
				if (TestMove(test_board, m, king, sqf, sqt, (CAPTURE | ENPASSANT_CAPTURE | PAWN_MOVE)))
				{
					// capture enpassant
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
		}
	}
}

void Board::KnightMoves(int sqf, int piece, int *test_board)
{
	Move m;
	int sqt, king, xside;
	king = kingloc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 8; d++)
	{
		sqt = knight_moves[sqf][d];
		if (test_board[sqt] == empty)
		{
			if (TestMove(test_board, m, king, sqf, sqt, NORMAL))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
		else if (PCOLOR(test_board[sqt]) == xside)
		{
			if (TestMove(test_board, m, king, sqf, sqt, CAPTURE))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
	}
}

void Board::BishopMoves(int sqf, int piece, int *test_board)
{
	Move m;
	int sqt, king, xside;
	king = kingloc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 4; d++)
	{
		for (sqt = sqf;;)
		{
			sqt = bishop_moves[sqt][d];
			if (test_board[sqt] == empty)
			{
				if (TestMove(test_board, m, king, sqf, sqt, NORMAL))
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
					if (piece != WK && piece != Bk)
					{
						continue;
					}
				}
			}
			if (PCOLOR(test_board[sqt]) == xside)
			{
				if (TestMove(test_board, m, king, sqf, sqt, CAPTURE))
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
			break;
		}
	}
}

void Board::RookMoves(int sqf, int piece, int *test_board)
{
	Move m;
	int sqt, king, xside;
	king = kingloc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 4; d++)
	{
		for (sqt = sqf;;)
		{
			sqt = rook_moves[sqt][d];
			if (test_board[sqt] == empty)
			{
				if (TestMove(test_board, m, king, sqf, sqt, NORMAL))
				{

					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
					if (piece != WK && piece != Bk)
					{
						continue;
					}
				}
			}
			if (PCOLOR(test_board[sqt]) == xside)
			{
				if (TestMove(test_board, m, king, sqf, sqt, CAPTURE))
				{
					moves[ply].push_back(m);
					SET_BIT(all_options[side2move], sqf);
					SET_BIT(piece_options[sqf], sqt);
				}
			}
			break;
		}
	}
}

void Board::QueenMoves(int sqf, int piece, int *test_board)
{
	BishopMoves(sqf, piece, test_board);
	RookMoves(sqf, piece, test_board);
}

void Board::KingMoves(int sqf, int piece, int *test_board)
{
	BishopMoves(sqf, piece, test_board);
	RookMoves(sqf, piece, test_board);
	CastleMoves(sqf, piece, test_board);
}

void Board::CastleMoves(int sqf, int piece, int *test_board)
{
	Move m;

	if (piece == WK && !incheck[SWHITE])
	{
		int sqf = e1;
		int sqt = g1;
		if (castle_options & WKS)
		{
			if (test_board[f1] == empty &&
				test_board[g1] == empty &&
				!AttackSquare(test_board, e1, SBLACK) &&
				!AttackSquare(test_board, f1, SBLACK) &&
				!AttackSquare(test_board, g1, SBLACK) &&
				TestMove(test_board, m, e1, e1, g1, CASTLE))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
		if (castle_options & WQS)
		{
			sqt = c1;
			if (test_board[b1] == empty &&
				test_board[c1] == empty &&
				test_board[d1] == empty &&
				!AttackSquare(test_board, e1, SBLACK) &&
				!AttackSquare(test_board, d1, SBLACK) &&
				!AttackSquare(test_board, c1, SBLACK) &&
				TestMove(test_board, m, e1, e1, c1, CASTLE))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
	}
	if (piece == Bk && !incheck[SBLACK])
	{
		int sqf = e8;
		int sqt = g8;
		if (castle_options & BKS)
		{
			if (test_board[f8] == empty &&
				test_board[g8] == empty &&
				!AttackSquare(test_board, e8, SBLACK) &&
				!AttackSquare(test_board, f8, SBLACK) &&
				!AttackSquare(test_board, g8, SBLACK) &&
				TestMove(test_board, m, e8, e8, g8, CASTLE))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
		if (castle_options & BQS)
		{
			sqt = c8;
			if (test_board[b8] == empty &&
				test_board[c8] == empty &&
				test_board[d8] == empty &&
				!AttackSquare(test_board, e8, SBLACK) &&
				!AttackSquare(test_board, d8, SBLACK) &&
				!AttackSquare(test_board, c8, SBLACK) &&
				TestMove(test_board, m, e8, e8, c8, CASTLE))
			{
				moves[ply].push_back(m);
				SET_BIT(all_options[side2move], sqf);
				SET_BIT(piece_options[sqf], sqt);
			}
		}
	}
}

void Board::MakeMove(int &sqf, int &sqt, Move &m, int *test_board, int tmove, int ppromote)
{
	m.sqf = (char)sqf;
	m.sqt = (char)sqt;
	m.piece_sqf = test_board[sqf];
	m.piece_sqt = test_board[sqt];
	m.movetype = (char)tmove;
	m.promote = ppromote;
}

bool Board::TestMove(int *test_board, Move &m, int king, int sqf, int sqt, int tmove)
{
	int side = PCOLOR(test_board[sqf]);
	int xside = side ^ 1;
	int rowto = side == SWHITE ? 0 : 7;
	int pp = side == SWHITE ? WQ : Bq;
	bool ispromote = false;

	if (tmove && PAWN_MOVE)
	{
		if (SQROW(sqt) == rowto)
		{
			MakeMove(sqf, sqt, m, test_board, tmove, pp);
			ispromote = true;
		}
	}
	else
	{
		MakeMove(sqf, sqt, m, test_board, tmove, 0);
	}
	DoMove(test_board, m);
	bool king_in_check = AttackSquare(test_board, king, xside);
	UnDoMove(test_board, m);

	return king_in_check;
}

void Board::DoMove(int *test_board, Move &m)
{
}

void Board::UnDoMove(int *test_board, Move &m)
{
}

bool Board::AttackSquare(int *test_board, int sqr, int attack_color)
{
	return false;
}

// ------------------------------------------------------------------------------------------------
// End Board
// ------------------------------------------------------------------------------------------------
