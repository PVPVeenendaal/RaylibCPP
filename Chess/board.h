#pragma once

#include <vector>
#include "move.h"

typedef unsigned long long U64;

enum ePieces
{
    empty,
    WP,
    WN,
    WB,
    WR,
    WQ,
    WK,
    Bp,
    Bn,
    Bb,
    Br,
    Bq,
    Bk
};

enum eColors
{
    SWHITE,
    SBLACK,
    SEMPTY
};

enum eSquares
{
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a1,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
    EDGE // outside the board
};

enum eCastle
{
	WKS = 1, // white can castle kingside
	WQS = 2, // white can castle queenside
	BKS = 4, // black can castle kingside
	BQS = 8, // black can castle queenside
};

const int promotepieces[2][4] =
{
    {WQ, WR, WB, WN},
    {Bq, Br, Bb, Bn}
};

// macro's
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define SET_BIT(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define GET_BIT(bitboard, square) ((bitboard) & (1ULL << (square)))
#define POP_BIT(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define SQCOL(x) ((x) < EDGE ? (x) & 7 : EDGE)
#define SQROW(x) ((x) < EDGE ? (x) >> 3 : EDGE)
#define PCOLOR(x) ((x) >= WP && (x) <= WK ? SWHITE : (x) >= Bp && (x) <= Bk ? SBLACK \
                                                  : empty)
#define WHITEPAWNROW 6
#define WHITEPROMOTEROW 0
#define BLACKPAWNROW 1
#define BLACKPROMOTEROW 7

class Board
{
public:
    // constructor
    Board();
    void New_Game();
    int GetChessBoard(int idx) { return idx >= 0 && idx < EDGE ? chess_board[idx]
                                                               : empty; }
    U64 GetPieceMove(int idx) { return idx < EDGE ? piece_options[idx]
                                                  : 0ULL; }
    U64 GetAllOptions(int side) { return all_options[side]; }                                              

private:
    int chess_board[65];
    U64 all_options[2]; // set bit when piece on posisition can move
    U64 piece_options[64]; // set bit when piece on position can move to
    int side2move;
    int kingloc[2];
    bool incheck[2];
    int epsquare;
    int castle_options;
    int ply;
    
    int knight_moves[64][8];   // [squares][directions]
    int bishop_moves[64][4];   // [squares][directions]
    int rook_moves[64][4];     // [squares][directions]
    int pawn_moves[64][2][4];  // [squares][sides][directions]
    MoveList moves[32];

    // methods
    void PawnMoves(int sqf, int piece, int *test_board);
    void KnightMoves(int sqf, int piece, int *test_board);
    void BishopMoves(int sqf, int piece, int *test_board);
    void RookMoves(int sqf, int piece, int *test_board);
    void QueenMoves(int sqf, int piece, int *test_board);
    void KingMoves(int sqf, int piece, int *test_board);
    void CastleMoves(int sqf, int piece, int *test_board);
    void MakeMove(int &sqf, int &sqt, Move &m, int *test_board, int tmove, int promote);
    bool TestMove(int *test_board, Move &m, int king, int sqf, int sqt, int tmove);
    void DoMove(int *test_board, Move &m);
    void UnDoMove(int *test_board, Move &m);
    bool AttackSquare(int *test_board, int sqr, int attack_color);

};