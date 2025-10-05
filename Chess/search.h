#pragma once

#include "board.h"
#include "bitboard.h"
#include "evaluate.h"

// ***************************************************
// Evaluate                                          *
// Using C Code from BBC by Code Monkey King         *
// Translated to C++ and objects by Peter Veenendaal *
// ***************************************************

#define infinity 50000
#define mate_value 49000
#define mate_score 48000
#define max_ply 64           // max ply that we can reach within a search
#define no_hash_entry 100000 // no hash entry found constant
// transposition table hash flags
#define hash_flag_exact 0
#define hash_flag_alpha 1
#define hash_flag_beta 2

// most valuable victim & less valuable attacker

/*

    (Victims) Pawn Knight Bishop   Rook  Queen   King
  (Attackers)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600

*/

// MVV LVA [attacker][victim]
static int mvv_lva[12][12] = {
    105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};

const int full_depth_moves = 4; // full depth moves counter
const int reduction_limit = 3;  // depth limit to consider reduction

// transposition table data structure
typedef struct
{
  U64 hash_key;  // "almost" unique chess position identifier
  int depth;     // current search depth
  int flag;      // flag the type of node (fail-low/fail-high/PV)
  int score;     // score (alpha/beta/PV)
  int best_move; // best move found
} tt;            // transposition table (TT aka hash table)

class Search
{
public:
  Search(Board *b, Evaluate *e, hash_data *g); // constructor
private:
  tt *hash_table = nullptr;       // transposition table data
  Board *brd;                     // pointer to the board object
  Evaluate *eval;                 // pointer to the evaluate object
  hash_data *gen;                 // pointer to the hash data structure
  bool stopped;                   // flag if search is over
  int nodes;                      // search counter
  int killer_moves[2][max_ply];   // killer moves [id][ply]
  int history_moves[12][64];      // history moves [piece][square]
  bool follow_pv;                 // follow PV or not
  bool score_pv;                  // set the score to PV
  int pv_length[max_ply];         // PV length [ply]
  int pv_table[max_ply][max_ply]; // PV table [ply][ply]
  int hash_entries = 0;           // number hash table entries

  // methods
  void Clear_hash_table();                                                   // clear TT (transposition table)
  void Init_hash_table(int mb);                                              // Initialize the TT (transposition table)
  int Read_hash_entry(int alpha, int beta, int *best_move, int depth);       // read an entry in the tansposition table
  void Write_hash_entry(int score, int best_move, int depth, int hash_flag); // Write an entry in the transposition table
  void Enable_pv_scoring(MoveList *move_list);                               // set follow_pv to true or false
  bool Is_repetition();                                                      // check if the current hash key is part of the repetition table 
  int Score_move(int move);                                                  // set score for a move
  void Sort_moves(MoveList *move_list, int best_move);                       // sort the moves
  void Print_move_scores(MoveList *move_list);                               // Print the score for the moves
  int Quiescence(int alpha, int beta);                                       // quiescence search
  int Negamax(int alpha, int beta, int depth);                               // negamax search
  void Communicate();                                                        // check the time or user input
};

// eof