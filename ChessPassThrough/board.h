#pragma once

#include <raylib.h>
#include <list>
#include <array>
#include "defs.h"
#include "piece.h"

typedef struct
{
    // draw
    bool reverse;
    int brd_col;
    int brd_row;
    // data
    std::array<Piece, 64> loc_pieces;    // 64 board
    std::array<Piece, 16> cap_pieces[2]; // max 15 captures pieces per color + empty piece per color
    std::list<int> pos_pieces[2]; // position pieces per color 
    int test_board[65];
    int cnt_cap_pieces[2];
    int test_cap_pieces[32];
    Piece captured_piece;
    int king_loc[2]; // king square per color
    int enpassant_square;
    int castle_rules;
    int player[2];
    int game_state;
    int game_over_state;
    int timer[2];        // in seconds
    int plustimer[2];    // in seconds
    int clocktime[2][5]; // for drawing
    int starttime[2];    // in ms
    int thinktime[2];    // in ms
    int side2move;
    bool promotion;
    int promotion_square_to;
    int promotion_square_from;
    int promotion_choice;
    int selection;
    U64 selection_options;
    int selection_piece;
    int selection_square;
    U64 all_options[2];
    U64 all_cap_options[2];
    int fifty_moves;
    U64 hash_key;
    U64 hash_keys[64];
    int hply;
} Board_Data;

class Board
{
private:
    Board_Data *data;

protected:
    void SetupBoard64();
    void DoMove(bool iswhite, int sqf, int sqt);
    void ChangeSide(int sqf, int sqt);
    void PrintBitboard(U64 bb, std::string title);
    bool AttackSquare(int *brd_piece, int sqr, int attack_color);
    bool TestMove(int *brd_piece, int king, int sqf, int sqt);
    bool TestPutMove(int *brd_piece, int piece, int king, int xking, int sqt);
    U64 CastleOptions(int piece);
    U64 RookOptions(int pos, int piece);
    U64 BishopOptions(int pos, int piece);
    U64 KnightOptions(int pos, int piece);
    U64 PawnOptions(int pos, int piece);
    U64 PutPieceOptions(int piece);
    void PieceOptions();   
    void TestGameOver();
    bool TestRepeatingMove();
    bool TestMaterial();

public:
    Board(bool);
    ~Board();
    Board_Data *Get();
    void Setup(int playerW, int playerB, int time, int plustime);
    void Update();
    void Draw(bool game_over_other_brd);
    void Mouse_handler(int x, int y);
    void FillBoard64();
    void CapPieceOptions();
};

extern Board *left_board;
extern Board *right_board;
