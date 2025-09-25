#pragma once

#include <vector>
#include "bitboard.h"
#include "hash.h"
#include "move.h"

// macro's
#define abs(x) ((x) >= 0 ? (x) : -(x))
#define sqcol(x) ((x) < no_sq ? (x) & 7 : no_sq)
#define sqrow(x) ((x) < no_sq ? (x) >> 3 : no_sq)

const int promote_pieces[2][4] = {
    {Q, R, B, N},
    {q, r, b, n},
};

const int castling_rights[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

typedef struct
{
    U64 bitboards[12];      // piece bitboards
    U64 occupancies[3];     // occupancy bitboards
    U64 all_options[2];     // set bit when piece on posisition index can move
    U64 piece_options[64];  // set bit when piece on position index can move to
    U64 hash_key;           // hash key for the current board state
    int side;               // side to move
    int enpassant;          // enpassant square
    int castle;             // castling rights
    int ply;                // half move counter
    int fifty;              // fifty move rule counter
    bool incheck[2];        // flag indicator or the king is in_check
    int gameover;           // 1 = checkmate, 2 = pat
} chess_board;

class Board
{
public:
    // constructor
    Board();
    // destructor
    ~Board();
    void New_Game();
    int GetPiece(int square);
    chess_board *GetChessBoard();
    int GetMove(int sqf, int sqt);
    int GetPromotionMove(int sqf, int sqt, int piece);
    void DoMove(int move);
private:
    chess_board *chsbrd;
    hash_data *gen;
    MoveList *list;
    int knight_moves[64][8];  // [squares][directions]
    int bishop_moves[64][4];  // [squares][directions]
    int rook_moves[64][4];    // [squares][directions]
    int pawn_moves[64][2][3]; // [squares][sides][directions]

    // methods
    void Generate_move_tables();
    void Set_occupancies(chess_board *brd);
    void Reset_board(chess_board *brd);
    chess_board *Copy_board(chess_board *brd);
    U64 Generate_hash_key(hash_data *gen);
    bool IsEmptySquare(int square, chess_board *brd);
    bool IsOccupiedByOponent(int square, int side, chess_board *brd);
    bool IsSquareAttacked(int square, int xside, chess_board *brd);
    void Generate_moves(MoveList *move_list, chess_board *brd);
    void Slider_piece_move(int &sqt, chess_board *brd, int &sqf, int &piece, int side, bool &is_capture, MoveList *move_list);
    void Leaper_piece_move(int &sqt, chess_board *brd, int &sqf, int &piece, int side, MoveList *move_list);
    bool Test_move(chess_board *brd, int move);
    void Makemove(int move, chess_board *brd);
    bool IsKingInCheck(chess_board *brd, int side);
    void Print_move(int move);
    void Print_move_list(MoveList *move_list);
    
};