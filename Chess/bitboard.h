#pragma once

#include <string>
#include <vector>

typedef unsigned long long U64;

#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

static inline int count_bits(U64 bitboard);
static inline int get_ls1b_index(U64 bitboard);
extern void print_bitboard(U64 bitboard);

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
    P,
    N,
    B,
    R,
    Q,
    K,
    p,
    n,
    b,
    r,
    q,
    k
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

const char ascii_pieces[13] = "PNBRQKpnbrqk";

const char ascii_occupancies[4] = "WBA"; 

const int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k};

char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n'};

const U64 piece_bitboards[12] = {
    71776119061217280ULL,
    4755801206503243776ULL,
    2594073385365405696ULL,
    9295429630892703744ULL,
    576460752303423488ULL,
    1152921504606846976ULL,
    65280ULL,
    66ULL,
    36ULL,
    129ULL,
    8ULL,
    16ULL
};