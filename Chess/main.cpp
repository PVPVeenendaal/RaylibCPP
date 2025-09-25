#include <iostream>
#include "raylib.h"
#include <string>
#include "board.h"
#include "bitboard.h"

// version
const std::string version = "0.60";
const std::string title = "Chess in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
const std::string pieces[12] = {
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

std::string square_to_coordinates[] = {
	"a8",
	"b8",
	"c8",
	"d8",
	"e8",
	"f8",
	"g8",
	"h8",
	"a7",
	"b7",
	"c7",
	"d7",
	"e7",
	"f7",
	"g7",
	"h7",
	"a6",
	"b6",
	"c6",
	"d6",
	"e6",
	"f6",
	"g6",
	"h6",
	"a5",
	"b5",
	"c5",
	"d5",
	"e5",
	"f5",
	"g5",
	"h5",
	"a4",
	"b4",
	"c4",
	"d4",
	"e4",
	"f4",
	"g4",
	"h4",
	"a3",
	"b3",
	"c3",
	"d3",
	"e3",
	"f3",
	"g3",
	"h3",
	"a2",
	"b2",
	"c2",
	"d2",
	"e2",
	"f2",
	"g2",
	"h2",
	"a1",
	"b1",
	"c1",
	"d1",
	"e1",
	"f1",
	"g1",
	"h1",
	"out"};

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
	Board *brdobj = new Board();
	int selectpiece = -1;
	int selectsquare = -1;
	int promotionmove = 0;
	U64 canmove;
	U64 options;
	// initialize
	InitWindow(SCREENWIDTH, SCREENHEIGHT, title.c_str());
	Texture2D table = LoadTexture("./assets/Table.png");
	table.width = SCREENWIDTH;
	table.height = SCREENHEIGHT;
	Texture2D board = LoadTexture("./assets/Board.png");
	board.width = BOARDSIZE;
	board.height = BOARDSIZE;
	Texture2D img_pieces[12];
	for (int i = 0; i < 12; ++i)
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
			DRAWBOARD[i] = brdobj->GetPiece(i);
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
				int sqr = y * 8 + x;
				if (get_bit(brdobj->GetChessBoard()->all_options[brdobj->GetChessBoard()->side], sqr))
				{
					DrawRectangleLines(
						x * SQUARESIZE + 21,
						y * SQUARESIZE + 21,
						SQUARESIZE - 2,
						SQUARESIZE - 2,
						YELLOW);
					DrawRectangleLines(
						x * SQUARESIZE + 22,
						y * SQUARESIZE + 22,
						SQUARESIZE - 4,
						SQUARESIZE - 4,
						YELLOW);
				}
				int piece = DRAWBOARD[sqr];
				if (piece >= 0)
				{
					DrawTexture(
						img_pieces[piece],
						x * SQUARESIZE + 24,
						y * SQUARESIZE + 24,
						RAYWHITE);
					if ((piece == K && brdobj->GetChessBoard()->incheck[white]) ||
						(piece == k && brdobj->GetChessBoard()->incheck[black]))
					{
						DrawRectangleLines(
							x * SQUARESIZE + 21,
							y * SQUARESIZE + 21,
							SQUARESIZE - 2,
							SQUARESIZE - 2,
							RED);
						DrawRectangleLines(
							x * SQUARESIZE + 22,
							y * SQUARESIZE + 22,
							SQUARESIZE - 4,
							SQUARESIZE - 4,
							RED);
					}
				}
				if (selectpiece >= 0 && selectpiece <= 63)
				{
					if (get_bit(brdobj->GetChessBoard()->piece_options[selectpiece], sqr))
					{
						DrawRectangleLines(
							x * SQUARESIZE + 21,
							y * SQUARESIZE + 21,
							SQUARESIZE - 2,
							SQUARESIZE - 2,
							GREEN);
						DrawRectangleLines(
							x * SQUARESIZE + 22,
							y * SQUARESIZE + 22,
							SQUARESIZE - 4,
							SQUARESIZE - 4,
							GREEN);
					}
				}
			}
		}
		if (promotionmove)
		{
			for (int index = 0; index < 4; ++index)
			{
				int piece = promote_pieces[brdobj->GetChessBoard()->side][index];
				DrawTexture(
					img_pieces[piece],
					index * SQUARESIZE + 24,
					8 * SQUARESIZE + 24,
					RAYWHITE);
			}
			DrawText("Kies het promotie stuk", 5 * SQUARESIZE + 24, 8 * SQUARESIZE + 36, 20, YELLOW);
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
			int x = (int)(GetMouseX() - 24) / SQUARESIZE;
			int y = (int)(GetMouseY() - 24) / SQUARESIZE;
			int sqr = (x >= 0 && x <= 7 && y >= 0 && y <= 7) ? y * 8 + x : -1;
			int psqr = -1;
			if (promotionmove)
			{
				psqr = (x >= 0 & x <= 4) ? x : -1;
			}
#ifndef NDEBUG
			std::cout << "x = " << x << " y = " << y << " sqr = " << sqr << "pc = " << psqr << std::endl;
#endif
			if (sqr > -1)
			{
				if (get_bit(brdobj->GetChessBoard()->all_options[brdobj->GetChessBoard()->side], sqr))
				{
					selectpiece = sqr;
#ifndef NDEBUG
					std::cout << "selectpiece " << square_to_coordinates[selectpiece] << std::endl;
#endif
				}
				else if (selectpiece >= 0 && selectpiece <= 63 && get_bit(brdobj->GetChessBoard()->piece_options[selectpiece], sqr))
				{
					selectsquare = sqr;
#ifndef NDEBUG
					std::cout << "selectsquare " << square_to_coordinates[selectsquare] << std::endl;
#endif
					int move = brdobj->GetMove(selectpiece, selectsquare);
					if (move >= 0)
					{
						promotionmove = get_move_promoted(move);
						if (promotionmove == 0)
						{
							brdobj->DoMove(move);
							selectpiece = -1;
							selectsquare = -1;
						}
					}
				}
			} else if (psqr > -1)
			{
				int move = brdobj->GetPromotionMove(selectpiece, selectsquare, promote_pieces[brdobj->GetChessBoard()->side][psqr]);
				if (move >= 0)
				{
					brdobj->DoMove(move);
					selectpiece = -1;
					selectsquare = -1;
					promotionmove = 0;
				}
			}
		}
		// Ai Move
	}

	// clean up
	for (int i = 0; i < 12; ++i)
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
// BitBoard
// ------------------------------------------------------------------------------------------------

static inline int count_bits(U64 bitboard)
{
	int count = 0;

	while (bitboard)
	{
		++count;
		bitboard &= bitboard - 1;
	}

	return count;
}

static inline int get_ls1b_index(U64 bitboard)
{
	return (bitboard) ? count_bits((bitboard & -bitboard) - 1) : -1;
}

void print_bitboard(U64 bitboard)
{
	printf("\n");
	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			int square = rank * 8 + file;
			if (!file)
			{
				printf("  %d ", 8 - rank);
			}
			printf(" %d", get_bit(bitboard, square) ? 1 : 0);
		}
		printf("\n");
	}
	printf("\n     a b c d e f g h\n\n");
	printf("     Bitboard: %llud\n\n", bitboard);
}

// ------------------------------------------------------------------------------------------------
// End BitBoard
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Hash
// ------------------------------------------------------------------------------------------------

void init_random_keys(hash_data *gen)
{
	gen->random_state = 1804289383;

	// init random piece/square keys
	for (int piece = P; piece <= k; piece++)
	{
		for (int square = 0; square < 64; square++)
		{
			gen->piece_keys[piece][square] = get_random_U64_number(gen);
		}
	}
	// init random enpassant keys
	for (int square = 0; square < 64; square++)
	{
		gen->enpassant_keys[square] = get_random_U64_number(gen);
	}
	// init random castle keys
	for (int index = 0; index < 16; index++)
	{
		gen->castle_keys[index] = get_random_U64_number(gen);
	}
	// init random for side = black
	gen->side_key = get_random_U64_number(gen);
}

unsigned int get_random_U32_number(hash_data *gen)
{
	unsigned int number = gen->random_state;

	number ^= number << 13;
	number ^= number >> 17;
	number ^= number << 5;
	gen->random_state = number;

	return number;
}

U64 get_random_U64_number(hash_data *gen)
{
	U64 n1, n2, n3, n4;

	n1 = (U64)(get_random_U32_number(gen)) & 0xFFFF;
	n2 = (U64)(get_random_U32_number(gen)) & 0xFFFF;
	n3 = (U64)(get_random_U32_number(gen)) & 0xFFFF;
	n4 = (U64)(get_random_U32_number(gen)) & 0xFFFF;

	return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// ------------------------------------------------------------------------------------------------
// End Hash
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Board
// ------------------------------------------------------------------------------------------------

Board::Board()
{
	this->chsbrd = new chess_board();
	this->gen = new hash_data();
	this->list = new MoveList();
	init_random_keys(this->gen);
	Generate_move_tables();
	New_Game();
}

Board::~Board()
{
	delete this->chsbrd;
}

void Board::New_Game()
{
	Reset_board(this->chsbrd);
	for (int bb_piece = P; bb_piece <= k; ++bb_piece)
	{
		this->chsbrd->bitboards[bb_piece] = piece_bitboards[bb_piece];
	}
	Set_occupancies(this->chsbrd);
#ifndef NDEBUG
	for (int bb_piece = P; bb_piece <= k; ++bb_piece)
	{
		std::cout << "Piece " << ascii_pieces[bb_piece] << std::endl;
		print_bitboard(this->chsbrd->bitboards[bb_piece]);
	}
	for (int bb_o = white; bb_o <= both; ++bb_o)
	{
		std::cout << " " << ascii_occupancies[bb_o] << std::endl;
		print_bitboard(this->chsbrd->occupancies[bb_o]);
	}
#endif
	this->chsbrd->side = white;
	this->chsbrd->castle = wk | wq | bk | bq;
	this->chsbrd->enpassant = no_sq;
	this->chsbrd->hash_key = Generate_hash_key(this->gen);
	Generate_moves(this->list, this->chsbrd);
#ifndef NDEBUG
	Print_move_list(this->list);
#endif
}

int Board::GetPiece(int square)
{
	for (int i = P; i <= k; ++i)
	{
		if (get_bit(this->chsbrd->bitboards[i], square))
		{
			return i;
		}
	}
	return -1;
}

chess_board *Board::GetChessBoard()
{
	return this->chsbrd;
}

int Board::GetMove(int sqf, int sqt)
{
	if (this->list->size() == 0)
	{
		return -1;
	}
	int *p = this->list->data();
	for (int index = 0; index < this->list->size(); ++index)
	{
		int move = p[index];
		int sqfrom = get_move_source(move);
		int sqto = get_move_target(move);
		if (sqf == sqfrom && sqt == sqto)
		{
#ifndef NDEBUG
			std::cout << "Move found:";
			Print_move(move);
			std::cout << std::endl;
			return move;
#endif
		}
	}
	return -1; // not found
}

int Board::GetPromotionMove(int sqf, int sqt, int piece)
{
    if (this->list->size() == 0)
	{
		return -1;
	}
	int *p = this->list->data();
	for (int index = 0; index < this->list->size(); ++index)
	{
		int move = p[index];
		int sqfrom = get_move_source(move);
		int sqto = get_move_target(move);
		int promote = get_move_promoted(move);
		if (sqf == sqfrom && sqt == sqto && piece == promote)
		{
#ifndef NDEBUG
			std::cout << "Promotion move found:";
			Print_move(move);
			std::cout << std::endl;
			return move;
#endif
		}
	}
	return -1; // not found
}

void Board::DoMove(int move)
{
#ifndef NDEBUG
	std::cout << "Do move :";
	Print_move(move);
	std::cout << std::endl;
#endif
	Makemove(move, this->chsbrd);
	this->chsbrd->incheck[white] = IsKingInCheck(this->chsbrd, white);
	this->chsbrd->incheck[black] = IsKingInCheck(this->chsbrd, black);
	Generate_moves(this->list, this->chsbrd);
	if (this->list->size() == 0)
	{
		if (this->chsbrd->incheck[this->chsbrd->side])
		{
			// checkmate
			this->chsbrd->gameover = 1;
		}
		else
		{
			this->chsbrd->gameover = 2;
		}
	}
#ifndef NDEBUG
	Print_move_list(this->list);
#endif
}

void Board::Generate_move_tables()
{
	int sqf, sqt, cnt;
	// knight moves
#ifndef NDEBUG
	std::cout << "const int KNIGHT_MOVES[64][8] = {" << std::endl;
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
					if (dy == 0 || dx == 0 || abs(dy) == abs(dx))
						continue;
					if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
					{
						sqt = (y + dy) * 8 + x + dx;
					}
					else
					{
						sqt = no_sq;
					}
					knight_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << square_to_coordinates[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << square_to_coordinates[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};" << std::endl;
#endif
	// bishop moves
#ifndef NDEBUG
	std::cout << "const int BISHOP_MOVES[64][4] = {" << std::endl;
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
					{
						sqt = (y + dy) * 8 + x + dx;
					}
					else
					{
						sqt = no_sq;
					}
					bishop_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << square_to_coordinates[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << square_to_coordinates[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};" << std::endl;
#endif
	// rook moves
#ifndef NDEBUG
	std::cout << "const int ROOK_MOVES[64][4] = {" << std::endl;
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
					if (abs(dy) == abs(dx))
						continue;
					if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
					{
						sqt = (y + dy) * 8 + x + dx;
					}
					else
					{
						sqt = no_sq;
					}
					rook_moves[sqf][cnt++] = sqt;
#ifndef NDEBUG
					std::cout << square_to_coordinates[sqt] << ",";
#endif
				}
			}
#ifndef NDEBUG
			std::cout << "}, //" << square_to_coordinates[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};" << std::endl;
#endif
	// white pawn moves
#ifndef NDEBUG
	std::cout << "const int WHITE PAWN_MOVES[64][3] = {" << std::endl;
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
				if (y <= 7 && y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
				{
					sqt = (y + dy) * 8 + x + dx;
				}
				else
				{
					sqt = no_sq;
				}
				pawn_moves[sqf][white][cnt++] = sqt;
#ifndef NDEBUG
				std::cout << square_to_coordinates[sqt] << ",";
#endif
			}
#ifndef NDEBUG
			std::cout << "}, //" << square_to_coordinates[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};" << std::endl;
#endif
	// black pawn moves
#ifndef NDEBUG
	std::cout << "const int BLACK PAWN_MOVES[64][3] = {" << std::endl;
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
				if (y >= 0 && y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
				{
					sqt = (y + dy) * 8 + x + dx;
				}
				else
				{
					sqt = no_sq;
				}
				pawn_moves[sqf][black][cnt++] = sqt;
#ifndef NDEBUG
				std::cout << square_to_coordinates[sqt] << ",";
#endif
			}
#ifndef NDEBUG
			std::cout << "}, //" << square_to_coordinates[sqf] << std::endl;
#endif
		}
	}
#ifndef NDEBUG
	std::cout << "};" << std::endl;
#endif
}

void Board::Set_occupancies(chess_board *brd)
{
	memset(brd->occupancies, 0ULL, sizeof(brd->occupancies));
	for (int piece = P; piece <= K; ++piece)
	{
		brd->occupancies[white] |= brd->bitboards[piece];
	}
	for (int piece = p; piece <= k; ++piece)
	{
		brd->occupancies[black] |= brd->bitboards[piece];
	}
	brd->occupancies[both] |= brd->occupancies[white];
	brd->occupancies[both] |= brd->occupancies[black];
}

void Board::Reset_board(chess_board *brd)
{
	memset(brd->bitboards, 0ULL, sizeof(brd->bitboards));
	memset(brd->occupancies, 0ULL, sizeof(brd->occupancies));
	memset(brd->piece_options, 0ULL, sizeof(brd->piece_options));
	memset(brd->all_options, 0ULL, sizeof(brd->all_options));
	brd->side = white;
	brd->enpassant = no_sq;
	brd->castle = 0;
	brd->fifty = 0;
	brd->hash_key = 0ULL;
	brd->ply = 0;
	brd->incheck[white] = false;
	brd->incheck[black] = false;
}

chess_board *Board::Copy_board(chess_board *brd)
{
	chess_board *keep = new chess_board();

	memcpy(keep->bitboards, brd->bitboards, sizeof(brd->bitboards));
	memcpy(keep->occupancies, brd->occupancies, sizeof(brd->occupancies));
	memcpy(keep->piece_options, brd->piece_options, sizeof(brd->piece_options));
	memcpy(keep->all_options, brd->all_options, sizeof(brd->all_options));
	keep->side = brd->side;
	keep->enpassant = brd->enpassant;
	keep->castle = brd->castle;
	keep->fifty = brd->fifty;
	keep->hash_key = brd->hash_key;
	keep->ply = brd->ply;
	keep->incheck[white] = brd->incheck[white];
	keep->incheck[black] = brd->incheck[black];

	return keep;
}

U64 Board::Generate_hash_key(hash_data *gen)
{
	U64 final_key = 0ULL;
	U64 bitboard;

	for (int piece = P; piece <= k; ++piece)
	{
		bitboard = chsbrd->bitboards[piece];
		while (bitboard)
		{
			int square = get_ls1b_index(bitboard);
			final_key ^= gen->piece_keys[piece][square];
			pop_bit(bitboard, square);
		}
	}
	if (chsbrd->enpassant != no_sq)
	{
		final_key ^= gen->enpassant_keys[chsbrd->enpassant];
	}
	final_key ^= gen->castle_keys[chsbrd->castle];
	if (chsbrd->side == black)
	{
		final_key ^= gen->side_key;
	}

	return final_key;
}

bool Board::IsEmptySquare(int square, chess_board *brd)
{
	return get_bit(brd->occupancies[both], square) == 0ULL;
}

bool Board::IsOccupiedByOponent(int square, int side, chess_board *brd)
{
	int xside = side ^ 1;

	return get_bit(brd->occupancies[xside], square) > 0ULL;
}

bool Board::IsSquareAttacked(int square, int xside, chess_board *brd) // xside = attacker
{
	int sqf, attacker;
	int side = xside ^ 1;
	// attack by pawn
	attacker = xside == white ? P : p;
	for (int index = 0; index <= 2; index += 2)
	{
		sqf = pawn_moves[square][side][index];
		if (sqf < no_sq && get_bit(brd->bitboards[attacker], sqf))
		{
			return true;
		}
	}
	// attack by knight
	attacker = xside == white ? N : n;
	for (int index = 0; index < 8; ++index)
	{
		sqf = knight_moves[square][index];
		if (sqf < no_sq && get_bit(brd->bitboards[attacker], sqf))
		{
			return true;
		}
	}
	// attack by king
	attacker = xside == white ? K : k;
	for (int index = 0; index < 8; ++index)
	{
		sqf = (index < 4) ? bishop_moves[square][index] : rook_moves[square][index - 4];
		if (sqf < no_sq && get_bit(brd->bitboards[attacker], sqf))
		{
			return true;
		}
	}
	// attack by bishop
	attacker = xside == white ? B : b;
	for (int index = 0; index < 4; ++index)
	{
		sqf = square;
		while (true)
		{
			sqf = bishop_moves[sqf][index];
			if (sqf == no_sq)
			{
				break;
			}
			if (IsEmptySquare(sqf, brd))
			{
				continue;
			}
			if (get_bit(brd->bitboards[attacker], sqf))
			{
				return true;
			}
			else
			{
				break;
			}
		}
	}
	// attack by rook
	attacker = xside == white ? R : r;
	for (int index = 0; index < 4; ++index)
	{
		sqf = square;
		while (true)
		{
			sqf = rook_moves[sqf][index];
			if (sqf == no_sq)
			{
				break;
			}
			if (IsEmptySquare(sqf, brd))
			{
				continue;
			}
			if (get_bit(brd->bitboards[attacker], sqf))
			{
				return true;
			}
			else
			{
				break;
			}
		}
	}
	// attack by queen
	attacker = xside == white ? Q : q;
	for (int index = 0; index < 8; ++index)
	{
		sqf = square;
		while (true)
		{
			sqf = (index < 4) ? bishop_moves[sqf][index] : rook_moves[sqf][index - 4];
			if (sqf == no_sq)
			{
				break;
			}
			if (IsEmptySquare(sqf, brd))
			{
				continue;
			}
			if (get_bit(brd->bitboards[attacker], sqf))
			{
				return true;
			}
			else
			{
				break;
			}
		}
	}

	return false;
}

void Board::Generate_moves(MoveList *move_list, chess_board *brd)
{
	int sqf, sqt, move;
	U64 bitboard;
	move_list->clear();
	memset(brd->piece_options, 0ULL, sizeof(brd->piece_options));
	memset(brd->all_options, 0ULL, sizeof(brd->all_options));
	int promoterow = brd->side == white ? 0 : 7;
	int pawnrow = brd->side == white ? 6 : 1;
	bool is_promote;

	for (int piece = P; piece <= k; ++piece)
	{
		if ((brd->side == white && piece > K) || (brd->side == black && piece < p))
		{
			continue;
		}
		bitboard = brd->bitboards[piece];
		while (bitboard)
		{
			sqf = get_ls1b_index(bitboard);
			switch (piece)
			{
			case P:
			case p:
				// pawn move
				// regular move
				sqt = pawn_moves[sqf][brd->side][1];
				if (sqt < no_sq)
				{
					move = 0;
					is_promote = false;
					if (IsEmptySquare(sqt, brd))
					{
						if (sqrow(sqt) == promoterow)
						{
							is_promote = true;
							move = encode_move(sqf, sqt, piece, promote_pieces[brd->side][0], 0, 0, 0, 0);
						}
						else
						{
							is_promote = false;
							move = encode_move(sqf, sqt, piece, 0, 0, 0, 0, 0);
						}
					}
					if (move > 0)
					{
						if (Test_move(brd, move))
						{
							move_list->push_back(move);
							set_bit(brd->piece_options[sqf], sqt);
							set_bit(brd->all_options[brd->side], sqf);
							if (is_promote)
							{
								for (int i = 1; i <= 3; ++i)
								{
									move = encode_move(sqf, sqt, piece, promote_pieces[brd->side][i], 0, 0, 0, 0);
									move_list->push_back(move);
								}
							}
							// double move
							if (sqrow(sqf) == pawnrow)
							{
								sqt = pawn_moves[sqt][brd->side][1];
								int move = 0;
								if (IsEmptySquare(sqt, brd)) // always on the board, so test on no_sq is not needed
								{
									move = encode_move(sqf, sqt, piece, 0, 0, 1, 0, 0);
								}
								if (move > 0)
								{
									move_list->push_back(move);
									set_bit(brd->piece_options[sqf], sqt);
									set_bit(brd->all_options[brd->side], sqf);
								}
							}
						}
					}
				}
				// capture move
				for (int index = 0; index <= 2; index += 2)
				{
					sqt = pawn_moves[sqf][brd->side][index];
					if (sqt < no_sq)
					{
						move = 0;
						is_promote = false;
						if (IsOccupiedByOponent(sqt, brd->side, brd))
						{
							if (sqrow(sqt) == promoterow)
							{
								is_promote = true;
								move = encode_move(sqf, sqt, piece, promote_pieces[brd->side][0], 1, 0, 0, 0);
							}
							else
							{
								is_promote = false;
								move = encode_move(sqf, sqt, piece, 0, 1, 0, 0, 0);
							}
						}
						if (move > 0)
						{
							if (Test_move(brd, move))
							{
								move_list->push_back(move);
								set_bit(brd->piece_options[sqf], sqt);
								set_bit(brd->all_options[brd->side], sqf);
								if (is_promote)
								{
									for (int i = 1; i <= 3; ++i)
									{
										move = encode_move(sqf, sqt, piece, promote_pieces[brd->side][i], 1, 0, 0, 0);
										move_list->push_back(move);
									}
								}
							}
						}
						// enpassant capture
						if (sqt == brd->enpassant)
						{
							move = encode_move(sqf, sqt, piece, 0, 1, 0, 1, 0);
							if (Test_move(brd, move))
							{
								move_list->push_back(move);
								set_bit(brd->piece_options[sqf], sqt);
								set_bit(brd->all_options[brd->side], sqf);
							}
						}
					}
				}
				break;
			case N:
			case n:
				// knight move
				for (int index = 0; index < 8; ++index)
				{
					sqt = knight_moves[sqf][index];
					if (sqt < no_sq)
					{
						Leaper_piece_move(sqt, brd, sqf, piece, brd->side, move_list);
					}
				}
				break;
			case B:
			case b:
				// bishop move
				for (int index = 0; index < 4; ++index)
				{
					sqt = sqf;
					while (true)
					{
						bool is_capture = false;
						sqt = bishop_moves[sqt][index];
						if (sqt < no_sq)
						{
							Slider_piece_move(sqt, brd, sqf, piece, brd->side, is_capture, move_list);
							if (is_capture)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				break;
			case R:
			case r:
				// rook move
				for (int index = 0; index < 4; ++index)
				{
					sqt = sqf;
					while (true)
					{
						bool is_capture = false;
						sqt = rook_moves[sqt][index];
						if (sqt < no_sq)
						{
							Slider_piece_move(sqt, brd, sqf, piece, brd->side, is_capture, move_list);
							if (is_capture)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				break;
			case Q:
			case q:
				// queen move
				for (int index = 0; index < 8; ++index)
				{
					sqt = sqf;
					while (true)
					{
						bool is_capture = false;
						sqt = index < 4 ? bishop_moves[sqt][index] : rook_moves[sqt][index - 4];
						if (sqt < no_sq)
						{
							Slider_piece_move(sqt, brd, sqf, piece, brd->side, is_capture, move_list);
							if (is_capture)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				break;
			case K:
			case k:
				// king move
				for (int index = 0; index < 8; ++index)
				{
					sqt = index < 4 ? bishop_moves[sqf][index] : rook_moves[sqf][index - 4];
					if (sqt < no_sq)
					{
						Leaper_piece_move(sqt, brd, sqf, piece, brd->side, move_list);
					}
				}
				// castle move
				if (piece == K) // white king
				{
					// king side castling is available
					if (brd->castle & wk)
					{
						if (IsEmptySquare(f1, brd) && IsEmptySquare(g1, brd))
						{
							if (!IsSquareAttacked(e1, black, brd) && !IsSquareAttacked(f1, black, brd) && !IsSquareAttacked(g1, black, brd))
							{
								int move = encode_move(e1, g1, piece, 0, 0, 0, 0, 1);
								move_list->push_back(move);
								set_bit(brd->piece_options[e1], g1);
								set_bit(brd->all_options[brd->side], e1);
							}
						}
					}
					// queen side castling is available
					if (brd->castle & wq)
					{
						if (IsEmptySquare(d1, brd) && IsEmptySquare(c1, brd) && IsEmptySquare(b1, brd))
						{
							if (!IsSquareAttacked(e1, black, brd) && !IsSquareAttacked(d1, black, brd) && !IsSquareAttacked(c1, black, brd))
							{
								int move = encode_move(e1, c1, piece, 0, 0, 0, 0, 1);
								move_list->push_back(move);
								set_bit(brd->piece_options[e1], c1);
								set_bit(brd->all_options[brd->side], e1);
							}
						}
					}
				}
				if (piece == k) // black king
				{
					// king side castling is available
					if (brd->castle & bk)
					{
						if (IsEmptySquare(f8, brd) && IsEmptySquare(g8, brd))
						{
							if (!IsSquareAttacked(e8, white, brd) && !IsSquareAttacked(f8, white, brd) && !IsSquareAttacked(g8, white, brd))
							{
								int move = encode_move(e8, g8, piece, 0, 0, 0, 0, 1);
								move_list->push_back(move);
								set_bit(brd->piece_options[e8], g8);
								set_bit(brd->all_options[brd->side], e8);
							}
						}
					}
					// queen side castling is available
					if (brd->castle & bq)
					{
						if (IsEmptySquare(d8, brd) && IsEmptySquare(c8, brd) && IsEmptySquare(b8, brd))
						{
							if (!IsSquareAttacked(e8, white, brd) && !IsSquareAttacked(d8, white, brd) && !IsSquareAttacked(c8, white, brd))
							{
								int move = encode_move(e8, c8, piece, 0, 0, 0, 0, 1);
								move_list->push_back(move);
								set_bit(brd->piece_options[e8], c8);
								set_bit(brd->all_options[brd->side], e8);
							}
						}
					}
				}
				break;
			}
			pop_bit(bitboard, sqf);
		}
	}
}

void Board::Slider_piece_move(int &sqt, chess_board *brd, int &sqf, int &piece, int side, bool &is_capture, MoveList *move_list)
{
	int move = 0;
	if (IsEmptySquare(sqt, brd))
	{
		move = encode_move(sqf, sqt, piece, 0, 0, 0, 0, 0);
		is_capture = false;
	}
	else if (IsOccupiedByOponent(sqt, side, brd))
	{
		move = encode_move(sqf, sqt, piece, 0, 1, 0, 0, 0);
		is_capture = true;
	}
	if (move > 0)
	{
		if (Test_move(brd, move))
		{
			move_list->push_back(move);
			set_bit(brd->piece_options[sqf], sqt);
			set_bit(brd->all_options[side], sqf);
		}
	}
	else
	{
		is_capture = true; // own piece
	}
}

void Board::Leaper_piece_move(int &sqt, chess_board *brd, int &sqf, int &piece, int side, MoveList *move_list)
{
	int move = 0;
	if (IsEmptySquare(sqt, brd))
	{
		move = encode_move(sqf, sqt, piece, 0, 0, 0, 0, 0);
	}
	else if (IsOccupiedByOponent(sqt, side, brd))
	{
		move = encode_move(sqf, sqt, piece, 0, 1, 0, 0, 0);
	}
	if (move > 0)
	{
		if (Test_move(brd, move))
		{
			move_list->push_back(move);
			set_bit(brd->piece_options[sqf], sqt);
			set_bit(brd->all_options[side], sqf);
		}
	}
}

bool Board::Test_move(chess_board *brd, int move)
{
	chess_board *testbrd = Copy_board(brd);
	Makemove(move, testbrd);
	testbrd->side ^= 1;
	bool incheck = IsKingInCheck(testbrd, testbrd->side);
	delete testbrd;
	return !incheck;
}

void Board::Makemove(int move, chess_board *brd)
{
	int sqf = get_move_source(move);
	int sqt = get_move_target(move);
	int piece = get_move_piece(move);
	int promoted_piece = get_move_promoted(move);
	int capture = get_move_capture(move);
	int double_push = get_move_double(move);
	int enpass = get_move_enpassant(move);
	int castling = get_move_castling(move);

	// move piece
	pop_bit(brd->bitboards[piece], sqf);
	set_bit(brd->bitboards[piece], sqt);
	// hash piece
	brd->hash_key ^= gen->piece_keys[piece][sqf]; // remove piece from source square in hash key
	brd->hash_key ^= gen->piece_keys[piece][sqt]; // set piece to the target square in hash key
	brd->fifty++;								  // update fifty moves rule
	// if pawn moved reset fifty moves rule
	if (piece == P || piece == p)
	{
		brd->fifty = 0;
	}
	if (capture) // handling capture moves
	{
		brd->fifty = 0; // reset fifty move rule
		int start_piece, end_piece;
		if (brd->side == white) // white to move
		{
			start_piece = p;
			end_piece = k;
		}
		else // black to move
		{
			start_piece = P;
			end_piece = K;
		}
		for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
		{
			// if there's a piece on the target square
			if (get_bit(brd->bitboards[bb_piece], sqt))
			{
				// remove it from corresponding bitboard
				pop_bit(brd->bitboards[bb_piece], sqt);

				// remove the piece from hash key
				brd->hash_key ^= gen->piece_keys[bb_piece][sqt];
				break;
			}
		}
	}
	if (promoted_piece) // handle pawn promotions
	{
		if (brd->side == white) // white to move
		{
			pop_bit(brd->bitboards[P], sqt);		  // erase the pawn from the target square
			brd->hash_key ^= gen->piece_keys[P][sqt]; // remove pawn from hash key
		}
		else // black to move
		{
			pop_bit(brd->bitboards[p], sqt);		  // erase the pawn from the target square
			brd->hash_key ^= gen->piece_keys[p][sqt]; // remove pawn from hash key
		}
		set_bit(brd->bitboards[promoted_piece], sqt);		   // set up promoted piece on chess board
		brd->hash_key ^= gen->piece_keys[promoted_piece][sqt]; // add promoted piece into the hash key
	}
	// handle enpassant captures
	if (enpass)
	{
		if (brd->side == white) // white to move
		{
			pop_bit(brd->bitboards[p], sqt + 8);		  // remove captured pawn
			brd->hash_key ^= gen->piece_keys[p][sqt + 8]; // remove pawn from hash key
		}
		else // black to move
		{
			pop_bit(brd->bitboards[P], sqt - 8);		  // remove captured pawn
			brd->hash_key ^= gen->piece_keys[P][sqt - 8]; // remove pawn from hash key
		}
	}
	// hash enpassant if available (remove enpassant square from hash key )
	if (brd->enpassant != no_sq)
	{
		brd->hash_key ^= gen->enpassant_keys[brd->enpassant];
	}
	brd->enpassant = no_sq; // reset enpassant square
	// handle double pawn push
	if (double_push)
	{
		if (brd->side == white) // white to move
		{
			brd->enpassant = sqt + 8;					   // set enpassant square
			brd->hash_key ^= gen->enpassant_keys[sqt + 8]; // hash enpassant
		}
		else // black to move
		{
			brd->enpassant = sqt - 8;					   // set enpassant square
			brd->hash_key ^= gen->enpassant_keys[sqt - 8]; // hash enpassant
		}
	}
	// handle castling moves
	if (castling)
	{
		switch (sqt) // switch target square
		{
		case (g1): // white castles king side
			// move H rook
			pop_bit(brd->bitboards[R], h1);
			set_bit(brd->bitboards[R], f1);
			// hash rook
			brd->hash_key ^= gen->piece_keys[R][h1]; // remove rook from h1 from hash key
			brd->hash_key ^= gen->piece_keys[R][f1]; // put rook on f1 into a hash key
			break;
		case (c1): // white castles queen side
			// move A rook
			pop_bit(brd->bitboards[R], a1);
			set_bit(brd->bitboards[R], d1);
			// hash rook
			brd->hash_key ^= gen->piece_keys[R][a1]; // remove rook from a1 from hash key
			brd->hash_key ^= gen->piece_keys[R][d1]; // put rook on d1 into a hash key
			break;
		case (g8): // black castles king side
			// move H rook
			pop_bit(brd->bitboards[r], h8);
			set_bit(brd->bitboards[r], f8);
			// hash rook
			brd->hash_key ^= gen->piece_keys[r][h8]; // remove rook from h8 from hash key
			brd->hash_key ^= gen->piece_keys[r][f8]; // put rook on f8 into a hash key
			break;
		case (c8): // black castles queen side
			// move A rook
			pop_bit(brd->bitboards[r], a8);
			set_bit(brd->bitboards[r], d8);
			// hash rook
			brd->hash_key ^= gen->piece_keys[r][a8]; // remove rook from a8 from hash key
			brd->hash_key ^= gen->piece_keys[r][d8]; // put rook on d8 into a hash key
			break;
		}
	}
	brd->hash_key ^= gen->castle_keys[brd->castle]; // remove hash castling
	// update castling rights
	brd->castle &= castling_rights[sqf];
	brd->castle &= castling_rights[sqt];
	brd->hash_key ^= gen->castle_keys[brd->castle]; // add hash castling
													// reset occupancies
	Set_occupancies(brd);
	brd->side ^= 1;
}

bool Board::IsKingInCheck(chess_board *brd, int side)
{
	int xside = side ^ 1;
	int king_position = side == white ? get_ls1b_index(brd->bitboards[K]) : get_ls1b_index(brd->bitboards[k]);
	brd->incheck[side] = IsSquareAttacked(king_position, xside, brd);
	return brd->incheck[side];
}

void Board::Print_move_list(MoveList *move_list)
{
	// do nothing on empty move list
	if (!move_list->size())
	{
		std::cout << std::endl
				  << "     No move in the move list!" << std::endl;
		return;
	}
	std::cout << "     move    piece     capture   double    enpass    castling" << std::endl
			  << std::endl;

	// loop over moves within a move list
	int *p = move_list->data();
	for (int index = 0; index < move_list->size(); ++index)
	{
		int move = p[index];
		const char *textfrom = square_to_coordinates[get_move_source(move)].c_str();
		const char *textto = square_to_coordinates[get_move_target(move)].c_str();
		// print move
		printf("      %s%s%c   %c         %d         %d         %d         %d\n",
			   textfrom,
			   textto,
			   get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
			   ascii_pieces[get_move_piece(move)],
			   get_move_capture(move) ? 1 : 0,
			   get_move_double(move) ? 1 : 0,
			   get_move_enpassant(move) ? 1 : 0,
			   get_move_castling(move) ? 1 : 0);
	}
	// print total number of moves
	printf("\n\n     Total number of moves: %zu\n\n", move_list->size());
}

void Board::Print_move(int move)
{
	if (get_move_promoted(move))
	{
		printf("%s%s%c", square_to_coordinates[get_move_source(move)].c_str(),
			   square_to_coordinates[get_move_target(move)].c_str(),
			   promoted_pieces[get_move_promoted(move)]);
	}
	else
	{
		printf("%s%s", square_to_coordinates[get_move_source(move)].c_str(),
			   square_to_coordinates[get_move_target(move)].c_str());
	}
}

// ------------------------------------------------------------------------------------------------
// End Board
// ------------------------------------------------------------------------------------------------
