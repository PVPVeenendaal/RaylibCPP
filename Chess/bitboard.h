#pragma once

#include <string>
#include <vector>

// ****************************************************
// Bitboard                                           *
// Using C Code from BBC by Code Monkey King          *
// Translated to C++ and objects by Peter Veenendaal  *
// ****************************************************

typedef unsigned long long U64;

#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

static inline int count_bits(U64 bitboard);
static inline int get_ls1b_index(U64 bitboard);
extern void print_bitboard(U64 bitboard); // debug only

// board squares
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
    no_sq
};

enum ePieces
{
    P, // White pawn
    N, // White knight
    B, // White bishop
    R, // White rook
    Q, // White queen
    K, // White king
    p, // Black pawn
    n, // Black knight
    b, // Black bishop
    r, // Black rook
    q, // Black queen
    k  // Black king
};

enum eColors
{
    white,
    black,
    both
};

enum eCastle
{
    wk = 1, // white kingside
    wq = 2, // white queenside
    bk = 4, // black kingside
    bq = 8  // black kingside
};

const char ascii_pieces[13] = "PNBRQKpnbrqk"; // enum pieces

const char ascii_occupancies[4] = "WBA"; // enum eColors => occupansies

char promoted_pieces[] = {
    [Q] = 'Q',
    [R] = 'R',
    [B] = 'B',
    [N] = 'N',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n'};

// start position pieces
const U64 piece_bitboards[12] = {
    71776119061217280ULL, // White pawns
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  1 1 1 1 1 1 1 1
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    4755801206503243776ULL, // White knights
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 1 0 0 0 0 1 0
       a b c d e f g h
    */
    2594073385365405696ULL, // White bishops
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 1 0 0 1 0 0
       a b c d e f g h
    */
    9295429630892703744ULL, // White rooks
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  1 0 0 0 0 0 0 1
       a b c d e f g h
    */
    576460752303423488ULL, // White queen
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 1 0 0 0 0
       a b c d e f g h
    */
    1152921504606846976ULL, // White king
    /*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 1 0 0 0
       a b c d e f g h
    */
    65280ULL, // Black pawns
    /*
    8  0 0 0 0 0 0 0 0
    7  1 1 1 1 1 1 1 1
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    66ULL, // Black knights
    /*
    8  0 1 0 0 0 0 1 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    36ULL, // Black bishhops
    /*
    8  0 0 1 0 0 1 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    129ULL, // Black rooks
    /*
    8  1 0 0 0 0 0 0 1
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    8ULL, // Black queen
    /*
    8  0 0 0 1 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
    16ULL // Black king
    /*
    8  0 0 0 0 1 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
    */
};

// start position occupansies
// white (P | N | B | R | Q | K)
/*
    8  0 0 0 0 0 0 0 0
    7  0 0 0 0 0 0 0 0
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  1 1 1 1 1 1 1 1
    1  1 1 1 1 1 1 1 1
       a b c d e f g h
*/
// Black (p | n | b | r | q | k)
/*
    8  1 1 1 1 1 1 1 1
    7  1 1 1 1 1 1 1 1
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0
       a b c d e f g h
*/
// Both (white | black)
/*
    8  1 1 1 1 1 1 1 1
    7  1 1 1 1 1 1 1 1
    6  0 0 0 0 0 0 0 0
    5  0 0 0 0 0 0 0 0
    4  0 0 0 0 0 0 0 0
    3  0 0 0 0 0 0 0 0
    2  1 1 1 1 1 1 1 1
    1  1 1 1 1 1 1 1 1
       a b c d e f g h
*/

// eof