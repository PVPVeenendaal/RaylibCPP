#include <iostream>
#include "raylib.h"
#include <string>
#include "board.h"
#include "bitboard.h"
#include "evaluate.h"
#include "search.h"
#include "move.h"

// version
const std::string version = "0.74";
// title
const std::string title = "Chess in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
// name of the image pictures
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
// name of the squares
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
// gamestate
enum eGamestate
{
	StartGame,
	PlayGame,
	StopGame,
};
// text to print when the game is finished
const std::string text_game_end[8] = {
	"",
	"wit staat schaakmat en verliest",
	"zwart staat schaakmat en verliest",
	"wit staat pat, het is remise",
	"zwart staat pat, het is remise",
	"het is renise door 50 zetten regel",
	"het is remise door 3 zetten regel",
	"het is remise door materiaal"};

// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------

int main()
{
	// draw sizes
	const int SQUARESIZE = 80;
	const int BOARDSIZE = 80 * 8;
	const int SCREENWIDTH = BOARDSIZE + 40;
	const int SCREENHEIGHT = SQUARESIZE * 9 + 40;
	const int PIECESIZE = 72;

	int DRAWBOARD[64]; // used for printing the board in the gui
	Board *brdobj = new Board();
	Evaluate *evalobj = new Evaluate();
	int selectpiece = -1;	   // square from
	int selectsquare = -1;	   // square to
	int promotionmove = 0;	   // promotion choice
	U64 canmove = 0ULL;		   // bitboard all_options [side to move]
	U64 options = 0ULL;		   // bitboard piece_options[selectpiece]
	bool reversed = false;	   // play white (false) or black (true)
	int gamestate = StartGame; // current game state
	int game_end = 0;		   // > 0 => game is finished

	// initialize raylib
	InitWindow(SCREENWIDTH, SCREENHEIGHT, title.c_str());
	// load images and set sizes
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
	Texture2D choice = LoadTexture("./assets/Choice.png");
	choice.width = PIECESIZE;
	choice.height = PIECESIZE;
	// set frames per second
	SetTargetFPS(10);

	// mainloop
	while (!WindowShouldClose())
	{
		// update
		for (int i = 0; i < 64; ++i)
		{
			DRAWBOARD[i] = brdobj->GetPiece(i);
		}
		if (brdobj->GetChessBoard()->gameover)
		{
			gamestate = StopGame;
			game_end = brdobj->GetChessBoard()->gameover;
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
				if (reversed)
				{
					sqr = 63 - sqr;
				}
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
		if (gamestate == StartGame || gamestate == StopGame)
		{
			DrawTexture(
				choice,
				24,
				8 * SQUARESIZE + 44,
				RAYWHITE);
			DrawText("Kies kleur: F5 = Wit, F6 = Zwart", 24 + SQUARESIZE, 8 * SQUARESIZE + 56, 20, YELLOW);
		}
		if (gamestate == StopGame)
		{
			DrawText(text_game_end[game_end].c_str(), 24 + SQUARESIZE, 8 * SQUARESIZE + 36, 20, PURPLE);
		}
		EndDrawing();

		// keypress
		if (IsKeyPressed(KEY_ENTER))
		{
		}
		else if (IsKeyPressed(KEY_F5))
		{
			if (reversed)
			{
				reversed = false;
				brdobj->New_Game();
			}
			gamestate = PlayGame;
		}
		else if (IsKeyPressed(KEY_F6))
		{
			reversed = true;
			brdobj->New_Game();
			gamestate = PlayGame;
		}
		// Mouse Press
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (gamestate == StartGame || gamestate == StopGame)
			{
				continue;
			}
			int x = (int)(GetMouseX() - 24) / SQUARESIZE;
			int y = (int)(GetMouseY() - 24) / SQUARESIZE;
			int sqr = (x >= 0 && x <= 7 && y >= 0 && y <= 7) ? y * 8 + x : -1;
			if (reversed)
			{
				sqr = 63 - sqr;
			}
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
#ifndef NDEBUG
							brdobj->Print_board();
							int eval = evalobj->Evaluate_board_position(brdobj->GetChessBoard());
							std::cout << "Evaluation: " << eval << std::endl;
#endif
						}
					}
				}
			}
			else if (psqr > -1)
			{
				int move = brdobj->GetPromotionMove(selectpiece, selectsquare, promote_pieces[brdobj->GetChessBoard()->side][psqr]);
				if (move >= 0)
				{
					brdobj->DoMove(move);
					selectpiece = -1;
					selectsquare = -1;
					promotionmove = 0;
#ifndef NDEBUG
					brdobj->Print_board();
					int eval = evalobj->Evaluate_board_position(brdobj->GetChessBoard());
					std::cout << "Evaluation: " << eval << std::endl;
#endif
				}
			}
		}
		// Ai Move
	}

	// clean up
	delete brdobj;
	delete evalobj;
	for (int i = 0; i < 12; ++i)
	{
		UnloadTexture(img_pieces[i]);
	}
	UnloadTexture(choice);
	UnloadTexture(table);
	UnloadTexture(board);
	CloseWindow();
	return 0;
}

// ------------------------------------------------------------------------------------------------
// End Main
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Move
// ------------------------------------------------------------------------------------------------

/// @brief print the move
/// @param move current move
void Print_move(int move)
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

/// @brief print the movelist
/// @param move_list current movelist
void Print_move_list(MoveList *move_list)
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
	int *arr = move_list->data();
	for (int index = 0; index < move_list->size(); ++index)
	{
		int move = arr[index];
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

// ------------------------------------------------------------------------------------------------
// End Move
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// BitBoard
// ------------------------------------------------------------------------------------------------

/// @brief Count the bits set in a bitboard
/// @param bitboard 64 bits
/// @return count
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

/// @brief Get de first set bit in a bitboard translated to a square
/// @param bitboard 64 bits
/// @return square 0 .. 63
static inline int get_ls1b_index(U64 bitboard)
{
	return (bitboard) ? count_bits((bitboard & -bitboard) - 1) : -1;
}

/// @brief Print a bitboard
/// example all white pawns in a starting position
///
///  8  0 0 0 0 0 0 0 0
///  7  0 0 0 0 0 0 0 0
///  6  0 0 0 0 0 0 0 0
///  5  0 0 0 0 0 0 0 0
///  4  0 0 0 0 0 0 0 0
///  3  0 0 0 0 0 0 0 0
///  2  1 1 1 1 1 1 1 1
///  1  0 0 0 0 0 0 0 0
///
///	 a b c d e f g h
///
///	 Bitboard: 71776119061217280d
/// @param bitboard 64 bits
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

/// @brief Generate random values
/// @param gen pointer to generated values put in structure
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

/// @brief Generate a 32 bit random value
/// @param gen pointer to generated values put in structure
/// @return pseudo random 32 bit value
unsigned int get_random_U32_number(hash_data *gen)
{
	unsigned int number = gen->random_state;

	number ^= number << 13;
	number ^= number >> 17;
	number ^= number << 5;
	gen->random_state = number;

	return number;
}

/// @brief Generate a 64 bit random value
/// @param gen pointer to generated values put in structure
/// @return pseudo random 64 bit value
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

/// @brief Constructor
Board::Board()
{
	this->brd = new chess_board();
	this->gen = new hash_data();
	this->list = new MoveList();
	this->repetition_table = new Hash_data_table();
	init_random_keys(this->gen);
	Generate_move_tables();
	New_Game();
}

/// @brief Destructor
Board::~Board()
{
	delete this->brd;
	delete this->gen;
	delete this->list;
	delete this->repetition_table;
}

/// @brief Start a new game, initialize the structure chess_board
void Board::New_Game()
{
	Reset_board();
	for (int bb_piece = P; bb_piece <= k; ++bb_piece)
	{
		this->brd->bitboards[bb_piece] = piece_bitboards[bb_piece];
	}
	Set_occupancies();
#ifndef NDEBUG
	for (int bb_piece = P; bb_piece <= k; ++bb_piece)
	{
		std::cout << "Piece " << ascii_pieces[bb_piece] << std::endl;
		print_bitboard(this->brd->bitboards[bb_piece]);
	}
	for (int bb_o = white; bb_o <= both; ++bb_o)
	{
		std::cout << " " << ascii_occupancies[bb_o] << std::endl;
		print_bitboard(this->brd->occupancies[bb_o]);
	}
#endif
	this->brd->side = white;
	this->brd->castle = wk | wq | bk | bq;
	this->brd->enpassant = no_sq;
	this->brd->hash_key = Generate_hash_key();
	this->brd->gameover = 0;
	this->repetition_table->clear();
	Generate_moves(this->list, true);

#ifndef NDEBUG
	Print_move_list(this->list);
#endif
}

/// @brief Get a piece on a square used for drawing in the gui
/// @param square a8..h1 = 0..63
/// @return piece P..k = 0..11
int Board::GetPiece(int square)
{
	for (int i = P; i <= k; ++i)
	{
		if (get_bit(this->brd->bitboards[i], square))
		{
			return i;
		}
	}
	return -1;
}

/// @brief Get the property chess_board
/// @return pointer to chess_board
chess_board *Board::GetChessBoard()
{
	return this->brd;
}

/// @brief Look in the movelist for a move with te starting coordinate and the ending coordinate used in the gui
/// @param sqf starting coordinate a8..h1
/// @param sqt ending coordinate a8..h1
/// @return move
int Board::GetMove(int sqf, int sqt)
{
	if (this->list->size() == 0)
	{
		return -1;
	}
	int *arr = this->list->data();
	for (int index = 0; index < this->list->size(); ++index)
	{
		int move = arr[index];
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

/// @brief Look in the movelist for a move with te starting coordinate and the ending coordinate and the promoted piece used in the gui
/// @param sqf starting coordinate a8..h1
/// @param sqt ending coordinate a8..h1
/// @param piece promotion piece N, B, R, Q for white to move and n, b, r, q for black to move
/// @return move
int Board::GetPromotionMove(int sqf, int sqt, int piece)
{
	if (this->list->size() == 0)
	{
		return -1;
	}
	int *arr = this->list->data();
	for (int index = 0; index < this->list->size(); ++index)
	{
		int move = arr[index];
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

/// @brief Make the found move final in the gui
/// @param move found move
void Board::DoMove(int move)
{
#ifndef NDEBUG
	std::cout << "Do move :";
	Print_move(move);
	std::cout << std::endl;
#endif
	Makemove(move, all_moves);
	this->brd->incheck[white] = IsKingInCheck(white);
	this->brd->incheck[black] = IsKingInCheck(black);
	Generate_moves(this->list, true);
	this->repetition_table->push_back(this->brd->hash_key);
	if (Is_Repetition() == 3)
	{
		this->brd->gameover = 6;
	}
	if (this->brd->fifty >= 100)
	{
		this->brd->gameover = 5;
	}
	if (this->list->size() == 0)
	{
		if (this->brd->incheck[this->brd->side])
		{
			// checkmate
			this->brd->gameover = this->brd->side == white ? 1 : 2;
		}
		else
		{
			// stalemate
			this->brd->gameover = this->brd->side == white ? 3 : 4;
		}
	}
#ifndef NDEBUG
	Print_move_list(this->list);
#endif
}

/// @brief print the chess board in ascii mode
void Board::Print_board()
{
	printf("\n");
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			if (!file)
				printf("  %d ", 8 - rank);
			int piece = -1;

			for (int bb_piece = P; bb_piece <= k; bb_piece++) // loop over all piece bitboards
			{
				if (get_bit(brd->bitboards[bb_piece], square)) // if there is a piece on current square
				{
					piece = bb_piece;
				}
			}
			printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
		}
		printf("\n");
	}
	printf("\n     a b c d e f g h\n\n");
	printf("     Side:     %s\n", !brd->side ? "white" : "black");
	printf("     Enpassant:   %s\n", (brd->enpassant != no_sq) ? square_to_coordinates[brd->enpassant].c_str() : "no");
	printf("     Castling:  %c%c%c%c\n\n", (brd->castle & wk) ? 'K' : '-',
		   (brd->castle & wq) ? 'Q' : '-',
		   (brd->castle & bk) ? 'k' : '-',
		   (brd->castle & bq) ? 'q' : '-');
	printf("     Hash key:  %llx\n\n", brd->hash_key);
	printf("     Fifty move: %d\n\n", brd->fifty);
}

/// @brief Generate the move tables per piece
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

/// @brief Fill the bitboards for the occupancies white, black or both with the piece bitboards
void Board::Set_occupancies()
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

/// @brief Initialise the chess_board
void Board::Reset_board()
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

/// @brief fill the keep chess_board with the data from the current chess_board
/// @return hardcopy of the chessboard
chess_board *Board::Copy_board()
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

/// @brief fill the currrent chess_board with the data from the keep chess_board
/// @param keep = hardcopy of the original chessboard
void Board::Take_back(chess_board *keep)
{
	memcpy(brd->bitboards, keep->bitboards, sizeof(keep->bitboards));
	memcpy(brd->occupancies, keep->occupancies, sizeof(keep->occupancies));
	memcpy(brd->piece_options, keep->piece_options, sizeof(keep->piece_options));
	memcpy(brd->all_options, keep->all_options, sizeof(keep->all_options));
	brd->side = keep->side;
	brd->enpassant = keep->enpassant;
	brd->castle = keep->castle;
	brd->fifty = keep->fifty;
	brd->hash_key = keep->hash_key;
	brd->ply = keep->ply;
	brd->incheck[white] = keep->incheck[white];
	brd->incheck[black] = keep->incheck[black];
}

/// @brief Generate a 'unique' key for the current state of the chess_board
/// @return 64 bit 'unique' key
U64 Board::Generate_hash_key()
{
	U64 final_key = 0ULL;
	U64 bitboard;

	for (int piece = P; piece <= k; ++piece)
	{
		bitboard = brd->bitboards[piece];
		while (bitboard)
		{
			int square = get_ls1b_index(bitboard);
			final_key ^= gen->piece_keys[piece][square];
			pop_bit(bitboard, square);
		}
	}
	if (brd->enpassant != no_sq)
	{
		final_key ^= gen->enpassant_keys[brd->enpassant];
	}
	final_key ^= gen->castle_keys[brd->castle];
	if (brd->side == black)
	{
		final_key ^= gen->side_key;
	}

	return final_key;
}

/// @brief test if a square on the chess_board is an empty one
/// @param square a8..h1
/// @return true/false
bool Board::IsEmptySquare(int square)
{
	return get_bit(brd->occupancies[both], square) == 0ULL;
}

/// @brief test if a square is occupied by the other side
/// @param square a8..h1
/// @param side white or black
/// @return true/false
bool Board::IsOccupiedByOponent(int square, int side)
{
	int xside = side ^ 1;

	return get_bit(brd->occupancies[xside], square) > 0ULL;
}

/// @brief test if a square is attacked by the other side
/// @param square a8..h1
/// @param xside the other side = attacker
/// @return true/false
bool Board::IsSquareAttacked(int square, int xside)
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
			if (IsEmptySquare(sqf))
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
			if (IsEmptySquare(sqf))
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
			if (IsEmptySquare(sqf))
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

/// @brief Count the number of repetitions from the current state off the chess_board
/// @return number
int Board::Is_Repetition()
{
	int cnt = 0;
	U64 *arr = repetition_table->data();
	for (int index = 0; index < repetition_table->size(); ++index)
	{
		U64 key = arr[index];
		if (key == brd->hash_key)
		{
			++cnt;
		}
	}
	return cnt;
}

/// @brief Generate the moves into a movelist and fill the bitboards for the options
///        all-options : set the bit from the sqaure from if it is possible to move
///        piece-options : set the bit from the square to if it is possible to move
///        The chess board is presented by bitboards, one for each piece
///	   	   plus 3 bitboards for occupansies: white, black and both
/// 	   The posible moves are get from the move tables [squares][directions]
/// @param move_list List of moves
/// @param include test for valid move (gui) or not (search)
void Board::Generate_moves(MoveList *move_list, bool include)
{
	int sqf, sqt, move;
	bool fill;
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
			case P: // white pawn
			case p: // black pawn
				// pawn move
				// regular move
				sqt = pawn_moves[sqf][brd->side][1];
				if (sqt < no_sq)
				{
					move = 0;
					is_promote = false;
					if (IsEmptySquare(sqt))
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
						fill = false;
						if (include & Test_move(move))
						{
							fill = true;
						}
						else if (!include)
						{
							fill = true;
						}
						if (fill)
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
								if (IsEmptySquare(sqt)) // always on the board, so test on no_sq is not needed
								{
									move = encode_move(sqf, sqt, piece, 0, 0, 1, 0, 0);
								}
								if (move > 0)
								{
									fill = false;
									if (include & Test_move(move))
									{
										fill = true;
									}
									else if (!include)
									{
										fill = true;
									}
									if (fill)
									{
										move_list->push_back(move);
										set_bit(brd->piece_options[sqf], sqt);
										set_bit(brd->all_options[brd->side], sqf);
									}
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
						if (IsOccupiedByOponent(sqt, brd->side))
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
							fill = false;
							if (include & Test_move(move))
							{
								fill = true;
							}
							else if (!include)
							{
								fill = true;
							}
							if (fill)
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
							fill = false;
							if (include & Test_move(move))
							{
								fill = true;
							}
							else if (!include)
							{
								fill = true;
							}
							if (fill)
							{
								move_list->push_back(move);
								set_bit(brd->piece_options[sqf], sqt);
								set_bit(brd->all_options[brd->side], sqf);
							}
						}
					}
				}
				break;
			case N: // white night
			case n: // black night
				// knight move
				for (int index = 0; index < 8; ++index)
				{
					sqt = knight_moves[sqf][index];
					if (sqt < no_sq)
					{
						Leaper_piece_move(include, sqt, sqf, piece, brd->side, move_list);
					}
				}
				break;
			case B: // white bishop
			case b: // black bishop
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
							Slider_piece_move(include, sqt, sqf, piece, brd->side, is_capture, move_list);
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
			case R: // white rook
			case r: // black rook
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
							Slider_piece_move(include, sqt, sqf, piece, brd->side, is_capture, move_list);
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
			case Q: // white queen
			case q: // black queen
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
							Slider_piece_move(include, sqt, sqf, piece, brd->side, is_capture, move_list);
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
			case K: // white king
			case k: // black king
				// king move
				for (int index = 0; index < 8; ++index)
				{
					sqt = index < 4 ? bishop_moves[sqf][index] : rook_moves[sqf][index - 4];
					if (sqt < no_sq)
					{
						Leaper_piece_move(include, sqt, sqf, piece, brd->side, move_list);
					}
				}
				// castle move
				if (piece == K) // white king
				{
					// king side castling is available
					if (brd->castle & wk)
					{
						if (IsEmptySquare(f1) && IsEmptySquare(g1))
						{
							if (!IsSquareAttacked(e1, black) && !IsSquareAttacked(f1, black) && !IsSquareAttacked(g1, black))
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
						if (IsEmptySquare(d1) && IsEmptySquare(c1) && IsEmptySquare(b1))
						{
							if (!IsSquareAttacked(e1, black) && !IsSquareAttacked(d1, black) && !IsSquareAttacked(c1, black))
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
						if (IsEmptySquare(f8) && IsEmptySquare(g8))
						{
							if (!IsSquareAttacked(e8, white) && !IsSquareAttacked(f8, white) && !IsSquareAttacked(g8, white))
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
						if (IsEmptySquare(d8) && IsEmptySquare(c8) && IsEmptySquare(b8))
						{
							if (!IsSquareAttacked(e8, white) && !IsSquareAttacked(d8, white) && !IsSquareAttacked(c8, white))
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

/// @brief find next move of a slider piece => bishop, rook or queen
/// @param include test for valid move (gui) or not (search)
/// @param sqt square to
/// @param sqf square from
/// @param piece piece to move
/// @param side color to move
/// @param is_capture flag that indicates that the next move ended by capture or own piece
/// @param move_list pointer to the movelist
void Board::Slider_piece_move(bool include, int &sqt, int &sqf, int &piece, int side, bool &is_capture, MoveList *move_list)
{
	int move = 0;
	bool fill;
	if (IsEmptySquare(sqt))
	{
		move = encode_move(sqf, sqt, piece, 0, 0, 0, 0, 0);
		is_capture = false;
	}
	else if (IsOccupiedByOponent(sqt, side))
	{
		move = encode_move(sqf, sqt, piece, 0, 1, 0, 0, 0);
		is_capture = true;
	}
	if (move > 0)
	{
		fill = false;
		if (include & Test_move(move))
		{
			fill = true;
		}
		else if (!include)
		{
			fill = true;
		}
		if (fill)
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

/// @brief find move of a leaper piece => knight or king
/// @param include test for valid move (gui) or not (search)
/// @param sqt square to
/// @param sqf square from
/// @param piece piece to move
/// @param side side to move
/// @param move_list pointer to the movelist
void Board::Leaper_piece_move(bool include, int &sqt, int &sqf, int &piece, int side, MoveList *move_list)
{
	int move = 0;
	bool fill;

	if (IsEmptySquare(sqt))
	{
		move = encode_move(sqf, sqt, piece, 0, 0, 0, 0, 0);
	}
	else if (IsOccupiedByOponent(sqt, side))
	{
		move = encode_move(sqf, sqt, piece, 0, 1, 0, 0, 0);
	}
	if (move > 0)
	{
		fill = false;
		if (include & Test_move(move))
		{
			fill = true;
		}
		else if (!include)
		{
			fill = true;
		}
		if (fill)
		{
			move_list->push_back(move);
			set_bit(brd->piece_options[sqf], sqt);
			set_bit(brd->all_options[side], sqf);
		}
	}
}

/// @brief Test if the move is valid
/// @param move move to test
/// @return true/false
bool Board::Test_move(int move)
{
	chess_board *keep = Copy_board();
	Makemove(move, all_moves);
	bool incheck = IsKingInCheck(brd->side);
	Take_back(keep);
	delete keep;
	return !incheck;
}

/// @brief Make a move, update from the bitboards and update from the hash_key
/// @param move move to make
/// @return true/false
bool Board::Makemove(int move, int make_move_flag)
{
	if (make_move_flag == all_moves)
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
		Set_occupancies();
		brd->side ^= 1;
		brd->hash_key ^= gen->side_key;
		return true;
	}
	else
	{
		if (get_move_capture(move))
		{
			return Makemove(move, all_moves);
		}
		else
		{
			return false;
		}
	}
}

/// @brief Test if the king from a side is in check
/// @param side side to move
/// @return true/false
bool Board::IsKingInCheck(int side)
{
	int xside = side ^ 1;
	int king_position = side == white ? get_ls1b_index(brd->bitboards[K]) : get_ls1b_index(brd->bitboards[k]);
	brd->incheck[side] = IsSquareAttacked(king_position, xside);
	return brd->incheck[side];
}

// ------------------------------------------------------------------------------------------------
// End Board
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Evaluate
// ------------------------------------------------------------------------------------------------

/// @brief Constructor
Evaluate::Evaluate()
{
	Init_evaluation_masks();
}

/// @brief evaluate current positon
///
///	Now in order to calculate interpolated score
///	for a given game phase use this formula
///	(same for material and positional scores):
///	(
///	score_opening * game_phase_score +
///	score_endgame * (opening_phase_score - game_phase_score)
///	) / opening_phase_score
///
///	E.g. the score for pawn on d4 at phase say 5000 would be
///	interpolated_score = (12 * 5000 + (-7) * (6192 - 5000)) / 6192 = 8,342377261
///
/// @param clone copy of the chessboard
/// @return value
int Evaluate::Evaluate_board_position(chess_board *clone)
{
	int game_phase_score = Get_game_phase_score(clone);	 // get game phase score
	int game_phase = -1;								 // game phase (opening, middle game, endgame)
	int score = 0, score_opening = 0, score_endgame = 0; // static evaluation score
	U64 bitboard;										 // current pieces bitboard copy
	int piece, square;									 // init piece & square
	int double_pawns = 0;								 // penalties

	if (game_phase_score > opening_phase_score) // pick up game phase based on game phase score
	{
		game_phase = opening;
	}
	else if (game_phase_score < endgame_phase_score)
	{
		game_phase = endgame;
	}
	else
	{
		game_phase = middlegame;
	}
	for (int bb_piece = P; bb_piece <= k; bb_piece++) // loop over piece bitboards
	{
		bitboard = clone->bitboards[bb_piece]; // init piece bitboard copy
		while (bitboard)					   // loop over pieces within a bitboard
		{
			piece = bb_piece;				   // init piece
			square = get_ls1b_index(bitboard); // init square
			score_opening += material_score[opening][piece];
			score_endgame += material_score[endgame][piece];
			switch (piece) // score positional piece scores
			{
			case P: // evaluate white pawns
				score_opening += positional_score[opening][pawn][square];
				score_endgame += positional_score[endgame][pawn][square];
				double_pawns = count_bits(clone->bitboards[P] & file_masks[square]);
				if (double_pawns > 1)
				{
					score_opening += (double_pawns - 1) * double_pawn_penalty_opening;
					score_endgame += (double_pawns - 1) * double_pawn_penalty_endgame;
				}
				if ((clone->bitboards[P] & isolated_masks[square]) == 0)
				{
					score_opening += isolated_pawn_penalty_opening;
					score_endgame += isolated_pawn_penalty_endgame;
				}
				if ((white_passed_masks[square] & clone->bitboards[p]) == 0)
				{
					score_opening += passed_pawn_bonus[get_rank[square]];
					score_endgame += passed_pawn_bonus[get_rank[square]];
				}
				break;
			case N: // evaluate white knights
				score_opening += positional_score[opening][knight][square];
				score_endgame += positional_score[endgame][knight][square];
				break;
			case B: // evaluate white bishops
				score_opening += positional_score[opening][bishop][square];
				score_endgame += positional_score[endgame][bishop][square];
				score_opening += (count_bits(clone->piece_options[square]) - bishop_unit) * bishop_mobility_opening; // mobility
				score_endgame += (count_bits(clone->piece_options[square]) - bishop_unit) * bishop_mobility_endgame; // mobility
				break;
			case R: // evaluate white rooks
				score_opening += positional_score[opening][rook][square];
				score_endgame += positional_score[endgame][rook][square];
				if ((clone->bitboards[P] & file_masks[square]) == 0) // semi open file
				{
					score_opening += semi_open_file_score;
					score_endgame += semi_open_file_score;
				}
				if (((clone->bitboards[P] | clone->bitboards[p]) & file_masks[square]) == 0) // open file
				{
					score_opening += open_file_score;
					score_endgame += open_file_score;
				}
				break;
			case Q: // evaluate white queens
				score_opening += positional_score[opening][queen][square];
				score_endgame += positional_score[endgame][queen][square];
				score_opening += (count_bits(clone->piece_options[square]) - queen_unit) * queen_mobility_opening; // mobility
				score_endgame += (count_bits(clone->piece_options[square]) - queen_unit) * queen_mobility_endgame; // mobility
				break;
			case K: // evaluate white king
				score_opening += positional_score[opening][king][square];
				score_endgame += positional_score[endgame][king][square];
				if ((clone->bitboards[P] & file_masks[square]) == 0) // semi open file
				{
					score_opening -= semi_open_file_score;
					score_endgame -= semi_open_file_score;
				}
				if (((clone->bitboards[P] | clone->bitboards[p]) & file_masks[square]) == 0) // open file
				{
					score_opening -= open_file_score;
					score_endgame -= open_file_score;
				}
				score_opening += count_bits(clone->piece_options[square] & clone->occupancies[white]) * king_shield_bonus; // king safety bonus
				score_endgame += count_bits(clone->piece_options[square] & clone->occupancies[white]) * king_shield_bonus; // king safety bonus
				break;
			case p: // evaluate black pawns
				score_opening -= positional_score[opening][pawn][mirror(square)];
				score_endgame -= positional_score[endgame][pawn][mirror(square)];
				double_pawns = count_bits(clone->bitboards[p] & file_masks[square]);
				if (double_pawns > 1)
				{
					score_opening -= (double_pawns - 1) * double_pawn_penalty_opening;
					score_endgame -= (double_pawns - 1) * double_pawn_penalty_endgame;
				}
				if ((clone->bitboards[p] & isolated_masks[square]) == 0)
				{
					score_opening -= isolated_pawn_penalty_opening;
					score_endgame -= isolated_pawn_penalty_endgame;
				}
				if ((black_passed_masks[square] & clone->bitboards[P]) == 0)
				{
					score_opening -= passed_pawn_bonus[get_rank[square]];
					score_endgame -= passed_pawn_bonus[get_rank[square]];
				}
				break;
			case n: // evaluate black knights
				score_opening -= positional_score[opening][knight][mirror(square)];
				score_endgame -= positional_score[endgame][knight][mirror(square)];
				break;
			case b: // evaluate black bishops
				score_opening -= positional_score[opening][bishop][mirror(square)];
				score_endgame -= positional_score[endgame][bishop][mirror(square)];
				score_opening -= (count_bits(clone->piece_options[square]) - bishop_unit) * bishop_mobility_opening; // mobility
				score_endgame -= (count_bits(clone->piece_options[square]) - bishop_unit) * bishop_mobility_endgame; // mobility
				break;
			case r: // evaluate black rooks
				score_opening -= positional_score[opening][rook][mirror(square)];
				score_endgame -= positional_score[endgame][rook][mirror(square)];
				if ((clone->bitboards[p] & file_masks[square]) == 0) // semi open file
				{
					score_opening -= semi_open_file_score;
					score_endgame -= semi_open_file_score;
				}
				if (((clone->bitboards[P] | clone->bitboards[p]) & file_masks[square]) == 0) // open file
				{
					score_opening -= open_file_score;
					score_endgame -= open_file_score;
				}
				break;
			case q: // evaluate black queens
				score_opening -= positional_score[opening][queen][mirror(square)];
				score_endgame -= positional_score[endgame][queen][mirror(square)];
				score_opening -= (count_bits(clone->piece_options[square]) - queen_unit) * queen_mobility_opening; // mobility
				score_endgame -= (count_bits(clone->piece_options[square]) - queen_unit) * queen_mobility_endgame; // mobility
				break;
			// evaluate black king
			case k: // get opening/endgame positional score
				score_opening -= positional_score[opening][king][mirror(square)];
				score_endgame -= positional_score[endgame][king][mirror(square)];
				if ((clone->bitboards[p] & file_masks[square]) == 0) // semi open file
				{
					score_opening += semi_open_file_score;
					score_endgame += semi_open_file_score;
				}
				if (((clone->bitboards[P] | clone->bitboards[p]) & file_masks[square]) == 0) // open file
				{
					score_opening += open_file_score;
					score_endgame += open_file_score;
				}
				score_opening -= count_bits(clone->piece_options[square] & clone->occupancies[black]) * king_shield_bonus; // king safety bonus
				score_endgame -= count_bits(clone->piece_options[square] & clone->occupancies[black]) * king_shield_bonus; // king safety bonus
				break;
			}
			pop_bit(bitboard, square); // reset the bit on square
		}
	}
	if (game_phase == middlegame) // interpolate score in the middlegame
	{
		score = (score_opening * game_phase_score +
				 score_endgame * (opening_phase_score - game_phase_score)) /
				opening_phase_score;
	}
	else if (game_phase == opening) // pure opening score in opening
	{
		score = score_opening;
	}
	else if (game_phase == endgame) // return pure endgame score in endgame
	{
		score = score_endgame;
	}
	return (clone->side == white) ? score : -score; // return final evaluation based on side
}

/// @brief File the file bitboard or the rank bitboard
/// @param file_number a8..h1
/// @param rank_number a8..h1
/// @return bitboard
U64 Evaluate::Set_file_rank_mask(int file_number, int rank_number)
{
	U64 mask = 0ULL; // file or rank mask

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			if (file_number != -1)
			{
				if (file == file_number) // on file match
				{
					set_bit(mask, square);
				}
			}
			else if (rank_number != -1)
			{
				if (rank == rank_number) // on rank match
				{
					set_bit(mask, square);
				}
			}
		}
	}
	return mask;
}

/// @brief Fill the mask bitboards
void Evaluate::Init_evaluation_masks()
{
	// Init file masks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			file_masks[square] |= Set_file_rank_mask(file, -1); // init file mask for a current square
		}
	}
	// Init rank masks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			rank_masks[square] |= Set_file_rank_mask(-1, rank); // init rank mask for a current square
		}
	}
	// Init isolated masks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			isolated_masks[square] |= Set_file_rank_mask(file - 1, -1); // init isolated pawns masks for a current square
			isolated_masks[square] |= Set_file_rank_mask(file + 1, -1); // init isolated pawns masks for a current square
		}
	}
	// White passed masks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			white_passed_masks[square] |= Set_file_rank_mask(file - 1, -1); // init white passed pawns mask for a current square
			white_passed_masks[square] |= Set_file_rank_mask(file, -1);		// init white passed pawns mask for a current square
			white_passed_masks[square] |= Set_file_rank_mask(file + 1, -1); // init white passed pawns mask for a current square
			for (int i = 0; i < (8 - rank); i++)							// loop over redudant ranks
			{
				white_passed_masks[square] &= ~rank_masks[(7 - i) * 8 + file]; // reset redudant bits
			}
		}
	}
	// Black passed masks
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;
			black_passed_masks[square] |= Set_file_rank_mask(file - 1, -1); // init black passed pawns mask for a current square
			black_passed_masks[square] |= Set_file_rank_mask(file, -1);		// init black passed pawns mask for a current square
			black_passed_masks[square] |= Set_file_rank_mask(file + 1, -1); // init black passed pawns mask for a current square
			for (int i = 0; i < rank + 1; i++)								// loop over redudant ranks
			{
				black_passed_masks[square] &= ~rank_masks[i * 8 + file]; // reset redudant bits
			}
		}
	}
}

/// @brief
///
///	The game phase score of the game is derived from the pieces
///		(not counting pawns and kings) that are still on the board.
///		The full material starting position game phase score is:
///
///		4 * knight material score in the opening +
///		4 * bishop material score in the opening +
///		4 * rook material score in the opening +
///		2 * queen material score in the opening
///
/// @return score
/// @param brd pointer to the chess_board
int Evaluate::Get_game_phase_score(chess_board *brd)
{
	int white_piece_scores = 0, black_piece_scores = 0; // white & black game phase scores
	for (int piece = N; piece <= Q; ++piece)			// loop over white pieces
	{
		white_piece_scores += count_bits(brd->bitboards[piece]) * material_score[opening][piece];
	}
	for (int piece = n; piece <= q; piece++) // loop over black pieces
	{
		black_piece_scores += count_bits(brd->bitboards[piece]) * -material_score[opening][piece];
	}
	return white_piece_scores + black_piece_scores; // return game phase score
}

// ------------------------------------------------------------------------------------------------
// End Evaluate
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Search
// ------------------------------------------------------------------------------------------------

/// @brief @brief constructor
/// @param b pointer to the board object
/// @param e pointer to the eval object
/// @param g pointer to the hash_data structure
Search::Search(Board *b, Evaluate *e, hash_data *g)
{
	brd = b;
	eval = e;
	gen = g;
}

/// @brief clear the transposition
/// @param hash_entry pointer to hash table
void Search::Clear_hash_table()
{
	for (tt *hash_entry = this->hash_table; hash_entry < this->hash_table + hash_entries; ++hash_entry) // loop over TT elements
	{
		// reset TT inner fields
		hash_entry->hash_key = 0;
		hash_entry->depth = 0;
		hash_entry->flag = 0;
		hash_entry->score = 0;
	}
}

/// @brief Initialize the TT (transposition table)
/// @param mb size of memory allocated
void Search::Init_hash_table(int mb)
{
	int hash_size = 0x100000 * mb;		   // init hash size
	hash_entries = hash_size / sizeof(tt); // init number of hash entries
	if (this->hash_table != nullptr)	   // free memory
	{
#ifndef NDEBUG
		std::cout << "    Clearing hash memory..." << std::endl;
#endif
		delete this->hash_table;
	}
	hash_table = (tt *)malloc(hash_entries * sizeof(tt)); // allocate memory
	if (hash_table == nullptr)
	{
#ifndef NDEBUG
		std::cout << "    Couldn't allocate memory for hash table, try " << mb / 2 << std::endl;
#endif
		Init_hash_table(mb / 2); // try to allocate with half size
	}
	else // if allocation succeeded
	{
		Clear_hash_table();
#ifndef NDEBUG
		std::cout << "    Hash table is initialied with " << hash_entries << " entries" << std::endl;
#endif
	}
}

/// @brief Read an entry from the hash data
/// @param best_move pointer to the best_move found
/// @param alpha alpha score
/// @param beta beta score
/// @param depth search depth
/// @return score pv, alpha or beta
int Search::Read_hash_entry(int alpha, int beta, int *best_move, int depth)
{
	// create a TT instance pointer to particular hash entry storing
	// the scoring data for the current board position if available
	tt *hash_entry = &hash_table[brd->GetChessBoard()->hash_key % hash_entries];

	// make sure dealing with the exact position
	if (hash_entry->hash_key == brd->GetChessBoard()->hash_key)
	{
		// make sure that the depth match the exact depth the search is now at
		if (hash_entry->depth >= depth)
		{
			// extract stored score from TT entry
			int score = hash_entry->score;

			// retrieve score independent from the actual path
			// from root node (position) to current node (position)
			if (score < -mate_score)
				score += brd->GetChessBoard()->ply;
			if (score > mate_score)
				score -= brd->GetChessBoard()->ply;

			// match the exact (PV node) score
			if (hash_entry->flag == hash_flag_exact)
				// return exact (PV node) score
				return score;

			// match alpha (fail-low node) score
			if ((hash_entry->flag == hash_flag_alpha) &&
				(score <= alpha))
				// return alpha (fail-low node) score
				return alpha;

			// match beta (fail-high node) score
			if ((hash_entry->flag == hash_flag_beta) &&
				(score >= beta))
				// return beta (fail-high node) score
				return beta;
		}
		// store best move
		*best_move = hash_entry->best_move;
	}

	return no_hash_entry; // if hash entry doesn't exist
}

/// @brief write an entry to the hash_table
/// @param best_move best move found so far
/// @param score current score pv, alpha or beta
/// @param depth search depth
/// @param hash_flag current flag pv, alpha or beta
void Search::Write_hash_entry(int score, int best_move, int depth, int hash_flag)
{
	// create a TT instance pointer to particular hash entry storing
	// the scoring data for the current board position if available
	tt *hash_entry = &hash_table[brd->GetChessBoard()->hash_key % hash_entries];

	// store score independent from the actual path
	// from root node (position) to current node (position)
	if (score < -mate_score)
		score -= brd->GetChessBoard()->ply;
	if (score > mate_score)
		score += brd->GetChessBoard()->ply;

	// write hash entry data
	hash_entry->hash_key = brd->GetChessBoard()->hash_key;
	hash_entry->score = score;
	hash_entry->flag = hash_flag;
	hash_entry->depth = depth;
	hash_entry->best_move = best_move;
}

/// @brief set follow_pv to true or false
/// @param move_list
void Search::Enable_pv_scoring(MoveList *move_list)
{
	follow_pv = false;

	int *arr = move_list->data();
	for (int index = 0; index < move_list->size(); ++index) // loop over the moves within a move list
	{
		if (pv_table[0][brd->GetChessBoard()->ply] == arr[index]) // make sure to hit the PV move
		{
			score_pv = true;  // enable move scoring to PV
			follow_pv = true; // enable following PV
		}
	}
}

/// @brief position repetition detection
/// @return true/false
bool Search::Is_repetition()
{
	U64 *arr = brd->Get_repetiton_table()->data();

	for (int index = 0; index < brd->Get_repetiton_table()->size(); ++index) // loop over repetition indices range
	{
		if (arr[index] == brd->Copy_board()->hash_key) // found a repetition
		{
			return true;
		}
	}

	return false; // no repetition found
}

/// @brief Set score for a move => move ordening
///  =======================
///	 Move ordering
///	 =======================
///	 1. PV move
///	 2. Captures in MVV/LVA
///	 3. 1st killer move
///	 4. 2nd killer move
///	 5. History moves
///	 6. Unsorted moves
///
/// @param move current move
/// @return score
int Search::Score_move(int move)
{
	if (score_pv) // if PV move scoring is allowed
	{
		if (pv_table[0][brd->GetChessBoard()->ply] == move) // make sure dealing with the PV move
		{
			score_pv = false; // disable score PV flag
			return 20000;	  // give PV move the highest score to search it first
		}
	}
	if (get_move_capture(move)) // score capture move
	{
		int piece = get_move_piece(move); // init source piece
		int target_piece = P;			  // init target piece
		// pick up bitboard piece index ranges depending on side
		int start_piece = brd->GetChessBoard()->side == white ? p : P;
		int end_piece = brd->GetChessBoard()->side == white ? k : K;

		for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) // loop over bitboards opposite to the current side to move
		{
			if (get_bit(brd->GetChessBoard()->bitboards[bb_piece], get_move_target(move)))
			{
				target_piece = bb_piece; // remove it from corresponding bitboard
				break;
			}
		}

		return mvv_lva[get_move_piece(move)][target_piece] + 10000; // score move by MVV LVA lookup [source piece][target piece]
	}
	else // score quiet move
	{
		if (killer_moves[0][brd->GetChessBoard()->ply] == move) // score 1st killer move
		{
			return 9000;
		}
		else if (killer_moves[1][brd->GetChessBoard()->ply] == move) // score 2nd killer move
		{
			return 8000;
		}
		else
		{
			return history_moves[get_move_piece(move)][get_move_target(move)];
		}
	}

	return 0;
}

/// @brief Sort the moves
/// @param move_list pointer to the moves found
/// @param best_move best move found so far
void Search::Sort_moves(MoveList *move_list, int best_move)
{
	// move scores
	MoveList *move_scores = new MoveList();

	int *arr = move_list->data();
	// score all the moves within a move list
	for (int count = 0; count < move_list->size(); ++count)
	{
		if (best_move == arr[count]) // if hash move available
		{
			move_scores->push_back(30000); // score move
		}
		else
		{
			move_scores->push_back(Score_move(arr[count])); // score move
		}
	}
	int *ms = move_scores->data();
	for (int current_move = 0; current_move < move_list->size(); ++current_move) // loop over current move within a move list
	{
		for (int next_move = current_move + 1; next_move < move_list->size(); ++next_move) // loop over next move within a move list
		{
			if (ms[current_move] < ms[next_move]) // compare current and next move scores
			{
				// swap scores
				int temp_score = ms[current_move];
				ms[current_move] = ms[next_move];
				ms[next_move] = temp_score;
				// swap moves
				int temp_move = arr[current_move];
				arr[current_move] = arr[next_move];
				arr[next_move] = temp_move;
			}
		}
	}
}

/// @brief Print the move scores
/// @param move_list pointer to the moves found
void Search::Print_move_scores(MoveList *move_list)
{
	printf("     Move scores:\n\n");

	// loop over moves within a move list
	int *arr = move_list->data();
	for (int count = 0; count < move_list->size(); ++count)
	{
		printf("     move: ");
		Print_move(arr[count]);
		printf(" score: %d\n", Score_move(arr[count]));
	}
}

/// @brief quiescence search
/// @param alpha alpha score
/// @param beta beta score
/// @return
int Search::Quiescence(int alpha, int beta)
{
	if ((nodes & 2047) == 0) // every 2047 nodes
	{
		Communicate(); // "listen" to the GUI/user input
	}
	++nodes;															  // increment nodes count
	int evaluation = eval->Evaluate_board_position(brd->GetChessBoard()); // evaluate position
	if (brd->GetChessBoard()->ply > max_ply - 1)						  // too deep
	{
		return evaluation;
	}
	if (evaluation >= beta) // fail-hard beta cutoff
	{
		return beta; // node (position) fails high
	}
	if (evaluation > alpha) // found a better move
	{
		alpha = evaluation; // PV node (position)
	}

	MoveList *move_list = new MoveList();
	brd->Generate_moves(move_list, false);
	Sort_moves(move_list, 0);
	int *arr = move_list->data();
	for (int count = 0; count < move_list->size(); ++count) // loop over moves within a movelist
	{
		chess_board *keep = brd->Copy_board();								   // preserve board state
		++brd->GetChessBoard()->ply;										   // increment ply
		brd->Get_repetiton_table()->push_back(brd->GetChessBoard()->hash_key); // increment repetition table
		if (!brd->Makemove(arr[count], only_captures))						   // make sure to make only legal moves
		{
			--brd->GetChessBoard()->ply;			// decrement ply
			brd->Get_repetiton_table()->pop_back(); // decrement repetition table
			delete keep;							// remove the keep object
			continue;								// skip to next move
		}
		int score = -Quiescence(-beta, -alpha); // score current move
		--brd->GetChessBoard()->ply;			// decrement ply
		brd->Get_repetiton_table()->pop_back(); // decrement repetition table
		brd->Take_back(keep);					// take move back
		delete keep;							// remove the keep object
		if (stopped)
		{
			delete move_list; // clean up the movelist
			return 0;		  // time is up
		}
		if (score > alpha) // found a better move
		{
			alpha = score;	   // PV node (position)
			if (score >= beta) // fail-hard beta cutoff
			{
				delete move_list;
				return beta; // node (position) fails high
			}
		}
	}

	delete move_list; // clean up the movelist
	return alpha;	  // node (position) fails low
}

/// @brief Negamax search
/// @param alpha alpha score
/// @param beta beta score
/// @param depth search depth
/// @return score
int Search::Negamax(int alpha, int beta, int depth)
{
	pv_length[brd->GetChessBoard()->ply] = brd->GetChessBoard()->ply; // init PV length
	int score = 0;													  // variable to store current move's score (from the static evaluation perspective)
	int hash_flag = hash_flag_alpha;								  // define hash flag
	int best_move = 0;												  // best move (to store in TT)

	if (brd->GetChessBoard()->ply && Is_repetition() || brd->GetChessBoard()->fifty >= 100) // if position repetition occurs or the 50 moves rule is reached
	{
		return 0; // return draw score
	}
	int pv_node = beta - alpha > 1; // a hack by Pedro Castro to figure out whether the current node is PV node or not
									// read hash entry if not in a root ply and hash entry is available
									// and current node is not a PV node
	if (brd->GetChessBoard()->ply && (score = Read_hash_entry(alpha, beta, &best_move, depth)) != no_hash_entry && pv_node == 0)
	{
		return score; // if the move has already been searched (hence has a value)
					  // just return the score for this move without searching it
	}
	if ((nodes & 2047) == 0) // every 2047 nodes
	{
		Communicate(); // "listen" to the GUI/user input
	}
	if (depth == 0) // recursion escapre condition
	{
		return Quiescence(alpha, beta); // run quiescence search
	}
	if (brd->GetChessBoard()->ply > max_ply - 1) // too deep
	{
		return eval->Evaluate_board_position(brd->GetChessBoard()); // evaluate position
	}
	++nodes; // increment nodes count
	// is king in check
	bool in_check = brd->IsKingInCheck(brd->GetChessBoard()->side);
	if (in_check) // increase search depth if the king has been exposed into a check
	{
		++depth;
	}
	int legal_moves = 0;													   // keep track of the legal moves which are made
	int static_eval = eval->Evaluate_board_position(brd->GetChessBoard());	   // get static evaluation score
	if (depth < 3 && !pv_node && !in_check && abs(beta - 1) > -infinity + 100) // evaluation pruning / static null move pruning
	{
		int eval_margin = 120 * depth;		   // define evaluation margin
		if (static_eval - eval_margin >= beta) // evaluation margin substracted from static evaluation score fails high
		{
			return static_eval - eval_margin; // evaluation margin substracted from static evaluation score
		}
	}
	if (depth >= 3 && !in_check && brd->GetChessBoard()->ply) // null move pruning
	{
		chess_board *keep = brd->Copy_board();								   // preserve board state
		++brd->GetChessBoard()->ply;										   // increment ply
		brd->Get_repetiton_table()->push_back(brd->GetChessBoard()->hash_key); // increment repetition table
		if (brd->GetChessBoard()->enpassant != no_sq)						   // hash enpassant if available
		{
			brd->GetChessBoard()->hash_key ^= gen->enpassant_keys[brd->GetChessBoard()->enpassant];
		}
		brd->GetChessBoard()->enpassant = no_sq;		   // reset enpassant capture square
		brd->GetChessBoard()->side ^= 1;				   // switch the side, literally giving opponent an extra move to make
		brd->GetChessBoard()->hash_key ^= gen->side_key;   // hash the side
		score = -Negamax(-beta, -beta + 1, depth - 1 - 2); // search moves with reduced depth to find beta cutoffs
														   // depth - 1 - R where R is a reduction limit
		--brd->GetChessBoard()->ply;					   // decrement ply
		brd->Get_repetiton_table()->pop_back();			   // decrement repetition table
		brd->Take_back(keep);							   // restore board state
		delete keep;									   // delete keep object
		if (stopped)									   // return 0 if time is up
		{
			return 0;
		}
		if (score >= beta) // fail-hard beta cutoff
		{
			return beta; // node (position) fails high
		}
	}
	if (!pv_node && !in_check && depth <= 3) // razoring
	{
		score = static_eval + 125; // get static eval and add first bonus
		int new_score;			   // define new score
		if (score < beta)		   // static evaluation indicates a fail-low node
		{
			if (depth == 1) // on depth 1
			{
				new_score = Quiescence(alpha, beta);			// get quiscence score
				return (new_score > score) ? new_score : score; // return quiescence score if it's greater then static evaluation score
			}
			score += 175;					// add second bonus to static evaluation
			if (score < beta && depth <= 2) // static evaluation indicates a fail-low node
			{
				new_score = Quiescence(alpha, beta); // get quiscence score
				if (new_score < beta)				 // quiescence score indicates fail-low node
				{
					return (new_score > score) ? new_score : score; // return quiescence score if it's greater then static evaluation score
				}
			}
		}
	}
	MoveList *move_list = new MoveList();
	brd->Generate_moves(move_list, false); // generate moves
	if (follow_pv)						   // if following the PV line
	{
		Enable_pv_scoring(move_list); // enable PV move scoring
	}
	Sort_moves(move_list, best_move); // sort moves
	int moves_searched = 0;			  // number of moves searched in a move list
	int *arr = move_list->data();
	// loop over moves within a movelist
	for (int count = 0; count < move_list->size(); ++count)
	{
		chess_board *keep = brd->Copy_board();								   // preserve board state
		++brd->GetChessBoard()->ply;										   // increment ply
		brd->Get_repetiton_table()->push_back(brd->GetChessBoard()->hash_key); // increment repetition table
		if (brd->Makemove(arr[count], all_moves) == 0)						   // make sure to make only legal moves
		{
			--brd->GetChessBoard()->ply;			// decrement ply
			brd->Get_repetiton_table()->pop_back(); // decrement repetition index
			delete keep;							// remove the keep object
			continue;
		}
		++legal_moves;			 // increment legal moves
		if (moves_searched == 0) // full depth search
		{
			score = -Negamax(-beta, -alpha, depth - 1);
		}
		else // late move reduction (LMR)
		{
			if (
				moves_searched >= full_depth_moves &&
				depth >= reduction_limit &&
				!in_check &&
				get_move_capture(arr[count]) == 0 &&
				get_move_promoted(arr[count]) == 0) // condition to consider LMR
			{
				score = -Negamax(-alpha - 1, -alpha, depth - 2); // search current move with reduced depth
			}
			else
			{
				score = alpha + 1; // hack to ensure that full-depth search is done
			}
			if (score > alpha) // principle variation search PVS
			{
				score = -Negamax(-alpha - 1, -alpha, depth - 1); // Once found a move with a score that is between alpha and beta,
																 // the rest of the moves are searched with the goal of proving that they are all bad.
																 // It's possible to do this a bit faster than a search that worries that one
																 // of the remaining moves might be good.
				if ((score > alpha) && (score < beta))			 // If the algorithm finds out that it was wrong, and that one of the
																 // subsequent moves was better than the first PV move, it has to search again,
																 // in the normal alpha-beta manner.  This happens sometimes, and it's a waste of time,
																 // but generally not often enough to counteract the savings gained from doing the
																 // "bad move proof" search referred to earlier.

				{
					score = -Negamax(-beta, -alpha, depth - 1); // re-search the move that has failed to be proved to be bad
																// with normal alpha beta score bounds
				}
			}
		}
		--brd->GetChessBoard()->ply;			// decrement ply
		brd->Get_repetiton_table()->pop_back(); // decrement repetition index
		brd->Take_back(keep);					// take move back
		delete keep;							// remove the keep object
		if (stopped)							// time is up
		{
			return 0;
		}
		++moves_searched;  // increment the counter of moves searched so far
		if (score > alpha) // // found a better move
		{
			hash_flag = hash_flag_exact;		   // switch hash flag from storing score for fail-low node
												   // to the one storing score for PV node
			best_move = arr[count];				   // store best move (for tt)
			if (get_move_capture(arr[count]) == 0) // on quiet moves
			{
				history_moves[get_move_piece(arr[count])][get_move_target(arr[count])] += depth; // store history moves
			}
			alpha = score;																										// PV node (position)
			pv_table[brd->GetChessBoard()->ply][brd->GetChessBoard()->ply] = arr[count];										// write PV move
			for (int next_ply = brd->GetChessBoard()->ply + 1; next_ply < pv_length[brd->GetChessBoard()->ply + 1]; ++next_ply) // loop over the next ply
			{
				pv_table[brd->GetChessBoard()->ply][next_ply] = pv_table[brd->GetChessBoard()->ply + 1][next_ply]; // copy move from deeper ply into a current ply's line
			}
			pv_length[brd->GetChessBoard()->ply] = pv_length[brd->GetChessBoard()->ply + 1]; // adjust PV length
			if (score >= beta)																 // fail-hard beta cutoff
			{
				Write_hash_entry(beta, best_move, depth, hash_flag_beta); // store hash entry with the score equal to beta
				if (get_move_capture(arr[count]) == 0)					  // on quiet moves
				{
					killer_moves[1][brd->GetChessBoard()->ply] = killer_moves[0][brd->GetChessBoard()->ply]; // store killer moves
					killer_moves[0][brd->GetChessBoard()->ply] = arr[count];								 // store killer moves
				}
				delete move_list; // clean up movelist
				return beta;	  // node (position) fails high
			}
		}
	}
	if (legal_moves == 0)
	{
		if (in_check) // king is in check
		{
			return -mate_value + brd->GetChessBoard()->ply; // return mating score (assuming closest distance to mating position)
		}

		else // king is not in check
		{
			return 0; // return stalemate score
		}
	}
	Write_hash_entry(alpha, best_move, depth, hash_flag); // store hash entry with the score equal to alpha
	delete move_list;									  // clean up movelist
	return alpha;										  // node (position) fails low
}

void Search::Communicate()
{
	// todo
}

// ------------------------------------------------------------------------------------------------
// End Search
// ------------------------------------------------------------------------------------------------

// eof