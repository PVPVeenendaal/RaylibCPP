#ifndef HASHCODE_H_
#define HASHCODE_H_

#include "defs.h"

typedef unsigned int U32;

class Hashcode
{
private:
    U32 random_state;
    U64 piece_keys[12][64];
    U64 cap_keys[12];
    U64 enpassant_keys[64];
    U64 castle_keys[16];
    U64 side_key;

protected:
    U32 Get32BitRandom();
    U64 Get64BitRandom();

public:
    Hashcode();
    U64 Get(int *board, int *cap, int enpassant, int castle, int side2move);
};

extern Hashcode *haskey_generator;

#endif