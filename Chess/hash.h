#pragma once

#include "bitboard.h"

typedef struct 
{
    unsigned int random_state;
    U64 piece_keys[12][64];
    U64 enpassant_keys[64];
    U64 castle_keys[16];
    U64 side_key;
} hash_data;

extern void init_random_keys(hash_data *gen);
extern unsigned int get_random_U32_number(hash_data *gen);
extern U64 get_random_U64_number(hash_data *gen);
