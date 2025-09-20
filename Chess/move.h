#pragma once

#include <vector>

struct Move
{
    char sqf;
    char sqt;
    char piece_sqf;
    char piece_sqt;
    char movetype;
    char promote;
};

enum eMovetype
{
    NORMAL = 0,
    CAPTURE = 1,
    CASTLE = 2,
    ENPASSANT_CAPTURE = 4,
    PUSH_PAWN_2_SQUARES = 8,
    PAWN_MOVE = 16,
    PROMOTE = 32    
};

typedef std::vector<Move> MoveList;