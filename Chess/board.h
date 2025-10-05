#pragma once

#include <vector>
#include "bitboard.h"
#include "hash.h"
#include "move.h"

// ****************************************************
// Board                                              *
// Using C Code from BBC by Code Monkey King          *
// Translated to C++ and objects by Peter Veenendaal  *
// ****************************************************

// macro's
#define abs(x) ((x) >= 0 ? (x) : -(x))
#define sqcol(x) ((x) < no_sq ? (x) & 7 : no_sq)
#define sqrow(x) ((x) < no_sq ? (x) >> 3 : no_sq)

const int promote_pieces[2][4] = {
    Q,
    R,
    B,
    N,
    q,
    r,
    b,
    n,
};

const int castling_rights[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14};

typedef struct
{
    U64 bitboards[12];     // piece bitboards => enum ePiece
    U64 occupancies[3];    // occupancy bitboards => enum eColor
    U64 all_options[2];    // set bit when piece on posisition square can move [side]
    U64 piece_options[64]; // set bit when piece on position square can move to [piece on square]
    U64 hash_key;          // hash key for the current board state
    int side;              // side to move
    int enpassant;         // enpassant square
    int castle;            // castling rights
    int ply;               // half move counter
    int fifty;             // fifty move rule counter
    bool incheck[2];       // flag indicator or the king is in_check [side]
    int gameover;          // 1 = checkmate, 2 = pat
} chess_board;

class Board
{
public:
    Board();                                                            // constructor
    ~Board();                                                           // destructor
    void New_Game();                                                    // begin a new game
    int GetPiece(int square);                                           // use in the gui
    chess_board *GetChessBoard();                                       // getter from the structure chess_board
    chess_board *Copy_board();                                          // keep the state of the structure chess_board
    void Take_back(chess_board *keep);                                  // set the state back of the current chess_board
    int GetMove(int sqf, int sqt);                                      // use in the gui
    int GetPromotionMove(int sqf, int sqt, int piece);                  // use in the gui
    void DoMove(int move);                                              // use in the gui
    void Print_board();                                                 // debug only
    void Generate_moves(MoveList *move_list, bool include);             // Generate the possible moves using bitboards for the pieces and the Move tables for the moves
    bool Makemove(int move, int make_move_flag);                        // Make a move to create a new position
    Hash_data_table *Get_repetiton_table() { return repetition_table; } // getter from repetition table
    int Is_Repetition();                                                // Check if the current position is a repetition => 3
    bool IsKingInCheck(int side);                                       // Check if the king form the side to move is in check

private:
    chess_board *brd = nullptr;                  // pointer to the structure chess_board
    hash_data *gen = nullptr;                    // pointer to the structure hash_data
    MoveList *list = nullptr;                    // pointer to the vector<int>
    Hash_data_table *repetition_table = nullptr; // pointer to the vector<U64>
    // Move tables
    int knight_moves[64][8];  // [squares][directions]
    int bishop_moves[64][4];  // [squares][directions]
    int rook_moves[64][4];    // [squares][directions]
    int pawn_moves[64][2][3]; // [squares][sides][directions]

    // methods
    void Generate_move_tables();                                                                                           // Fill the move tables
    void Set_occupancies();                                                                                                // Fill the occupansies
    void Reset_board();                                                                                                    // Initialize the structure chess_board
    U64 Generate_hash_key();                                                                                               // Create a hashvalue form the current position
    bool IsEmptySquare(int square);                                                                                        // Check if the square is empty for the current position
    bool IsOccupiedByOponent(int square, int side);                                                                        // Check if the square is occupied by the other side
    bool IsSquareAttacked(int square, int xside);                                                                          // Check if the sqaure is attacked by the other side
    void Slider_piece_move(bool include, int &sqt, int &sqf, int &piece, int side, bool &is_capture, MoveList *move_list); // test if a slider piece move is found (B, R, Q, b, r, q)
    void Leaper_piece_move(bool include, int &sqt, int &sqf, int &piece, int side, MoveList *move_list);                   // test if a leaper piece move is found (N, K, n, k)
    bool Test_move(int move);                                                                                              // Test if the move is valid
};

// eof