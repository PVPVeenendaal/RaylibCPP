#pragma once

#include "defs.h"

struct Piece
{
	int piecetype;
	int location;
	U64 options;
	bool promoted;
	bool incheck; // king only
};

extern Piece empty_piece;
