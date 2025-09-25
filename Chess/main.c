#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define testboard 0

// define bitboard data type
#define U64 unsigned long long

// board squares
enum
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

// encode pieces
enum
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
    k,
    no_piece
};

// sides to move (colors)
enum
{
    white,
    black,
    both
};

// bishop and rook
enum
{
    rook,
    bishop
};

// castle rights
enum
{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8
};

// ASCII pieces
char ascii_pieces[13] = "PNBRQKpnbrqk";

// convert ASCII character pieces to encoded constants
int char_pieces[] = {
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

// promoted pieces
char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n'};

// convert squares to coordinates
char *square_to_coordinates[] = {
    "a8",
    "b8",
    "c8",
    "d8",
    "e8",
    "f8",
    "g8",
    "h8",
    "a7",
    "b7",
    "c7",
    "d7",
    "e7",
    "f7",
    "g7",
    "h7",
    "a6",
    "b6",
    "c6",
    "d6",
    "e6",
    "f6",
    "g6",
    "h6",
    "a5",
    "b5",
    "c5",
    "d5",
    "e5",
    "f5",
    "g5",
    "h5",
    "a4",
    "b4",
    "c4",
    "d4",
    "e4",
    "f4",
    "g4",
    "h4",
    "a3",
    "b3",
    "c3",
    "d3",
    "e3",
    "f3",
    "g3",
    "h3",
    "a2",
    "b2",
    "c2",
    "d2",
    "e2",
    "f2",
    "g2",
    "h2",
    "a1",
    "b1",
    "c1",
    "d1",
    "e1",
    "f1",
    "g1",
    "h1",
};

// move types
enum
{
    all_moves,
    only_captures
};

// move list structure
typedef struct
{
    // moves
    int moves[512];

    // move count
    int count;
} moves;

// start position in fen string
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "

/********************************************************************\
 * chess board                                                      *
\********************************************************************/

typedef struct
{
    /* data */

    U64 bitboards[12]; // piece bitboards

    U64 occupancies[3]; // occupancy bitboards

    U64 promoted; // promote bitboard, keep track of promoted pieces

    int cap_pieces[2][31]; // captured pieces per color

    int cap_pieces_count[2]; // captured pieces counter per color

    int side; // side to move

    int enpassant; // enpassant square

    int castle; // castling rights

    U64 hash_key; // "almost" unique position identifier aka hash key or position key

    U64 repetition_table[1024]; // positions repetition table
                                // 1024 is a number of plies (512 moves) in the entire game

    int repetition_index; // repetition index

    int ply; // half move counter

    int fifty; // fifty move rule counter

    U64 options[64]; // options will be used by the gui

    U64 cap_options[2][31]; // captured options per color

} board_t;

board_t *left_board;
board_t *right_board;
board_t *keep_left;
board_t *keep_right;

/********************************************************************\ 
 * timer                                                            *
\********************************************************************/
// get time in milliseconds
int get_time_ms()
{
    int time = (int)(GetTime() * 1000.0);
    // // test
    // printf("%d ", time);
    return time;
}

/********************************************************************\
 * random numbers                                                   *
\********************************************************************/
// pseudo random number state
unsigned int random_state = 1804289383;

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = random_state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random number state
    random_state = number;

    // return random number
    return number;
}

// generate 64-bit pseudo legal numbers
U64 get_random_U64_number()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;

    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// generate magic number candidate
U64 generate_magic_number()
{
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

/**********************************\
 * bit manipulations              *
\**********************************/

// set/get/pop bit macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// count bits within a bitboard (Brian Kernighan's way)
static inline int count_bits(U64 bitboard)
{
    // bit counter
    int count = 0;

    // consecutively reset least significant 1st bit
    while (bitboard)
    {
        // increment count
        count++;

        // reset least significant 1st bit
        bitboard &= bitboard - 1;
    }

    // return bit count
    return count;
}

// get least significant 1st bit index
static inline int get_ls1b_index(U64 bitboard)
{
    // make sure bitboard is not 0
    if (bitboard)
    {
        // count trailing bits before LS1B
        return count_bits((bitboard & -bitboard) - 1);
    }

    // otherwise
    else
        // return illegal index
        return -1;
}

// print bitboard
void print_bitboard(U64 bitboard)
{
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            // print bit state (either 1 or 0)
            printf(" %d", get_bit(bitboard, square) ? 1 : 0);
        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print bitboard as unsigned decimal number
    printf("     Bitboard: %llud\n\n", bitboard);
}

// print board
void print_board(board_t *brd) // brd = left or right
{
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop ober board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            // define piece variable
            int piece = -1;

            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // if there is a piece on current square
                if (get_bit(brd->bitboards[bb_piece], square))
                    // get piece code
                    piece = bb_piece;
            }

            // print different piece set depending on OS
            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print side to move
    printf("     Side:     %s\n", !brd->side ? "white" : "black");

    // print enpassant square
    printf("     Enpassant:   %s\n", (brd->enpassant != no_sq) ? square_to_coordinates[brd->enpassant] : "no");

    // print castling rights
    printf("     Castling:  %c%c%c%c\n\n", (brd->castle & wk) ? 'K' : '-',
           (brd->castle & wq) ? 'Q' : '-',
           (brd->castle & bk) ? 'k' : '-',
           (brd->castle & bq) ? 'q' : '-');

    // print hash key
    printf("     Hash key:  %llx\n", brd->hash_key);

    // fifty move rule counter
    printf("     Fifty move: %d\n\n", brd->fifty);
}

// reset board variables
void reset_board(board_t *brd) // brd = left or right
{
    // reset board position (bitboards)
    memset(brd->bitboards, 0ULL, sizeof(brd->bitboards));

    // reset occupancies (bitboards)
    memset(brd->occupancies, 0ULL, sizeof(brd->occupancies));

    // reset promoted bitboard
    brd->promoted = 0ULL;

    // reset captured pieces
    memset(brd->cap_pieces, 0, sizeof(brd->cap_pieces));
    brd->cap_pieces_count[white] = 0;
    brd->cap_pieces_count[black] = 0;

    // reset game state variables
    brd->side = 0;
    brd->enpassant = no_sq;
    brd->castle = 0;

    // reset repetition index
    brd->repetition_index = 0;

    // reset fifty move rule counter
    brd->fifty = 0;

    // reset repetition table
    memset(brd->repetition_table, 0ULL, sizeof(brd->repetition_table));

    // reset options table
    memset(brd->options, 0ULL, sizeof(brd->options));

    // reset cap_options table
    memset(brd->cap_options, 0ULL, sizeof(brd->cap_options));
}

/**********************************\
 * zobrist keys                   *
\**********************************/
// random piece keys [piece][square] [piece][0] = black captured pieces, [piece][63] = white captured pieces
U64 piece_keys[12][64];

// random enpassant keys [square]
U64 enpassant_keys[64];

// random castling keys
U64 castle_keys[16];

// random side key
U64 side_key;

// init random hash keys
void init_random_keys()
{
    // update pseudo random number state
    random_state = 1804289383;

    // loop over piece codes
    for (int piece = P; piece <= k; piece++)
    {
        // loop over board squares
        for (int square = 0; square < 64; square++)
            // init random piece keys
            piece_keys[piece][square] = get_random_U64_number();
    }

    // loop over board squares
    for (int square = 0; square < 64; square++)
        // init random enpassant keys
        enpassant_keys[square] = get_random_U64_number();

    // loop over castling keys
    for (int index = 0; index < 16; index++)
        // init castling keys
        castle_keys[index] = get_random_U64_number();

    // init random side key
    side_key = get_random_U64_number();
}

// generate "almost" unique position ID aka hash key from scratch
U64 generate_hash_key(board_t *brd) // brd = left or right
{
    // final hash key
    U64 final_key = 0ULL;

    // temp piece bitboard copy
    U64 bitboard;

    // loop over piece bitboards
    for (int piece = P; piece <= k; piece++)
    {
        // init piece bitboard copy
        bitboard = brd->bitboards[piece];

        // loop over the pieces within a bitboard
        while (bitboard)
        {
            // init square occupied by the piece
            int square = get_ls1b_index(bitboard);

            // hash piece
            final_key ^= piece_keys[piece][square];

            // pop LS1B
            pop_bit(bitboard, square);
        }
    }

    // loop over white captured pieces
    for (int cap_idx = 0; cap_idx < brd->cap_pieces_count[white]; cap_idx++)
    {
        int piece = brd->cap_pieces[white][cap_idx];
        final_key ^= piece_keys[piece][63 - cap_idx];
    }

    // loop over black captured pieces
    for (int cap_idx = 0; cap_idx < brd->cap_pieces_count[black]; cap_idx++)
    {
        int piece = brd->cap_pieces[black][cap_idx];
        final_key ^= piece_keys[piece][cap_idx];
    }

    // if enpassant square is on board
    if (brd->enpassant != no_sq)
        // hash enpassant
        final_key ^= enpassant_keys[brd->enpassant];

    // hash castling rights
    final_key ^= castle_keys[brd->castle];

    // hash the side only if black is to move
    if (brd->side == black)
        final_key ^= side_key;

    // return generated hash key
    return final_key;
}

// parse FEN string
void parse_fen(char *fen, board_t *brd) // brd = left or right
{
    // prepare for new game
    reset_board(brd);

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init current square
            int square = rank * 8 + file;

            // match ascii pieces within FEN string
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                // init piece type
                int piece = char_pieces[*fen];

                // set piece on corresponding bitboard
                set_bit(brd->bitboards[piece], square);

                // increment pointer to FEN string
                fen++;
            }

            // match empty square numbers within FEN string
            if (*fen >= '0' && *fen <= '9')
            {
                // init offset (convert char 0 to int 0)
                int offset = *fen - '0';

                // define piece variable
                int piece = -1;

                // loop over all piece bitboards
                for (int bb_piece = P; bb_piece <= k; bb_piece++)
                {
                    // if there is a piece on current square
                    if (get_bit(brd->bitboards[bb_piece], square))
                        // get piece code
                        piece = bb_piece;
                }

                // on empty current square
                if (piece == -1)
                    // decrement file
                    file--;

                // adjust file counter
                file += offset;

                // increment pointer to FEN string
                fen++;
            }

            // match rank separator
            if (*fen == '/')
                // increment pointer to FEN string
                fen++;
        }
    }

    // got to parsing side to move (increment pointer to FEN string)
    fen++;

    // parse side to move
    (*fen == 'w') ? (brd->side = white) : (brd->side = black);

    // go to parsing castling rights
    fen += 2;

    // parse castling rights
    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K':
            brd->castle |= wk;
            break;
        case 'Q':
            brd->castle |= wq;
            break;
        case 'k':
            brd->castle |= bk;
            break;
        case 'q':
            brd->castle |= bq;
            break;
        case '-':
            break;
        }

        // increment pointer to FEN string
        fen++;
    }

    // go to parsing enpassant square (increment pointer to FEN string)
    fen++;

    // parse enpassant square
    if (*fen != '-')
    {
        // parse enpassant file & rank
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');

        // init enpassant square
        brd->enpassant = rank * 8 + file;
    }

    // no enpassant square
    else
        brd->enpassant = no_sq;

    // go to parsing half move counter (increment pointer to FEN string)
    fen++;

    // parse half move counter to init fifty move counter
    brd->fifty = atoi(fen);

    // loop over white pieces bitboards
    for (int piece = P; piece <= K; piece++)
        // populate white occupancy bitboard
        brd->occupancies[white] |= brd->bitboards[piece];

    // loop over black pieces bitboards
    for (int piece = p; piece <= k; piece++)
        // populate white occupancy bitboard
        brd->occupancies[black] |= brd->bitboards[piece];

    // init all occupancies
    brd->occupancies[both] |= brd->occupancies[white];
    brd->occupancies[both] |= brd->occupancies[black];

    // init hash key
    brd->hash_key = generate_hash_key(brd);
}

// preserve board state
void copy_board(board_t *brd, board_t *keep) // brd,keep = left or right
{
    memcpy(keep->bitboards, brd->bitboards, sizeof(brd->bitboards));
    memcpy(keep->occupancies, brd->occupancies, sizeof(brd->occupancies));
    memcpy(keep->options, brd->options, sizeof(brd->options));
    memcpy(keep->cap_pieces, brd->cap_pieces, sizeof(brd->cap_pieces));
    memcpy(keep->cap_options, brd->cap_options, sizeof(brd->cap_options));
    memcpy(keep->repetition_table, brd->repetition_table, sizeof(brd->repetition_table));
    keep->cap_pieces_count[white] = brd->cap_pieces_count[white];
    keep->cap_pieces_count[black] = brd->cap_pieces_count[black];
    keep->side = brd->side;
    keep->enpassant = brd->enpassant;
    keep->castle = brd->castle;
    keep->fifty = brd->fifty;
    keep->hash_key = brd->hash_key;
    keep->ply = brd->ply;
    keep->promoted = brd->promoted;
    keep->repetition_index = brd->repetition_index;
}

// restore board state
void take_back(board_t *brd, board_t *keep) // brd,keep = left or right
{
    memcpy(brd->bitboards, keep->bitboards, sizeof(brd->bitboards));
    memcpy(brd->occupancies, keep->occupancies, sizeof(brd->occupancies));
    memcpy(brd->options, keep->options, sizeof(brd->options));
    memcpy(brd->cap_pieces, keep->cap_pieces, sizeof(brd->cap_pieces));
    memcpy(brd->cap_options, keep->cap_options, sizeof(brd->cap_options));
    memcpy(brd->repetition_table, keep->repetition_table, sizeof(brd->repetition_table));
    brd->cap_pieces_count[white] = keep->cap_pieces_count[white];
    brd->cap_pieces_count[black] = keep->cap_pieces_count[black];
    brd->side = keep->side;
    brd->enpassant = keep->enpassant;
    brd->castle = keep->castle;
    brd->fifty = keep->fifty;
    brd->hash_key = keep->hash_key;
    brd->ply = keep->ply;
    brd->promoted = keep->promoted;
    brd->repetition_index = keep->repetition_index;
}

/**********************************\
 * attacks                        *
\**********************************/
// not A file constant
const U64 not_a_file = 18374403900871474942ULL;

// not H file constant
const U64 not_h_file = 9187201950435737471ULL;

// not HG file constant
const U64 not_hg_file = 4557430888798830399ULL;

// not AB file constant
const U64 not_ab_file = 18229723555195321596ULL;

// bishop relevant occupancy bit count for every square on board
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6};

// rook relevant occupancy bit count for every square on board
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12};

// rook magic numbers
U64 rook_magic_numbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL};

// bishop magic numbers
U64 bishop_magic_numbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL};

// pawn attacks table [side][square]
U64 pawn_attacks[2][64];

// knight attacks table [square]
U64 knight_attacks[64];

// king attacks table [square]
U64 king_attacks[64];

// bishop attack masks
U64 bishop_masks[64];

// rook attack masks
U64 rook_masks[64];

// bishop attacks table [square][occupancies]
U64 bishop_attacks[64][512];

// rook attacks rable [square][occupancies]
U64 rook_attacks[64][4096];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white pawns
    if (!side)
    {
        // generate pawn attacks
        if ((bitboard >> 7) & not_a_file)
            attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file)
            attacks |= (bitboard >> 9);
    }

    // black pawns
    else
    {
        // generate pawn attacks
        if ((bitboard << 7) & not_h_file)
            attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file)
            attacks |= (bitboard << 9);
    }

    // return attack map
    return attacks;
}

// generate knight attacks
U64 mask_knight_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate knight attacks
    if ((bitboard >> 17) & not_h_file)
        attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file)
        attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file)
        attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file)
        attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file)
        attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file)
        attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file)
        attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file)
        attacks |= (bitboard << 6);

    // return attack map
    return attacks;
}

// generate king attacks
U64 mask_king_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate king attacks
    if (bitboard >> 8)
        attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file)
        attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file)
        attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file)
        attacks |= (bitboard >> 1);
    if (bitboard << 8)
        attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file)
        attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file)
        attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file)
        attacks |= (bitboard << 1);

    // return attack map
    return attacks;
}

// mask bishop attacks
U64 mask_bishop_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square >> 3;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));

    // return attack map
    return attacks;
}

// mask rook attacks
U64 mask_rook_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square >> 3;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    // return attack map
    return attacks;
}

// generate bishop attacks on the fly
U64 bishop_attacks_on_the_fly(int square, U64 block)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square >> 3;
    int tf = square % 8;

    // generate bishop atacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    // return attack map
    return attacks;
}

// generate rook attacks on the fly
U64 rook_attacks_on_the_fly(int square, U64 block)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // init ranks & files
    int r, f;

    // init target rank & files
    int tr = square >> 3;
    int tf = square % 8;

    // generate rook attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    // return attack map
    return attacks;
}

// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

// set occupancies
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{
    // occupancy map
    U64 occupancy = 0ULL;

    // loop over the range of bits within attack mask
    for (int count = 0; count < bits_in_mask; count++)
    {
        // get LS1B index of attacks mask
        int square = get_ls1b_index(attack_mask);

        // pop LS1B in attack map
        pop_bit(attack_mask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }

    // return occupancy map
    return occupancy;
}

// magics
// find appropriate magic number
U64 find_magic_number(int square, int relevant_bits, int bishop)
{
    // init occupancies
    U64 occupancies[4096];

    // init attack tables
    U64 attacks[4096];

    // init used attacks
    U64 used_attacks[4096];

    // init attack mask for a current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // init occupancy indices
    int occupancy_indices = 1 << relevant_bits;

    // loop over occupancy indices
    for (int index = 0; index < occupancy_indices; index++)
    {
        // init occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        // init attacks
        attacks[index] = bishop ? bishop_attacks_on_the_fly(square, occupancies[index]) : rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidate
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6)
            continue;

        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indices; index++)
        {
            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];

            // otherwise
            else if (used_attacks[magic_index] != attacks[index])
                // magic index doesn't work
                fail = 1;
        }

        // if magic number works
        if (!fail)
            // return it
            return magic_number;
    }

    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

// init magic numbers
void init_magic_numbers()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
        // init rook magic numbers
        rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], rook);

    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
        // init bishop magic numbers
        bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], bishop);
}

// init slider piece's attack tables
void init_sliders_attacks(int bishop)
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        // init current mask
        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        // init relevant occupancy bit count
        int relevant_bits_count = count_bits(attack_mask);

        // init occupancy indices
        int occupancy_indices = (1 << relevant_bits_count);

        // loop over occupancy indices
        for (int index = 0; index < occupancy_indices; index++)
        {
            // bishop
            if (bishop)
            {
                // init current occupancy variation
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                // init magic index
                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

                // init bishop attacks
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }

            // rook
            else
            {
                // init current occupancy variation
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                // init magic index
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

                // init rook attacks
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

// get bishop attacks
static inline U64 get_bishop_attacks(int square, U64 occupancy)
{
    // get bishop attacks assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    // return bishop attacks
    return bishop_attacks[square][occupancy];
}

// get rook attacks
static inline U64 get_rook_attacks(int square, U64 occupancy)
{
    // get rook attacks assuming current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    // return rook attacks
    return rook_attacks[square][occupancy];
}

// get queen attacks
static inline U64 get_queen_attacks(int square, U64 occupancy)
{
    // init result attacks bitboard
    U64 queen_attacks = 0ULL;

    // init bishop occupancies
    U64 bishop_occupancy = occupancy;

    // init rook occupancies
    U64 rook_occupancy = occupancy;

    // get bishop attacks assuming current board occupancy
    bishop_occupancy &= bishop_masks[square];
    bishop_occupancy *= bishop_magic_numbers[square];
    bishop_occupancy >>= 64 - bishop_relevant_bits[square];

    // get bishop attacks
    queen_attacks = bishop_attacks[square][bishop_occupancy];

    // get rook attacks assuming current board occupancy
    rook_occupancy &= rook_masks[square];
    rook_occupancy *= rook_magic_numbers[square];
    rook_occupancy >>= 64 - rook_relevant_bits[square];

    // get rook attacks
    queen_attacks |= rook_attacks[square][rook_occupancy];

    // return queen attacks
    return queen_attacks;
}

/**********************************\
 * move generator                 *
\**********************************/
// is square current given attacked by the current given side
static inline int is_square_attacked(int square, int side, board_t *brd) // brd = left or right
{
    // attacked by white pawns
    if ((side == white) && (pawn_attacks[black][square] & brd->bitboards[P]))
        return 1;

    // attacked by black pawns
    if ((side == black) && (pawn_attacks[white][square] & brd->bitboards[p]))
        return 1;

    // attacked by knights
    if (knight_attacks[square] & ((side == white) ? brd->bitboards[N] : brd->bitboards[n]))
        return 1;

    // attacked by bishops
    if (get_bishop_attacks(square, brd->occupancies[both]) & ((side == white) ? brd->bitboards[B] : brd->bitboards[b]))
        return 1;

    // attacked by rooks
    if (get_rook_attacks(square, brd->occupancies[both]) & ((side == white) ? brd->bitboards[R] : brd->bitboards[r]))
        return 1;

    // attacked by bishops
    if (get_queen_attacks(square, brd->occupancies[both]) & ((side == white) ? brd->bitboards[Q] : brd->bitboards[q]))
        return 1;

    // attacked by kings
    if (king_attacks[square] & ((side == white) ? brd->bitboards[K] : brd->bitboards[k]))
        return 1;

    // by default return false
    return 0;
}

// print attacked squares
void print_attacked_squares(int side, board_t *brd) // brd = left or right
{
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);

            // check whether current square is attacked or not
            printf(" %d", is_square_attacked(square, side, brd) ? 1 : 0);
        }

        // print new line every rank
        printf("\n");
    }

    // print files
    printf("\n     a b c d e f g h\n\n");
}

// encode move
#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
    (source) |                                                                             \
        (target << 6) |                                                                    \
        (piece << 12) |                                                                    \
        (promoted << 16) |                                                                 \
        (capture << 20) |                                                                  \
        (double << 21) |                                                                   \
        (enpassant << 22) |                                                                \
        (castling << 23)

// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)

// extract double pawn push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)

// extract castling flag
#define get_move_castling(move) (move & 0x800000)

// add move to the move list
static inline void add_move(moves *move_list, int move)
{
    // strore move
    move_list->moves[move_list->count] = move;

    // increment move count
    move_list->count++;
}

// print move (for UCI purposes)
void print_move(int move)
{
    if (get_move_promoted(move))
        printf("%s%s%c", square_to_coordinates[get_move_source(move)],
               square_to_coordinates[get_move_target(move)],
               promoted_pieces[get_move_promoted(move)]);
    else
        printf("%s%s", square_to_coordinates[get_move_source(move)],
               square_to_coordinates[get_move_target(move)]);
}

// print move list
void print_move_list(moves *move_list)
{
    // do nothing on empty move list
    if (!move_list->count)
    {
        printf("\n     No move in the move list!\n");
        return;
    }

    printf("\n     move    piece     capture   double    enpass    castling\n\n");

    // loop over moves within a move list
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        // init move
        int move = move_list->moves[move_count];

        // print move
        printf("      %s%s%c   %c         %d         %d         %d         %d\n", square_to_coordinates[get_move_source(move)],
               square_to_coordinates[get_move_target(move)],
               get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
               ascii_pieces[get_move_piece(move)],
               get_move_capture(move) ? 1 : 0,
               get_move_double(move) ? 1 : 0,
               get_move_enpassant(move) ? 1 : 0,
               get_move_castling(move) ? 1 : 0);
    }

    // print total number of moves
    printf("\n\n     Total number of moves: %d\n\n", move_list->count);
}

// castling rights update constants
const int castling_rights[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14};

// make move on chess board
static inline int make_move(int move, int move_flag, board_t *brd, board_t *keep) // brd,keep = left or right
{
    // quiet moves
    if (move_flag == all_moves)
    {
        // preserve board state
        copy_board(brd, keep);

        // parse move
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double(move);
        int enpass = get_move_enpassant(move);
        int castling = get_move_castling(move);

        // move piece
        pop_bit(brd->bitboards[piece], source_square);
        set_bit(brd->bitboards[piece], target_square);
        if (get_bit(brd->promoted, source_square))
        {
            pop_bit(brd->promoted, source_square);
            set_bit(brd->promoted, target_square);
        }

        // hash piece
        brd->hash_key ^= piece_keys[piece][source_square]; // remove piece from source square in hash key
        brd->hash_key ^= piece_keys[piece][target_square]; // set piece to the target square in hash key

        // increment fifty move rule counter
        brd->fifty++;

        // if pawn moved
        if (piece == P || piece == p)
            // reset fifty move rule counter
            brd->fifty = 0;

        // handling capture moves
        if (capture)
        {
            // reset fifty move rule counter
            brd->fifty = 0;

            // pick up bitboard piece index ranges depending on side
            int start_piece, end_piece;

            // white to move
            if (brd->side == white)
            {
                start_piece = p;
                end_piece = k;
            }

            // black to move
            else
            {
                start_piece = P;
                end_piece = K;
            }

            // loop over bitboards opposite to the current side to move
            for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
            {
                // if there's a piece on the target square
                if (get_bit(brd->bitboards[bb_piece], target_square))
                {
                    // remove it from corresponding bitboard
                    pop_bit(brd->bitboards[bb_piece], target_square);

                    // remove the piece from hash key
                    brd->hash_key ^= piece_keys[bb_piece][target_square];
                    break;
                }
            }
        }

        // handle pawn promotions
        if (promoted_piece)
        {
            // white to move
            if (brd->side == white)
            {
                // erase the pawn from the target square
                pop_bit(brd->bitboards[P], target_square);

                // remove pawn from hash key
                brd->hash_key ^= piece_keys[P][target_square];
            }

            // black to move
            else
            {
                // erase the pawn from the target square
                pop_bit(brd->bitboards[p], target_square);

                // remove pawn from hash key
                brd->hash_key ^= piece_keys[p][target_square];
            }

            // set up promoted piece on chess board
            set_bit(brd->bitboards[promoted_piece], target_square);
            set_bit(brd->promoted, target_square);

            // add promoted piece into the hash key
            brd->hash_key ^= piece_keys[promoted_piece][target_square];
        }

        // handle enpassant captures
        if (enpass)
        {
            // erase the pawn depending on side to move
            (brd->side == white) ? pop_bit(brd->bitboards[p], target_square + 8) : pop_bit(brd->bitboards[P], target_square - 8);

            // white to move
            if (brd->side == white)
            {
                // remove captured pawn
                pop_bit(brd->bitboards[p], target_square + 8);

                // remove pawn from hash key
                brd->hash_key ^= piece_keys[p][target_square + 8];
            }

            // black to move
            else
            {
                // remove captured pawn
                pop_bit(brd->bitboards[P], target_square - 8);

                // remove pawn from hash key
                brd->hash_key ^= piece_keys[P][target_square - 8];
            }
        }

        // hash enpassant if available (remove enpassant square from hash key )
        if (brd->enpassant != no_sq)
            brd->hash_key ^= enpassant_keys[brd->enpassant];

        // reset enpassant square
        brd->enpassant = no_sq;

        // handle double pawn push
        if (double_push)
        {
            // white to move
            if (brd->side == white)
            {
                // set enpassant square
                brd->enpassant = target_square + 8;

                // hash enpassant
                brd->hash_key ^= enpassant_keys[target_square + 8];
            }

            // black to move
            else
            {
                // set enpassant square
                brd->enpassant = target_square - 8;

                // hash enpassant
                brd->hash_key ^= enpassant_keys[target_square - 8];
            }
        }

        // handle castling moves
        if (castling)
        {
            // switch target square
            switch (target_square)
            {
            // white castles king side
            case (g1):
                // move H rook
                pop_bit(brd->bitboards[R], h1);
                set_bit(brd->bitboards[R], f1);

                // hash rook
                brd->hash_key ^= piece_keys[R][h1]; // remove rook from h1 from hash key
                brd->hash_key ^= piece_keys[R][f1]; // put rook on f1 into a hash key
                break;

            // white castles queen side
            case (c1):
                // move A rook
                pop_bit(brd->bitboards[R], a1);
                set_bit(brd->bitboards[R], d1);

                // hash rook
                brd->hash_key ^= piece_keys[R][a1]; // remove rook from a1 from hash key
                brd->hash_key ^= piece_keys[R][d1]; // put rook on d1 into a hash key
                break;

            // black castles king side
            case (g8):
                // move H rook
                pop_bit(brd->bitboards[r], h8);
                set_bit(brd->bitboards[r], f8);

                // hash rook
                brd->hash_key ^= piece_keys[r][h8]; // remove rook from h8 from hash key
                brd->hash_key ^= piece_keys[r][f8]; // put rook on f8 into a hash key
                break;

            // black castles queen side
            case (c8):
                // move A rook
                pop_bit(brd->bitboards[r], a8);
                set_bit(brd->bitboards[r], d8);

                // hash rook
                brd->hash_key ^= piece_keys[r][a8]; // remove rook from a8 from hash key
                brd->hash_key ^= piece_keys[r][d8]; // put rook on d8 into a hash key
                break;
            }
        }

        // hash castling
        brd->hash_key ^= castle_keys[brd->castle];

        // update castling rights
        brd->castle &= castling_rights[source_square];
        brd->castle &= castling_rights[target_square];

        // hash castling
        brd->hash_key ^= castle_keys[brd->castle];

        // reset occupancies
        memset(brd->occupancies, 0ULL, sizeof(brd->occupancies));

        // loop over white pieces bitboards
        for (int bb_piece = P; bb_piece <= K; bb_piece++)
            // update white occupancies
            brd->occupancies[white] |= brd->bitboards[bb_piece];

        // loop over black pieces bitboards
        for (int bb_piece = p; bb_piece <= k; bb_piece++)
            // update black occupancies
            brd->occupancies[black] |= brd->bitboards[bb_piece];

        // update both sides occupancies
        brd->occupancies[both] |= brd->occupancies[white];
        brd->occupancies[both] |= brd->occupancies[black];

        // change side
        brd->side ^= 1;

        // hash side
        if (brd->side == black)
            brd->hash_key ^= side_key;
        
        // make sure that king has not been exposed into a check
        if (is_square_attacked((brd->side == white) ? get_ls1b_index(brd->bitboards[k]) : get_ls1b_index(brd->bitboards[K]), brd->side, brd))
        {
            // take move back
            take_back(brd, keep);

            // return illegal move
            return 0;
        }

        // otherwise
        else
            // return legal move
            return 1;
    }

    // capture moves
    else
    {
        // make sure move is the capture
        if (get_move_capture(move))
            make_move(move, all_moves, brd, keep);

        // otherwise the move is not a capture
        else
            // don't make it
            return 0;
    }
    return 1;
}

// test move on a copy of the chess board for valid options
static inline int test_move(int move, board_t *brd) // brd,keep = left or right
{
    // parse move
    int source_square = get_move_source(move);
    int target_square = get_move_target(move);
    int piece = get_move_piece(move);
    int promoted_piece = get_move_promoted(move);
    int capture = get_move_capture(move);
    int enpass = get_move_enpassant(move);
    int castling = get_move_castling(move);

    // move piece
    pop_bit(brd->bitboards[piece], source_square);
    set_bit(brd->bitboards[piece], target_square);

    // handling capture moves
    if (capture)
    {
        // pick up bitboard piece index ranges depending on side
        int start_piece, end_piece;

        // white to move
        if (brd->side == white)
        {
            start_piece = p;
            end_piece = k;
        }
        // black to move
        else
        {
            start_piece = P;
            end_piece = K;
        }

        // loop over bitboards opposite to the current side to move
        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
        {
            // if there's a piece on the target square
            if (get_bit(brd->bitboards[bb_piece], target_square))
            {
                // remove it from corresponding bitboard
                pop_bit(brd->bitboards[bb_piece], target_square);
                break;
            }
        }
    }

    // handle pawn promotions
    if (promoted_piece)
    {
        // white to move
        if (brd->side == white)
            // erase the pawn from the target square
            pop_bit(brd->bitboards[P], target_square);
        // black to move
        else
            // erase the pawn from the target square
            pop_bit(brd->bitboards[p], target_square);

        // set up promoted piece on chess board
        set_bit(brd->bitboards[promoted_piece], target_square);
        set_bit(brd->promoted, target_square);
    }

    // handle enpassant captures
    if (enpass)
    {
        // erase the pawn depending on side to move
        (brd->side == white) ? pop_bit(brd->bitboards[p], target_square + 8) : pop_bit(brd->bitboards[P], target_square - 8);

        // white to move
        if (brd->side == white)
            // remove captured pawn
            pop_bit(brd->bitboards[p], target_square + 8);
        // black to move
        else
            // remove captured pawn
            pop_bit(brd->bitboards[P], target_square - 8);
    }

    // handle castling moves
    if (castling)
    {
        // switch target square
        switch (target_square)
        {
        // white castles king side
        case (g1):
            // move H rook
            pop_bit(brd->bitboards[R], h1);
            set_bit(brd->bitboards[R], f1);
            break;

        // white castles queen side
        case (c1):
            // move A rook
            pop_bit(brd->bitboards[R], a1);
            set_bit(brd->bitboards[R], d1);
            break;

        // black castles king side
        case (g8):
            // move H rook
            pop_bit(brd->bitboards[r], h8);
            set_bit(brd->bitboards[r], f8);
            break;

        // black castles queen side
        case (c8):
            // move A rook
            pop_bit(brd->bitboards[r], a8);
            set_bit(brd->bitboards[r], d8);
            break;
        }
    }

    // reset occupancies
    memset(brd->occupancies, 0ULL, sizeof(brd->occupancies));

    // loop over white pieces bitboards
    for (int bb_piece = P; bb_piece <= K; bb_piece++)
        // update white occupancies
        brd->occupancies[white] |= brd->bitboards[bb_piece];

    // loop over black pieces bitboards
    for (int bb_piece = p; bb_piece <= k; bb_piece++)
        // update black occupancies
        brd->occupancies[black] |= brd->bitboards[bb_piece];

    // update both sides occupancies
    brd->occupancies[both] |= brd->occupancies[white];
    brd->occupancies[both] |= brd->occupancies[black];

    // change side
    brd->side ^= 1;

    // make sure that king has not been exposed into a check
    if (is_square_attacked((brd->side == white) ? get_ls1b_index(brd->bitboards[k]) : get_ls1b_index(brd->bitboards[K]), brd->side, brd))
        return 0;
    else
        return 1;
}

// generate all moves
static inline void generate_moves(moves *move_list, board_t *brd) // brd = left or right
{
    // init move count
    move_list->count = 0;

    // define source & target squares
    int source_square, target_square;

    // define current piece's bitboard copy & it's attacks
    U64 bitboard, attacks;

    // loop over all the bitboards
    for (int piece = P; piece <= k; piece++)
    {
        // init piece bitboard copy
        bitboard = brd->bitboards[piece];

        // generate white pawns & white king castling moves
        if (brd->side == white)
        {
            // pick up white pawn bitboards index
            if (piece == P)
            {
                // loop over white pawns within white pawn bitboard
                while (bitboard)
                {
                    // init source square
                    source_square = get_ls1b_index(bitboard);

                    // init target square
                    target_square = source_square - 8;

                    // generate quiet pawn moves
                    if (!(target_square < a8) && !get_bit(brd->occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((source_square >= a2 && source_square <= h2) && !get_bit(brd->occupancies[both], target_square - 8))
                                add_move(move_list, encode_move(source_square, (target_square - 8), piece, 0, 0, 1, 0, 0));
                        }
                    }

                    // init pawn attacks bitboard
                    attacks = pawn_attacks[brd->side][source_square] & brd->occupancies[black];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_ls1b_index(attacks);

                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        }

                        else
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        // pop ls1b of the pawn attacks
                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (brd->enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant square (bit)
                        U64 enpassant_attacks = pawn_attacks[brd->side][source_square] & (1ULL << brd->enpassant);

                        // make sure enpassant capture available
                        if (enpassant_attacks)
                        {
                            // init enpassant capture target square
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            // castling moves
            if (piece == K)
            {
                // king side castling is available
                if (brd->castle & wk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(brd->occupancies[both], f1) && !get_bit(brd->occupancies[both], g1))
                    {
                        // make sure king and the f1 squares are not under attacks
                        if (!is_square_attacked(e1, black, brd) && !is_square_attacked(f1, black, brd) && !is_square_attacked(g1, black, brd))
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                    }
                }

                // queen side castling is available
                if (brd->castle & wq)
                {
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(brd->occupancies[both], d1) && !get_bit(brd->occupancies[both], c1) && !get_bit(brd->occupancies[both], b1))
                    {
                        // make sure king and the d1 squares are not under attacks
                        if (!is_square_attacked(e1, black, brd) && !is_square_attacked(d1, black, brd) && !is_square_attacked(c1, black, brd))
                            add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        // generate black pawns & black king castling moves
        else
        {
            // pick up black pawn bitboards index
            if (piece == p)
            {
                // loop over white pawns within white pawn bitboard
                while (bitboard)
                {
                    // init source square
                    source_square = get_ls1b_index(bitboard);

                    // init target square
                    target_square = source_square + 8;

                    // generate quiet pawn moves
                    if (!(target_square > h1) && !get_bit(brd->occupancies[both], target_square))
                    {
                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((source_square >= a7 && source_square <= h7) && !get_bit(brd->occupancies[both], target_square + 8))
                                add_move(move_list, encode_move(source_square, (target_square + 8), piece, 0, 0, 1, 0, 0));
                        }
                    }

                    // init pawn attacks bitboard
                    attacks = pawn_attacks[brd->side][source_square] & brd->occupancies[white];

                    // generate pawn captures
                    while (attacks)
                    {
                        // init target square
                        target_square = get_ls1b_index(attacks);

                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2)
                        {
                            add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        }

                        else
                            // one square ahead pawn move
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        // pop ls1b of the pawn attacks
                        pop_bit(attacks, target_square);
                    }

                    // generate enpassant captures
                    if (brd->enpassant != no_sq)
                    {
                        // lookup pawn attacks and bitwise AND with enpassant square (bit)
                        U64 enpassant_attacks = pawn_attacks[brd->side][source_square] & (1ULL << brd->enpassant);

                        // make sure enpassant capture available
                        if (enpassant_attacks)
                        {
                            // init enpassant capture target square
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

            // castling moves
            if (piece == k)
            {
                // king side castling is available
                if (brd->castle & bk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(brd->occupancies[both], f8) && !get_bit(brd->occupancies[both], g8))
                    {
                        // make sure king and the f8 squares are not under attacks
                        if (!is_square_attacked(e8, white, brd) && !is_square_attacked(f8, white, brd) && !is_square_attacked(g8, white, brd))
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }

                // queen side castling is available
                if (brd->castle & bq)
                {
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(brd->occupancies[both], d8) && !get_bit(brd->occupancies[both], c8) && !get_bit(brd->occupancies[both], b8))
                    {
                        // make sure king and the d8 squares are not under attacks
                        if (!is_square_attacked(e8, white, brd) && !is_square_attacked(d8, white, brd) && !is_square_attacked(c8, white, brd))
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        // genarate knight moves
        if ((brd->side == white) ? piece == N : piece == n)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_ls1b_index(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = knight_attacks[source_square] & ((brd->side == white) ? ~brd->occupancies[white] : ~brd->occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_ls1b_index(attacks);

                    // quiet move
                    if (!get_bit(((brd->side == white) ? brd->occupancies[black] : brd->occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate bishop moves
        if ((brd->side == white) ? piece == B : piece == b)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_ls1b_index(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_bishop_attacks(source_square, brd->occupancies[both]) & ((brd->side == white) ? ~brd->occupancies[white] : ~brd->occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_ls1b_index(attacks);

                    // quiet move
                    if (!get_bit(((brd->side == white) ? brd->occupancies[black] : brd->occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate rook moves
        if ((brd->side == white) ? piece == R : piece == r)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_ls1b_index(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_rook_attacks(source_square, brd->occupancies[both]) & ((brd->side == white) ? ~brd->occupancies[white] : ~brd->occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_ls1b_index(attacks);

                    // quiet move
                    if (!get_bit(((brd->side == white) ? brd->occupancies[black] : brd->occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate queen moves
        if ((brd->side == white) ? piece == Q : piece == q)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_ls1b_index(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = get_queen_attacks(source_square, brd->occupancies[both]) & ((brd->side == white) ? ~brd->occupancies[white] : ~brd->occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_ls1b_index(attacks);

                    // quiet move
                    if (!get_bit(((brd->side == white) ? brd->occupancies[black] : brd->occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }

        // generate king moves
        if ((brd->side == white) ? piece == K : piece == k)
        {
            // loop over source squares of piece bitboard copy
            while (bitboard)
            {
                // init source square
                source_square = get_ls1b_index(bitboard);

                // init piece attacks in order to get set of target squares
                attacks = king_attacks[source_square] & ((brd->side == white) ? ~brd->occupancies[white] : ~brd->occupancies[black]);

                // loop over target squares available from generated attacks
                while (attacks)
                {
                    // init target square
                    target_square = get_ls1b_index(attacks);

                    // quiet move
                    if (!get_bit(((brd->side == white) ? brd->occupancies[black] : brd->occupancies[white]), target_square))
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                    else
                        // capture move
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    // pop ls1b in current attacks set
                    pop_bit(attacks, target_square);
                }

                // pop ls1b of the current piece bitboard copy
                pop_bit(bitboard, source_square);
            }
        }
    }
}

// init all variables
void init_all()
{
    // init leaper pieces attacks
    init_leapers_attacks();

    // init slider pieces attacks
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    // init random keys for hashing purposes
    init_random_keys();
}

void moves_to_options(moves *move_list, board_t *brd, board_t *keep) // brd, keep = left or right
{
    if (!move_list->count)
    {
        printf("\n     No move in the move list!\n");
        return;
    }
    for (int square = 0; square < 64; square++)
    {
        brd->options[square] = 0ULL;
    }
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        // init move
        int move = move_list->moves[move_count];

        int sqf = get_move_source(move);
        int sqt = get_move_target(move);
        copy_board(brd, keep);

        if (test_move(move, keep))
            set_bit(brd->options[sqf], sqt);
    }
}

void print_options(board_t *brd) // brd = left or right
{
    for (int square = 0; square < 64; square++)
    {
        if (brd->options[square] == 0ULL)
            continue;
        printf("%s:\n", square_to_coordinates[square]);
        print_bitboard(brd->options[square]);
    }
}

/********************************************************************\
 * Gui
\********************************************************************/
// sizes
#define squaresize 72
#define halfsquaresize 36
#define screenwidth 1440
#define screenheight 864
#define boardsize 576
#define large_piece_size 64
#define small_piece_size 32

// players
enum
{
    human,
    ai
};

// gamestate
enum
{
    input,
    running,
    gameover
};

// vars
// key
enum
{
    none,
    f9,
    f10,
    f11
};

// gameover state
enum
{
    go_none,
    go_wmate,    // white wins black is mated
    go_wtime,    // white wins black is out of time
    go_bmate,    // black wins white is mated
    go_btime,    // black wins white is out of time
    go_drawmat,  // draw by material
    go_drawpatw, // draw by white king is pat
    go_drawpatb, // draw by black king is pat
    go_drawrep,  // draw by repeat moves rule
    go_draw50    // draw by 50 moves rule
};

// selection
enum
{
    input_piece,
    input_square,
    input_cap_piece,
};

// image containers
Texture2D img_table;            // shows the background
Texture2D img_chess_board;      // shows the chessboard
Texture2D img_large_pieces[12]; // the large pieces P..k, show pieces on the board
Texture2D img_small_pieces[12]; // the small pieces P..l, show captured pieces
Texture2D img_players[2];       // shows the player per color
Texture2D img_enter_btn;        // shows the enter button
Texture2D img_plus_min_btn;     // shows the plus-min button
Texture2D img_choice_btn;       // shows black/white color
Texture2D img_chess_clock;      // shows the chess clock

/// @brief load the images
/// @param img pointer to the imagedata
void load_images()
{
    img_table = LoadTexture("assets/images/Table.png");
    img_table.width = screenwidth;
    img_table.height = screenheight;
    img_chess_board = LoadTexture("assets/images/Board.png");
    img_chess_board.width = boardsize;
    img_chess_board.height = boardsize;
    img_large_pieces[P] = LoadTexture("assets/images/PawnW.png");
    img_small_pieces[P] = LoadTexture("assets/images/PawnW.png");
    img_large_pieces[N] = LoadTexture("assets/images/KnightW.png");
    img_small_pieces[N] = LoadTexture("assets/images/KnightW.png");
    img_large_pieces[B] = LoadTexture("assets/images/BishopW.png");
    img_small_pieces[B] = LoadTexture("assets/images/BishopW.png");
    img_large_pieces[R] = LoadTexture("assets/images/RookW.png");
    img_small_pieces[R] = LoadTexture("assets/images/RookW.png");
    img_large_pieces[Q] = LoadTexture("assets/images/QueenW.png");
    img_small_pieces[Q] = LoadTexture("assets/images/QueenW.png");
    img_large_pieces[K] = LoadTexture("assets/images/KingW.png");
    img_small_pieces[K] = LoadTexture("assets/images/KingW.png");
    img_large_pieces[p] = LoadTexture("assets/images/PawnB.png");
    img_small_pieces[p] = LoadTexture("assets/images/PawnB.png");
    img_large_pieces[n] = LoadTexture("assets/images/KnightB.png");
    img_small_pieces[n] = LoadTexture("assets/images/KnightB.png");
    img_large_pieces[b] = LoadTexture("assets/images/BishopB.png");
    img_small_pieces[b] = LoadTexture("assets/images/BishopB.png");
    img_large_pieces[r] = LoadTexture("assets/images/RookB.png");
    img_small_pieces[r] = LoadTexture("assets/images/RookB.png");
    img_large_pieces[q] = LoadTexture("assets/images/QueenB.png");
    img_small_pieces[q] = LoadTexture("assets/images/QueenB.png");
    img_large_pieces[k] = LoadTexture("assets/images/KingB.png");
    img_small_pieces[k] = LoadTexture("assets/images/KingB.png");
    for (int idx = 0; idx < 12; idx++)
    {
        img_large_pieces[idx].width = large_piece_size;
        img_large_pieces[idx].height = large_piece_size;
        img_small_pieces[idx].width = small_piece_size;
        img_small_pieces[idx].height = small_piece_size;
    }
    img_players[human] = LoadTexture("assets/images/HUMAN.png");
    img_players[human].width = squaresize;
    img_players[human].height = squaresize;
    img_players[ai] = LoadTexture("assets/images/AI.png");
    img_players[ai].width = squaresize;
    img_players[ai].height = squaresize;
    img_enter_btn = LoadTexture("assets/images/Enter.png");
    img_enter_btn.height = squaresize;
    img_enter_btn.width = squaresize * 2;
    img_plus_min_btn = LoadTexture("assets/images/Plusmin.png");
    img_plus_min_btn.height = squaresize;
    img_plus_min_btn.width = squaresize * 2;
    img_choice_btn = LoadTexture("assets/images/Choice.png");
    img_choice_btn.height = squaresize;
    img_choice_btn.width = squaresize;
    img_chess_clock = LoadTexture("assets/images/clock.png");
    img_chess_clock.width = squaresize * 2;
    img_chess_clock.height = squaresize;
}

/// @brief unload images
/// @param img pointer to the imagedata
void unload_images()
{
    for (int idx = 0; idx < 12; idx++)
    {
        UnloadTexture(img_large_pieces[idx]);
        UnloadTexture(img_small_pieces[idx]);
        if (idx < 2)
            UnloadTexture(img_players[idx]);
    }
    UnloadTexture(img_table);
    UnloadTexture(img_chess_board);
    UnloadTexture(img_enter_btn);
    UnloadTexture(img_plus_min_btn);
    UnloadTexture(img_choice_btn);
    UnloadTexture(img_chess_clock);
}

const char *rules[26] = {
    "   REGELS DOORGEEF SCHAAK:",
    "   ------------------------------",
    "1. Een speler mag een doorgegeven stuk op het bord zetten in plaats van een gewone zet (het verplaatsen van een stuk).",
    "2. Met het plaatsen mag niet worden geslagen en geen schaak worden gegeven.",
    "3. Een pion mag niet op een promotieveld of op de eigen onderste rij worden geplaatst.",
    "4. Een stuk dat is ontstaan door de promotie van een pion wordt bij het doorgeven weer een pion.",
    "5. De doorgegeven schaakstukken moeten zichtbaar zijn voor de tegenstanders.",
    "6. Pat of remise wegens materiaaltekort is pas definitief als de andere partij afgelopen is.",
    "7. Bij tijdsoverschrijding is de partij remise als de tegenstander de partij niet reglementair kan winnen, ook niet door het",
    "   inzetten van stukken die diens teamgenoot geslagen heeft of nog kan slaan.",
    "8. De wedstrijd eindigt als beide partijen zijn afgelopen, de uitslag is, net als bij andere teamwedstrijden, de optelsom",
    "   van de afzonderlijke partijen, dus 2-0, 1,5-0,5 of 1-1.",
    "",
    "   WERKING PROGRAMA:",
    "   ----------------------",
    "1. Je speelt van onder naar boven dus met wit op het rechter of met zwart op het linker bord.",
    "2. De a.i zal voor de overige spelers de zetten doen.",
    "3. Geslagen en doorgegeven stukken of pionnen zijn zichtbaar boven of onder het bord.",
    "4. Zodra je aan zet bent kun je op een stuk of pion op het bord of eventueel op een doorgegeven stuk of pion klikken met de",
    "   muis, het programma zal dan de valide velden tonen als deze er zijn, zodra je op een valide veld klikt wordt deze zet",
    "   uitgevoerd en gaat de a.i zijn volgende zet berekenen.",
    "5. Hieronder worden de standaard instellingen getoond deze kunnen gewijzigd worden door op de buttons te klikken.",
    "6. Door op de enter button/key te klikken wordt het spel begonnen, de borden worden getoond en de klokken gestart.",
    "",
    "   STANDAARD INSTELLINGEN:",
    "   ------------------------------",
};
const char *colorstr[2] = {"Speler kleur : Wit", "Speler kleur : Zwart"};
const int colorint[2] = {white, black};
const char *timestr[7] = {
    "Speeltijd : 5 min",
    "Speeltijd : 10 min",
    "Speeltijd : 15 min",
    "Speeltijd : 30 min",
    "Speeltijd : 45 min",
    "Speeltijd : 60 min",
    "Speeltijd : 100 min",
};
const int timeint[7] = {300, 600, 900, 1800, 2700, 3600, 6000}; // seconds
const char *plusstr[7] = {
    "Plus tijd per zet : 0 sec",
    "Plus tijd per zet : 2 sec",
    "Plus tijd per zet : 3 sec",
    "Plus tijd per zet : 5 sec",
    "Plus tijd per zet : 10 sec",
    "Plus tijd per zet : 15 sec",
    "Plus tijd per zet : 30 sec",
};
const int plusint[7] = {0, 2, 3, 5, 10, 15, 30}; // seconds
const int pcolor[12] = {white, white, white, white, white, white, black, black, black, black, black, black};
const int promotion_pieces[2][4] = {
    {Q, R, B, N},
    {q, r, b, n},
};

// game controls
// 0 = white, 1 = black
int choice_color;

// 0..7
int choice_time;

// timeint[choice_time]
int game_time;

// 0 .. 7
int choice_plus;

// plusint[choice_plus]
int game_plus_time;

// 0, F9, F10, F11
int fkey;

// input, running, gameover
int game_state;

/********************************************************************\
 * Gui board
\********************************************************************/

/// @brief gui_board data, left or right
typedef struct
{
    /* data */
    int reverse; // 1 = left or 0 = right

    int brd_col; // start column

    int brd_row; // start row

    int captured_piece; // captured piece

    int players[2]; // human or ai per color

    int clocktime[2][5]; // for drawing per color, timeleft in h::mm::ss

    int timer[2]; // remaining time per color

    int stoptimer; // during calculation of the possible moves the timer will be stopped

    int thinktime[2]; // current thinktime per move per color

    int starttime[2]; // starting time per move per color

    int in_check[2]; // 1 = true 0 = false

    int gameover; // 1 = true, 0 = false

    int gameoverstate; // enum gameover state

    int turnstep; // input_piece, input_square or input_cap_piece

    U64 selection_option; // options from the selected piece 

    int selection_cap_piece; // index of the piece in the captured list

    int selection_piece; // square from the selected piece => square from

    int selection_square; // square to move to

    int promotion; // promotion flag

    int promoted_piece; // selected promotion piece

    moves movelist; // calculated moves

} guiboard_t;

guiboard_t *left_gui_board;
guiboard_t *right_gui_board;

/********************************************************************\
 * Board Controls
\********************************************************************/
/// @brief initialize guiboard
/// @param guibrd pointer to guibrd
void init_gui_board(guiboard_t *guibrd)
{
    for (int idx = 0; idx < 5; idx++)
    {
        guibrd->clocktime[white][idx] = 0;
        guibrd->clocktime[black][idx] = 0;
    }
    guibrd->captured_piece = -1;
    guibrd->gameover = 0;
    guibrd->gameoverstate = go_none;
    guibrd->in_check[white] = 0;
    guibrd->in_check[black] = 0;
    guibrd->promoted_piece = -1;
    guibrd->promotion = 0;
    guibrd->selection_option = 0ULL;
    guibrd->selection_cap_piece = -1;
    guibrd->selection_piece = no_sq;
    guibrd->selection_square = no_sq;
    guibrd->turnstep = input_piece;
    guibrd->stoptimer = 1;
    guibrd->starttime[white] = 0;
    guibrd->starttime[black] = 0;
    guibrd->movelist.count = 0;
    guibrd->thinktime[white] = 0;
    guibrd->thinktime[black] = 0;
    guibrd->timer[white] = 0;
    guibrd->timer[black] = 0;
}

/// @brief draw the guiboard
/// @param guibrd pointer to guiboard_t
/// @param brd pointer to board_t   
/// @param gameover_ob indication if the other board has the game over flag set
void draw_gui_board(guiboard_t *guibrd, board_t *brd, int gameover_ob)
{
    int sqr, piece, isoption, row, col;
    const char *X_CO[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    const char *Y_CO[8] = {"8", "7", "6", "5", "4", "3", "2", "1"};
    const char *NUMBER[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const char *MX = ":";
    const char *GAMEOVERTXT[10] = {
        "Onbekend",
        "Wit wc.int, zwart staat mat",
        "Wit wc.int, zwart heeft geen bedenktijd meer",
        "Zwart wc.int, wit staat mat",
        "Zwart wc.int, wit heeft geen bedenktijd meer",
        "Remise door onvoldoende mat materiaal",
        "Remise de witte koning staat pat",
        "Remise de zwart koning staat pat",
        "Remise door herhaling van zetten",
        "Remise door de 50 zetten regel",
    };
    const Color sel = {201, 255, 229, 96};
    const Color clr = {201, 255, 229, 64};
    const Color red = {241, 156, 187, 128};
    const Color w_c = {255, 255, 255, 128};
    const Color goc = {255, 255, 255, 96};

    // draw board
    DrawRectangle(guibrd->brd_col, guibrd->brd_row, boardsize + 2, boardsize + 2, YELLOW);
    DrawTexture(img_chess_board, guibrd->brd_col + 1, guibrd->brd_row + 1, RAYWHITE);
    // draw coordinates
    for (int idx = 0; idx < 8; idx++)
    {
        if (guibrd->reverse)
        {
            DrawText(X_CO[7 - idx], guibrd->brd_col + idx * squaresize + halfsquaresize, guibrd->brd_row - halfsquaresize, 20, RAYWHITE);
            DrawText(Y_CO[7 - idx], guibrd->brd_col - halfsquaresize, guibrd->brd_row + idx * squaresize + halfsquaresize, 20, RAYWHITE);
        }
        else
        {
            DrawText(X_CO[idx], guibrd->brd_col + idx * squaresize + halfsquaresize, guibrd->brd_row - halfsquaresize, 20, RAYWHITE);
            DrawText(Y_CO[idx], guibrd->brd_col - halfsquaresize, guibrd->brd_row + idx * squaresize + halfsquaresize, 20, RAYWHITE);
        }
    }
    // draw pieces and possible options
    for (int idy = 0; idy < 8; idy++)
    {
        for (int idx = 0; idx < 8; idx++)
        {
            sqr = idy * 8 + idx;
            piece = -1;
            for (int pbb = 0; pbb < 12; pbb++)
            {
                if (get_bit(brd->bitboards[pbb], sqr))
                {
                    piece = pbb;
                    break;
                }
            }
            isoption = (get_bit(guibrd->selection_option, sqr)) ? 1 : 0;
            if (guibrd->reverse)
            {
                row = guibrd->brd_row + (7 - idy) * squaresize;
                col = guibrd->brd_col + (7 - idx) * squaresize;
            }
            else
            {
                row = guibrd->brd_row + idy * squaresize;
                col = guibrd->brd_col + idx * squaresize;
            }
            if (guibrd->selection_piece == sqr)
                DrawRectangle(col + 3, row + 3, squaresize - 4, squaresize - 4, sel);
            if (isoption)
                DrawRectangle(col + 3, row + 3, squaresize - 4, squaresize - 4, clr);
            if (piece == K || piece == k)
            {
                if (guibrd->in_check[pcolor[piece]])
                    DrawRectangle(col + 3, row + 3, squaresize - 4, squaresize - 4, red);
            }
            if (piece != -1)
                DrawTexture(img_large_pieces[piece], col + 5, row + 5, RAYWHITE);
        }
    }
    // draw captured pieces
    // // test
    // if (brd->cap_pieces_count[white] == 0)
    //      brd->cap_pieces[white][brd->cap_pieces_count[white]++] = P;
    for (int idx = 0; idx < brd->cap_pieces_count[white]; idx++)
    {
        if (guibrd->reverse)
        {
            row = guibrd->brd_row - squaresize;
            col = guibrd->brd_col + idx * halfsquaresize;
        }
        else
        {
            row = guibrd->brd_row + boardsize + halfsquaresize;
            col = guibrd->brd_col + idx * halfsquaresize;
        }
        DrawTexture(img_small_pieces[brd->cap_pieces[white][idx]], col + 4, row + 4, RAYWHITE);
    }
    // // test
    // if (brd->cap_pieces_count[black] == 0)
    //      brd->cap_pieces[black][brd->cap_pieces_count[black]++] = b;
    for (int idx = 0; idx < brd->cap_pieces_count[black]; idx++)
    {
        if (guibrd->reverse)
        {
            row = guibrd->brd_row + boardsize + halfsquaresize;
            col = guibrd->brd_col + idx * halfsquaresize;
        }
        else
        {
            row = guibrd->brd_row - squaresize;
            col = guibrd->brd_col + idx * halfsquaresize;
        }
        DrawTexture(img_small_pieces[brd->cap_pieces[black][idx]], col + 4, row + 4, RAYWHITE);
    }
    // draw players
    if (guibrd->reverse)
    {
        DrawTexture(img_players[guibrd->players[white]], guibrd->brd_col + boardsize - squaresize, 0, w_c);
        DrawTexture(img_players[guibrd->players[black]], guibrd->brd_col + boardsize - squaresize, guibrd->brd_row + boardsize + squaresize, w_c);
    }
    else
    {
        DrawTexture(img_players[guibrd->players[black]], guibrd->brd_col + boardsize - squaresize, 0, w_c);
        DrawTexture(img_players[guibrd->players[white]], guibrd->brd_col + boardsize - squaresize, guibrd->brd_row + boardsize + squaresize, w_c);
    }
    // draw clocks
    // // test
    // brd->side = black;
    col = guibrd->brd_col + boardsize - squaresize * 3;
    if (brd->side == white)
        row = (guibrd->reverse) ? 0 : guibrd->brd_row + boardsize + squaresize;
    else
        row = (guibrd->reverse) ? guibrd->brd_row + boardsize + squaresize : 0;
    DrawTexture(img_chess_clock, col, row, BROWN);
    DrawRectangle(col + 16, row + 30, 115, 20, BROWN);
    Color clock_color = (brd->side == white) ? WHITE : BLACK;
    DrawText(NUMBER[guibrd->clocktime[brd->side][0]], col + 18, row + 30, 20, clock_color);
    DrawText(MX, col + 38, row + 30, 20, clock_color);
    DrawText(NUMBER[guibrd->clocktime[brd->side][1]], col + 48, row + 30, 20, clock_color);
    DrawText(NUMBER[guibrd->clocktime[brd->side][2]], col + 68, row + 30, 20, clock_color);
    DrawText(MX, col + 88, row + 30, 20, clock_color);
    DrawText(NUMBER[guibrd->clocktime[brd->side][3]], col + 98, row + 30, 20, clock_color);
    DrawText(NUMBER[guibrd->clocktime[brd->side][4]], col + 118, row + 30, 20, clock_color);
    // draw game over
    // // test
    // guibrd->gameover = 1;
    if (guibrd->gameover)
    {
        DrawRectangle(guibrd->brd_col + 1, guibrd->brd_row + 1, boardsize - 1, boardsize - 1, goc);
        DrawRectangle(guibrd->brd_col + 1, guibrd->brd_row + boardsize + 1, boardsize - 2, squaresize * 4 - 2, goc);
        DrawText(GAMEOVERTXT[guibrd->gameoverstate], guibrd->brd_col + halfsquaresize, guibrd->brd_row + boardsize + halfsquaresize, 20, YELLOW);
        if (gameover_ob)
            DrawText("druk op enter...", guibrd->brd_col + halfsquaresize, guibrd->brd_row + boardsize + halfsquaresize + squaresize, 20, YELLOW);
    }
    // draw promotion
    // // test
    // guibrd->promotion = 1;
    // // test
    // brd->side = black;
    if (guibrd->promotion)
    {
        DrawRectangle(guibrd->brd_col + 1, guibrd->brd_row + 1, boardsize - 1, boardsize - 1, goc);
        DrawRectangle(guibrd->brd_col + squaresize * 2, guibrd->brd_row + squaresize * 3, squaresize * 4, squaresize * 2, DARKGRAY);
        DrawText("Kies een stuk:", guibrd->brd_col + squaresize * 2 + halfsquaresize, guibrd->brd_row + squaresize * 3 + halfsquaresize, 20, YELLOW);
        for (int idx = 0; idx < 4; idx++)
        {
            int pidx = promotion_pieces[brd->side][idx];
            DrawTexture(img_large_pieces[pidx], guibrd->brd_col + squaresize * (2 + idx) + 4, guibrd->brd_row + squaresize * 4 + 4, LIGHTGRAY);
        }
    }
}

/// @brief update the timer
/// @param guibrd pointer to guiboard_t
/// @param brd pointer to board_t
void update_gui_board(guiboard_t *guibrd, board_t *brd)
{
    if (!guibrd->gameover && !guibrd->stoptimer)
    {
        int duration = get_time_ms() - guibrd->starttime[brd->side];
        guibrd->thinktime[brd->side] += duration;
        guibrd->starttime[brd->side] = get_time_ms();
        if (guibrd->thinktime[brd->side] > 1000)
        {
            if (guibrd->timer[brd->side] > 0)
            {
                guibrd->timer[brd->side] -= 1;
            }
            guibrd->thinktime[brd->side] -= 1000;
        }
    }
    int hr = (int)(guibrd->timer[brd->side] / 3600);
    int rst = guibrd->timer[brd->side] - hr * 3600;
    int sec = rst % 60;
    int min = (int)((rst - sec) / 60);
    guibrd->clocktime[brd->side][4] = sec % 10;
    guibrd->clocktime[brd->side][3] = (int)((sec - guibrd->clocktime[brd->side][4]) / 10);
    guibrd->clocktime[brd->side][2] = min % 10;
    guibrd->clocktime[brd->side][1] = (int)((min - guibrd->clocktime[brd->side][2]) / 10);
    guibrd->clocktime[brd->side][0] = hr;
    if (guibrd->timer[brd->side] == 0)
    {
        guibrd->gameover = 1;
        guibrd->gameoverstate = brd->side == white ? go_btime : go_wtime;
    }
}

/// @brief mouse handler
/// @param guibrd pointer to guiboard_t
/// @param brd pointer to board_t
/// @param keep pointer to board_t
/// @param x mouse column
/// @param y mouse row
void mouse_handler_gui_board(guiboard_t *guibrd, board_t *brd, board_t *keep, int x, int y)
{
    int col = (int)((x - guibrd->brd_col) / squaresize);
    int row = y >= guibrd->brd_row ? (int)((y - guibrd->brd_row) / squaresize) : y >= guibrd->brd_row - squaresize ? -1
                                                                                                                   : -2;
    // promotion ?
    if (guibrd->promotion)
    {
        if (col >= 2 && col <= 5 && row == 4)
        {
            guibrd->promoted_piece = promotion_pieces[brd->side][col - 2];
            int srcpiece = -1;
            for (int pbb = 0; pbb < 12; pbb++)
            {
                if (get_bit(brd->bitboards[pbb], guibrd->selection_piece))
                {
                    srcpiece = pbb;
                    break;
                }
            }
            int move = -1; // not found
            for (int idx = 0; idx < guibrd->movelist.count; idx++)
            {
                move = guibrd->movelist.moves[idx];
                int source_square = get_move_source(move);
                int piece = get_move_piece(move);
                int target_square = get_move_target(move);
                int promoted_piece = get_move_promoted(move);
                if (piece == srcpiece && guibrd->selection_piece == source_square && guibrd->selection_square == target_square && promoted_piece == guibrd->promoted_piece)
                    break;
                else
                    move = -1;
            }
            if (move >= 0)
            {
                guibrd->promotion = 0;
                if (game_plus_time > 0)
                    guibrd->timer[brd->side] += game_plus_time;
                update_gui_board(guibrd, brd);
                guibrd->stoptimer = 1;
                make_move(move, all_moves, brd, keep);
                generate_moves(&guibrd->movelist, brd);
                moves_to_options(&left_gui_board->movelist, brd, keep);
                guibrd->turnstep = input_piece;
                guibrd->starttime[brd->side] = (int)(GetTime() * 1000.0);
                guibrd->stoptimer = 0;
            }
        }
    }
    else if (guibrd->turnstep == input_piece)
    {
        if (row >= 0 && row < 8)
        {
            int sqr = (guibrd->reverse) ? (7 - row) * 8 + 7 - col : row * 8 + col;
            int piece = -1;
            for (int pbb = 0; pbb < 12; pbb++)
            {
                if (get_bit(brd->bitboards[pbb], sqr))
                {
                    piece = pbb;
                    break;
                }
            }
            if (piece >= P && pcolor[piece] == brd->side)
            {
                guibrd->selection_piece = sqr;
                guibrd->selection_option = brd->options[sqr];
                guibrd->turnstep = input_square;
            }
        }
    }
    else if (guibrd->turnstep == input_square)
    {
        if (row >= 0 && row < 8)
        {
            int sqr = (guibrd->reverse) ? (7 - row) * 8 + 7 - col : row * 8 + col;
            int piece = -1;
            for (int pbb = 0; pbb < 12; pbb++)
            {
                if (get_bit(brd->bitboards[pbb], sqr))
                {
                    piece = pbb;
                    break;
                }
            }
            if (piece >= P && pcolor[piece] == brd->side)
            {
                // choose another piece
                guibrd->selection_piece = sqr;
                guibrd->selection_option = brd->options[sqr];
            }
            if (piece == -1 || (piece >= P && pcolor[piece] != brd->side))
            {
                // empty sqaure or other piece
                guibrd->selection_square = sqr;
                if (sqr >> 3 == 0 && piece == P)
                    guibrd->promotion = 1;
                if (sqr >> 3 == 7 && piece == p)
                    guibrd->promotion = 1;
                if (!guibrd->promotion)
                {
                    int srcpiece = -1;
                    for (int pbb = 0; pbb < 12; pbb++)
                    {
                        if (get_bit(brd->bitboards[pbb], guibrd->selection_piece))
                        {
                            srcpiece = pbb;
                            break;
                        }
                    }
                    int move = -1; // not found
                    for (int idx = 0; idx < guibrd->movelist.count; idx++)
                    {
                        move = guibrd->movelist.moves[idx];
                        int source_square = get_move_source(move);
                        int piece = get_move_piece(move);
                        int target_square = get_move_target(move);
                        if (piece == srcpiece && guibrd->selection_piece == source_square && guibrd->selection_square == target_square)
                            break;
                        else
                            move = -1;
                    }
                    if (move >= 0)
                    {
                        if (game_plus_time > 0)
                            guibrd->timer[brd->side] += game_plus_time;
                        update_gui_board(guibrd, brd);
                        guibrd->stoptimer = 1;
                        make_move(move, all_moves, brd, keep);
                        generate_moves(&guibrd->movelist, brd);
                        moves_to_options(&left_gui_board->movelist, brd, keep);
                        if (game_plus_time > 0)
                            guibrd->timer[brd->side ^ 1] += game_plus_time;
                        guibrd->turnstep = input_piece;
                        guibrd->starttime[brd->side] = (int)(GetTime() * 1000.0);
                        guibrd->stoptimer = 0;
                    }
                }
            }
        }
    }
}

/********************************************************************\
 * Game Controls
\********************************************************************/
/// @brief copy movelist
/// @param listsrc pointer to source moves
/// @param listdst pointer to distination moves
void copy_moves(moves *listsrc, moves *listdst)
{
    listdst->count = 0;
    for (int idx = 0; idx < listsrc->count; idx++)
        add_move(listdst, listsrc->moves[idx]);
}

/// @brief initialize the gui
void init_gui()
{
    choice_color = 0;
    choice_time = 0;
    game_time = 0;
    choice_plus = 0;
    game_plus_time = 0;
    fkey = none;
    game_state = input;
    left_gui_board->reverse = 1;
    left_gui_board->brd_col = squaresize;
    left_gui_board->brd_row = squaresize * 2;
    right_gui_board->reverse = 0;
    right_gui_board->brd_col = boardsize + squaresize * 3;
    right_gui_board->brd_row = squaresize * 2;
}

/// @brief start a new game
void new_game()
{
    int plrs[4];

    plrs[white] = ai;
    plrs[black] = ai;
    plrs[white + 2] = ai;
    plrs[black + 2] = ai;

    if (fkey > none && fkey < f11)
    {
        if (fkey == f11)
        {
            plrs[black] = human;
            plrs[white + 2] = human;
        }
        if (fkey == f10)
        {
            plrs[white] = human;
            plrs[black] = human;
            plrs[white + 2] = human;
            plrs[black + 2] = human;
        }
    }
    else
    {
        if (choice_color == white)
        {
            plrs[white + 2] = human;
        }
        else
        {
            plrs[black] = human;
        }
    }
    init_all();
    init_gui_board(left_gui_board);
    init_gui_board(right_gui_board);
    left_gui_board->stoptimer = 1;
    right_gui_board->stoptimer = 1;
    parse_fen(start_position, left_board);
    generate_moves(&left_gui_board->movelist, left_board);
    moves_to_options(&left_gui_board->movelist, left_board, keep_left);
    copy_board(left_board, right_board);
    copy_moves(&left_gui_board->movelist, &right_gui_board->movelist);
    left_gui_board->players[white] = plrs[white];
    left_gui_board->players[black] = plrs[black];
    left_gui_board->timer[white] = game_time;
    left_gui_board->timer[black] = game_time;
    right_gui_board->players[white] = plrs[white + 2];
    right_gui_board->players[black] = plrs[black + 2];
    right_gui_board->timer[white] = game_time;
    right_gui_board->timer[black] = game_time;
    left_gui_board->thinktime[white] = 0;
    left_gui_board->thinktime[black] = 0;
    right_gui_board->thinktime[white] = 0;
    right_gui_board->thinktime[black] = 0;
    left_gui_board->starttime[left_board->side] = get_time_ms();
    right_gui_board->starttime[right_board->side] = get_time_ms();
    left_gui_board->stoptimer = 0;
    right_gui_board->stoptimer = 0;
    game_state = running;
}

void update_game()
{
    if (game_state == running)
    {
        if (left_gui_board->captured_piece >= P)
        {
            if (pcolor[left_gui_board->captured_piece] == white)
            {
                right_board->hash_key ^= piece_keys[left_gui_board->captured_piece][63 - right_board->cap_pieces_count[white]];
                right_board->cap_pieces[white][right_board->cap_pieces_count[white]++] = left_gui_board->captured_piece;
                left_gui_board->captured_piece = -1;
                if (right_board->side == white)
                {
                    // right_board->CapPieceOptions()
                }
            }
            else
            {
                right_board->hash_key ^= piece_keys[left_gui_board->captured_piece][right_board->cap_pieces_count[black]];
                right_board->cap_pieces[black][right_board->cap_pieces_count[black]++] = left_gui_board->captured_piece;
                left_gui_board->captured_piece = -1;
                if (right_board->side == black)
                {
                    // right_board->CapPieceOptions()
                }
            }
        }
        if (right_gui_board->captured_piece >= P)
        {
            if (pcolor[right_gui_board->captured_piece] == white)
            {
                left_board->hash_key ^= piece_keys[right_gui_board->captured_piece][63 - left_board->cap_pieces_count[white]];
                left_board->cap_pieces[white][left_board->cap_pieces_count[white]++] = right_gui_board->captured_piece;
                right_gui_board->captured_piece = -1;
                if (left_board->side == white)
                {
                    // left_board->CapPieceOptions()
                }
            }
            else
            {
                left_board->hash_key ^= piece_keys[right_gui_board->captured_piece][left_board->cap_pieces_count[black]];
                left_board->cap_pieces[black][left_board->cap_pieces_count[black]++] = right_gui_board->captured_piece;
                right_gui_board->captured_piece = -1;
                if (left_board->side == black)
                {
                    // left_board->CapPieceOptions()
                }
            }
        }
        update_gui_board(left_gui_board, left_board);
        update_gui_board(right_gui_board, right_board);
        if (left_gui_board->gameover && right_gui_board->gameover)
            game_state = gameover;
    }
}

/// @brief start of the program
/// @return returncode
int main()
{
    left_board = (board_t *)malloc(sizeof(board_t));
    assert(left_board != NULL);
    right_board = (board_t *)malloc(sizeof(board_t));
    assert(right_board != NULL);
    keep_left = (board_t *)malloc(sizeof(board_t));
    assert(keep_left != NULL);
    keep_right = (board_t *)malloc(sizeof(board_t));
    assert(keep_right != NULL);
    left_gui_board = (guiboard_t *)malloc(sizeof(guiboard_t));
    assert(left_gui_board != NULL);
    right_gui_board = (guiboard_t *)malloc(sizeof(guiboard_t));
    assert(right_gui_board != NULL);

    if (testboard)
    {
        moves movelist;
        init_all();
        parse_fen(start_position, left_board);
        copy_board(left_board, keep_left);
        print_board(left_board);
        generate_moves(&movelist, left_board);
        print_move_list(&movelist);
        moves_to_options(&movelist, left_board, keep_left);
        print_options(left_board);
        free(left_board);
        free(right_board);
        free(keep_left);
        free(keep_right);
        free(left_gui_board);
        free(right_gui_board);
        return 0;
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenwidth, screenheight, "Raylib Chess pass-through game by P.Veenendaal!");

    load_images();
    init_gui();

    SetTargetFPS(10); // Set our game to run at 10 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        update_game();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        if (game_state == input)
        {
            DrawRectangle(0, 0, screenwidth, screenheight, BLUE);
            // regels doorgeefschaak / werking programma
            for (int idx = 0; idx < 26; idx++)
                DrawText(rules[idx], 8, 24 * idx + 24, 20, WHITE);
            // kleurkeuze
            DrawText(colorstr[choice_color], squaresize, squaresize * 9 + halfsquaresize, 20, YELLOW);
            DrawTexture(img_choice_btn, squaresize * 5, squaresize * 9, RAYWHITE);
            // tijdkeuze
            DrawText(timestr[choice_time], squaresize, squaresize * 10 + halfsquaresize, 20, YELLOW);
            DrawTexture(img_plus_min_btn, squaresize * 5, squaresize * 10, RAYWHITE);
            // plustijdkeuze
            DrawText(plusstr[choice_plus], squaresize, squaresize * 11 + halfsquaresize, 20, YELLOW);
            DrawTexture(img_plus_min_btn, squaresize * 5, squaresize * 11, RAYWHITE);
            // enterkeuze
            DrawTexture(img_enter_btn, squaresize * 9, squaresize * 11, RAYWHITE);
        }
        else
        {
            DrawTexture(img_table, 0, 0, RAYWHITE);
            draw_gui_board(left_gui_board, left_board, (right_gui_board->gameover) ? 1 : 0);
            draw_gui_board(right_gui_board, right_board, (left_gui_board->gameover) ? 1 : 0);
            if (game_state == gameover)
                DrawTexture(img_enter_btn, squaresize * 9, squaresize * 11, RAYWHITE);
        }
        DrawFPS(1, 1);
        EndDrawing();
        //----------------------------------------------------------------------------------
        // keypress
        if (IsKeyPressed(KEY_ENTER))
        {
            if (game_state == input)
            {
                game_time = timeint[choice_time];
                game_plus_time = plusint[choice_plus];
                new_game();
            }
            else if (game_state == gameover)
                game_state = input;
        }
        if (IsKeyPressed(KEY_F9))
        {
            fkey = f9;
        }
        if (IsKeyPressed(KEY_F10))
        {
            fkey = f10;
        }
        if (IsKeyPressed(KEY_F11))
        {
            fkey = f11;
        }

        // Mouse Press
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x = GetMouseX();
            int y = GetMouseY();
            if (game_state == input)
            {
                if (x >= squaresize * 5 && x < squaresize * 6 && y >= squaresize * 9 && y < squaresize * 10)
                    choice_color ^= 1;
                if (x >= squaresize * 5 && x < squaresize * 6 && y >= squaresize * 10 && y < squaresize * 11)
                {
                    if (choice_time < 6)
                        choice_time += 1;
                }
                if (x >= squaresize * 6 && x < squaresize * 7 && y >= squaresize * 10 && y < squaresize * 11)
                {
                    if (choice_time > 0)
                        choice_time -= 1;
                }
                if (x >= squaresize * 5 && x < squaresize * 6 && y >= squaresize * 11 && y < squaresize * 12)
                {
                    if (choice_plus < 6)
                        choice_plus += 1;
                }
                if (x >= squaresize * 6 && x < squaresize * 7 && y >= squaresize * 11 && y < squaresize * 12)
                {
                    if (choice_plus > 0)
                        choice_plus -= 1;
                }
                if (x >= squaresize * 9 && x < squaresize * 11 && y >= squaresize * 11 && y < squaresize * 13)
                {
                    game_time = timeint[choice_time];
                    game_plus_time = plusint[choice_plus];
                    new_game();
                }
            }
            else
            {
                // enter after the game over
                if (game_state == gameover)
                {
                    if (x >= squaresize * 9 && x < squaresize * 11 && y >= squaresize * 11 && y < squaresize * 13)
                        game_state = input;
                }
                else
                {
                    if (x >= squaresize && x <= squaresize * 9)
                    {
                        mouse_handler_gui_board(left_gui_board, left_board, keep_left, x, y);
                    }
                    if (x >= squaresize * 11 && x <= squaresize * 19)
                    {
                        mouse_handler_gui_board(right_gui_board, right_board, keep_right, x, y);
                    }
                }
            }
        }

        // Ai Move
        // game_ai_move(&game)
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(left_board);
    free(right_board);
    free(keep_left);
    free(keep_right);
    free(left_gui_board);
    free(right_gui_board);
    CloseWindow(); // Close window and OpenGL context
                   //--------------------------------------------------------------------------------------

    return 0;
}