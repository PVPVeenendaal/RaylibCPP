#pragma once

#ifndef NDEBUG
	#define LOGPRN 1
	#define FOUR_PLAYER 1
#else
	#define LOGPRN 0
	#define FOUR_PLAYER 0
#endif

#define GENERATE 0
#define TEST 0


// constants
// sizes
const int SQUARESIZE = 72;
const int HALFSQUARESIZE = 36;
const int SCREENWIDTH = 1440;
const int SCREENHEIGHT = 864;
const int BOARDSIZE = 576;
const int LARGE_PIECE_SIZE = 64;
const int SMALL_PIECE_SIZE = 32;
// no value
const int NOVALUE = -1;
// chess rules const
#if TEST
const int MAX_FIFTY_MOVES_RULE = 10; 
#else
const int MAX_FIFTY_MOVES_RULE = 100; 
#endif

// pieces
enum ePieces
{
	WP = 0,
	WN,
	WB,
	WR,
	WQ,
	WK,
	BP = 6,
	BN,
	BB,
	BR,
	BQ,
	BK,
	NP = 12,
};

// colors
enum eColors
{
	PWHITE = 0,
	PBLACK,
	NC = 12,
};

// selection
enum eSelection
{
	SELECT_PIECE = 0,
	SELECT_SQUARE,
	DO_MOVE,
};

// players
enum ePlayer
{
	HUMAN = 0,
	AI,
};

// squares
enum eSqaure
{
	A8 = 0,
	B8,
	C8,
	D8,
	E8,
	F8,
	G8,
	H8,
	A7 = 8,
	B7,
	C7,
	D7,
	E7,
	F7,
	G7,
	H7,
	A6 = 16,
	B6,
	C6,
	D6,
	E6,
	F6,
	G6,
	H6,
	A5 = 24,
	B5,
	C5,
	D5,
	E5,
	F5,
	G5,
	H5,
	A4 = 32,
	B4,
	C4,
	D4,
	E4,
	F4,
	G4,
	H4,
	A3 = 40,
	B3,
	C3,
	D3,
	E3,
	F3,
	G3,
	H3,
	A2 = 48,
	B2,
	C2,
	D2,
	E2,
	F2,
	G2,
	H2,
	A1 = 56,
	B1,
	C1,
	D1,
	E1,
	F1,
	G1,
	H1,
	EDGE = 64, // outside the board
};

enum ePawnRow
{
	WPAWNROW = 6,
	BPAWNROW = 1,
};

enum eGamestate
{
	INPUT = 0,
	RUNNING = 1,
	GAMEOVER = 2,
};

enum eGameOverState
{
	NONE = 0,
	WMATE = 1,	  // white wins, black is mated
	WTIME = 2,	  // white wins, black is out of time
	BMATE = 3,	  // black wins, white is mated
	BTIME = 4,	  // black wins, white is out of time
	DRAWMAT = 5,  // draw by material
	DRAWPATW = 6, // draw by white king is pat
	DRAWPATB = 7, // draw by black king is pat
	DRAWREP = 8,  // draw by repeat moves rule
	DRAW50 = 9,	  // draw by 50 moves rule
};

enum eImageType
{
	IMG_TABLE = 0,
	IMG_CHESS_BOARD = 1,
	IMG_LARGE_PIECES = 2,
	IMG_SMALL_PIECES = 14,
	IMG_PLAYERS = 26,
	IMG_ENTER_BTN = 28,
	IMG_PLUS_MIN_BTN = 29,
	IMG_CHOICE_BTN = 30,
	IMG_CHESS_CLOCK = 31,
};

enum eCastle
{
	WKS = 1, // white can castle kingside
	WQS = 2, // white can castle queenside
	BKS = 4, // black can castle kingside
	BQS = 8, // black can castle queenside
};

typedef unsigned long long U64;

// macro's
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define PCOLOR(x) ((x) <= WK ? PWHITE : (x) <= BK ? PBLACK \
												  : NC)
#define SET_BIT(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define GET_BIT(bitboard, square) ((bitboard) & (1ULL << (square)))
#define POP_BIT(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

#define SQCOL(x) ((x) < EDGE ? (x) & 7 : NOVALUE)
#define SQROW(x) ((x) < EDGE ? (x) >> 3 : NOVALUE)
