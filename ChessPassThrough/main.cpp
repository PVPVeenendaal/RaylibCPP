#include <iostream>
#include <raylib.h>
#include "defs.h"
#include "hashcode.h"
#include "tables.h"

#include "game.h"
#include "board.h"
#include "images.h"

// ------------------------------------------------------------------------------------------------
// Tables
// ------------------------------------------------------------------------------------------------

Game *game;

/// @brief Constructor 
Game::Game()
{
    data = new Game_Data();
    data->game_time = 0;
    data->game_plus_time = 0;
    data->fkey = 0;
    data->game_state = INPUT;
    data->choice_color = 0;
    data->choice_plus = 0;
    data->choice_time = 0;
}

/// @brief Destructor
Game::~Game()
{
    delete data;
}

/// @brief Update the gui
void Game::Update()
{
    if (data->game_state == RUNNING)
    {
        if (left_board->Get()->captured_piece.piecetype != NP)
        {
            if (PCOLOR(left_board->Get()->captured_piece.piecetype) == PWHITE)
            {
                if (left_board->Get()->captured_piece.promoted)
                    left_board->Get()->captured_piece.piecetype = WP;
                right_board->Get()->cap_pieces[PWHITE][right_board->Get()->cnt_cap_pieces[PWHITE]++] = left_board->Get()->captured_piece;
                if (right_board->Get()->side2move == PWHITE)
                {
                    right_board->FillBoard64();
                    right_board->CapPieceOptions();
                }
            }
            else
            {
                if (left_board->Get()->captured_piece.promoted)
                    left_board->Get()->captured_piece.piecetype = BP;
                right_board->Get()->cap_pieces[PBLACK][right_board->Get()->cnt_cap_pieces[PBLACK]++] = left_board->Get()->captured_piece;
                if (right_board->Get()->side2move == PBLACK)
                {
                    right_board->FillBoard64();
                    right_board->CapPieceOptions();
                }
            }
            left_board->Get()->captured_piece = empty_piece;
        }
        if (right_board->Get()->captured_piece.piecetype != NP)
        {
            if (PCOLOR(right_board->Get()->captured_piece.piecetype) == PWHITE)
            {
                if (right_board->Get()->captured_piece.promoted)
                    right_board->Get()->captured_piece.piecetype = WP;
                left_board->Get()->cap_pieces[PWHITE][left_board->Get()->cnt_cap_pieces[PWHITE]++] = right_board->Get()->captured_piece;
                if (left_board->Get()->side2move == PWHITE)
                {
                    left_board->FillBoard64();
                    left_board->CapPieceOptions();
                }
            }
            else
            {
                if (right_board->Get()->captured_piece.promoted)
                    right_board->Get()->captured_piece.piecetype = BP;
                left_board->Get()->cap_pieces[PBLACK][left_board->Get()->cnt_cap_pieces[PBLACK]++] = right_board->Get()->captured_piece;
                if (left_board->Get()->side2move == PBLACK)
                {
                    left_board->FillBoard64();
                    left_board->CapPieceOptions();
                }    
            }
            right_board->Get()->captured_piece = empty_piece;
        }
        left_board->Update();
        right_board->Update();
        if (left_board->Get()->game_state == GAMEOVER && right_board->Get()->game_state == GAMEOVER)
            data->game_state = GAMEOVER;
    }
}

/// @brief Setup game
void Game::Setup()
{
    int players[4];
    players[PWHITE] = AI;
    players[PBLACK] = AI;
    players[PWHITE + 2] = AI;
    players[PBLACK + 2] = AI;
    if (data->fkey > 0 && data->fkey < 12)
    {
        if (data->fkey == 11)
        {
            players[PBLACK] = HUMAN;
            players[PWHITE + 2] = HUMAN;
        }
        else if (data->fkey == 10)
        {
            players[PWHITE] = HUMAN;
            players[PBLACK] = HUMAN;
            players[PWHITE + 2] = HUMAN;
            players[PBLACK + 2] = HUMAN;
        }
    }
    else
    {
        if (data->choice_color == PWHITE)
            players[PWHITE + 2] = HUMAN;
        else
            players[PBLACK] = HUMAN;
    }
#ifdef FOUR_PLAYER
    players[PWHITE] = HUMAN;
    players[PBLACK] = HUMAN;
    players[PWHITE + 2] = HUMAN;
    players[PBLACK + 2] = HUMAN;
#endif
    left_board->Setup(players[PWHITE], players[PBLACK], data->game_time, data->game_plus_time);
    right_board->Setup(players[PWHITE + 2], players[PBLACK + 2], data->game_time, data->game_plus_time);
    data->game_state = RUNNING;
}

/// @brief Constructor
void Game::Draw()
{
    if (data->game_state == INPUT)
    {
        DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, BLUE);
        // regels doorgeefschaak / werking programma
        for (int i = 0; i < 26; i++)
        {
            DrawText(RULES[i].c_str(), 8, 24 * i + 24, 20, WHITE);
        }
        // kleurkeuze
        DrawText(
            COLORSTR[data->choice_color].c_str(),
            SQUARESIZE,
            SQUARESIZE * 9 + HALFSQUARESIZE,
            20,
            YELLOW);
        DrawTexture(images->Get(IMG_CHOICE_BTN, 0), SQUARESIZE * 5, SQUARESIZE * 9, RAYWHITE);
        // tijdkeuze
        DrawText(
            TIMESTR[data->choice_time].c_str(),
            SQUARESIZE,
            SQUARESIZE * 10 + HALFSQUARESIZE,
            20,
            YELLOW);
        DrawTexture(images->Get(IMG_PLUS_MIN_BTN, 0), SQUARESIZE * 5, SQUARESIZE * 10, RAYWHITE),
            // plustijdkeuze
            DrawText(
                PLUSSTR[data->choice_plus].c_str(),
                SQUARESIZE,
                SQUARESIZE * 11 + HALFSQUARESIZE,
                20,
                YELLOW);
        DrawTexture(images->Get(IMG_PLUS_MIN_BTN, 0), SQUARESIZE * 5, SQUARESIZE * 11, RAYWHITE);
        // enterkeuze
        DrawTexture(images->Get(IMG_ENTER_BTN, 0), SQUARESIZE * 9, SQUARESIZE * 11, RAYWHITE);
    }
    else
    {
        DrawTexture(images->Get(IMG_TABLE, 0), 0, 0, RAYWHITE);
        left_board->Draw(right_board->Get()->game_state == GAMEOVER);
        right_board->Draw(left_board->Get()->game_state == GAMEOVER);
        if (data->game_state == GAMEOVER)
            DrawTexture(images->Get(IMG_ENTER_BTN, 0), SQUARESIZE * 9, SQUARESIZE * 11, RAYWHITE);
    }
}

/// @brief Enter handler
void Game::Enter_handler()
{
    if (data->game_state == INPUT)
    {
        data->game_time = TIMEINT[data->choice_time];
        data->game_plus_time = PLUSINT[data->choice_plus];
        data->game_state = RUNNING;
        Setup();
    }
    else
    {
        // enter after the game over
        if (data->game_state == GAMEOVER)
        {
            data->game_state = INPUT;
        }
    }
}

/// @brief Key handler
/// @param key 
void Game::Key_handler(int key)
{
    if (data->game_state == INPUT)
    {
        switch (key)
        {
        case 298:
            data->fkey = 12;
            break;
        case 299:
            data->fkey = 10;
            break;
        case 300:
            data->fkey = 11;
            break;
        }
    }
}

/// @brief Mouse handler
/// @param x column
/// @param y row
void Game::Mouse_handler(int x, int y)
{
    if (data->game_state == INPUT)
    {
        if (x >= SQUARESIZE * 5 &&
            x < SQUARESIZE * 6 &&
            y >= SQUARESIZE * 9 &&
            y < SQUARESIZE * 10)
        {
            data->choice_color ^= 1;
        }
        else if (x >= SQUARESIZE * 5 &&
                 x < SQUARESIZE * 6 &&
                 y >= SQUARESIZE * 10 &&
                 y < SQUARESIZE * 11)
        {
            if (data->choice_time < 6)
            {
                data->choice_time++;
            }
        }
        else if (x >= SQUARESIZE * 6 &&
                 x < SQUARESIZE * 7 &&
                 y >= SQUARESIZE * 10 &&
                 y < SQUARESIZE * 11)
        {
            if (data->choice_time > 0)
            {
                data->choice_time--;
            }
        }
        else if (x >= SQUARESIZE * 5 &&
                 x < SQUARESIZE * 6 &&
                 y >= SQUARESIZE * 11 &&
                 y < SQUARESIZE * 12)
        {
            if (data->choice_plus < 6)
            {
                data->choice_plus++;
            }
        }
        else if (x >= SQUARESIZE * 6 &&
                 x < SQUARESIZE * 7 &&
                 y >= SQUARESIZE * 11 &&
                 y < SQUARESIZE * 12)
        {
            if (data->choice_plus > 0)
            {
                data->choice_plus--;
            }
        }
        else if (x >= SQUARESIZE * 9 &&
                 x < SQUARESIZE * 11 &&
                 y >= SQUARESIZE * 11 &&
                 y < SQUARESIZE * 13)
        {
            data->game_time = TIMEINT[data->choice_time];
            data->game_plus_time = PLUSINT[data->choice_plus];
            data->game_state = RUNNING;
            Setup();
        }
    }
    else
    {
        // enter after the game over
        if (data->game_state == GAMEOVER)
        {
            if (x >= SQUARESIZE * 9 &&
                x < SQUARESIZE * 11 &&
                y >= SQUARESIZE * 11 &&
                y < SQUARESIZE * 13)
                data->game_state = INPUT;
        }
        else
        {
            if (x >= SQUARESIZE && x <= SQUARESIZE * 9)
                left_board->Mouse_handler(x, y);
            if (x >= SQUARESIZE * 11 && x <= SQUARESIZE * 19)
                right_board->Mouse_handler(x, y);
        }
    }
}

/// @brief Do the Ai move
void Game::AiMove()
{
}

// ------------------------------------------------------------------------------------------------
// End Game
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Board
// ------------------------------------------------------------------------------------------------

Board *left_board;
Board *right_board;

/// @brief Calculate pawn options
/// @param sqf square from
/// @param piece wp or bp
/// @return 
U64 Board::PawnOptions(int sqf, int piece)
{
	int king, sqt;
	U64 options = 0ULL;

	if (piece == WP)
	{
		king = data->king_loc[PWHITE];
		// normal move
		sqt = WPAWN_MOVES[sqf][1];
		if (data->test_board[sqt] == NP)
		{
			if (TestMove(data->test_board, king, sqf, sqt))
				SET_BIT(options, sqt);
			if (SQROW(sqf) == WPAWNROW)
			{
				sqt = WPAWN_MOVES[sqt][1];
				if (data->test_board[sqt] == NP && TestMove(data->test_board, king, sqf, sqt))
					SET_BIT(options, sqt);
			}
		}
		// captures
		for (int d = 0; d <= 2; d += 2)
		{
			sqt = WPAWN_MOVES[sqf][d];
			if (PCOLOR(data->test_board[sqt]) == PBLACK && TestMove(data->test_board, king, sqf, sqt))
				SET_BIT(options, sqt);
		}
	}
	else
	{
		king = data->king_loc[PBLACK];
		// normal move
		sqt = BPAWN_MOVES[sqf][1];
		if (data->test_board[sqt] == NP)
		{
			if (TestMove(data->test_board, king, sqf, sqt))
				SET_BIT(options, sqt);
			if (SQROW(sqf) == BPAWNROW)
			{
				sqt = BPAWN_MOVES[sqt][1];
				if (data->test_board[sqt] == NP && TestMove(data->test_board, king, sqf, sqt))
					SET_BIT(options, sqt);
			}
		}
		// captures
		for (int d = 0; d <= 2; d += 2)
		{
			sqt = BPAWN_MOVES[sqf][d];
			if (PCOLOR(data->test_board[sqt]) == PWHITE && TestMove(data->test_board, king, sqf, sqt))
				SET_BIT(options, sqt);
		}
	}

	data->all_options[data->side2move] |= options;
	return options;
}

/// @brief Test move
/// @param piece 
/// @return 64 bit options
U64 Board::PutPieceOptions(int piece)
{
	int row_s, row_e, sqr;
	int pawn = data->side2move == PWHITE ? WP : BP;
	U64 options = 0ULL;

	row_s = piece == pawn ? 1 : 0;
	row_e = piece == pawn ? 6 : 7;

	for (int rank = row_s; rank <= row_e; rank++)
	{
		for (int file = 0; file <= 7; file++)
		{
			sqr = rank * 8 + file;
			if (data->test_board[sqr] != NP)
				continue;
			if (TestPutMove(data->test_board, piece, data->king_loc[data->side2move], data->king_loc[data->side2move ^ 1], sqr))
				SET_BIT(options, sqr);
		}
	}

	data->all_cap_options[data->side2move] |= options;
	return options;
}

/// @brief Calculate knight options
/// @param sqf square form
/// @param piece WN or BN
/// @return 64 bit options
U64 Board::KnightOptions(int sqf, int piece)
{
	int sqt, king, xside;
	U64 options = 0ULL;
	king = data->king_loc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 8; d++)
	{
		sqt = KNIGHT_MOVES[sqf][d];
		if (data->test_board[sqt] == NP || (data->test_board[sqt] != NP && PCOLOR(data->test_board[sqt]) == xside))
			if (TestMove(data->test_board, king, sqf, sqt))
				SET_BIT(options, sqt);
	}

	data->all_options[data->side2move] |= options;
	return options;
}

/// @brief Calculate bishop options
/// @param sqf square from
/// @param piece WB or BB
/// @return 64 bit options
U64 Board::BishopOptions(int sqf, int piece)
{
	int sqt, king, xside;
	U64 options = 0ULL;
	king = data->king_loc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 4; d++)
	{
		for (sqt = sqf;;)
		{
			sqt = BISHOP_MOVES[sqt][d];
			if (data->test_board[sqt] == NP)
				if (TestMove(data->test_board, king, sqf, sqt))
				{
					SET_BIT(options, sqt);
					if (piece != WK && piece != BK)
						continue;
				}
			if (PCOLOR(data->test_board[sqt]) == xside)
				if (TestMove(data->test_board, king, sqf, sqt))
					SET_BIT(options, sqt);
			break;
		}
	}

	data->all_options[data->side2move] |= options;
	return options;
}

/// @brief Calculate rook options
/// @param sqf square from
/// @param piece WR or BR
/// @return 64 bit options
U64 Board::RookOptions(int sqf, int piece)
{
	int sqt, king, xside;
	U64 options = 0ULL;
	king = data->king_loc[PCOLOR(piece)];
	xside = PCOLOR(piece) ^ 1;

	for (int d = 0; d < 4; d++)
	{
		for (sqt = sqf;;)
		{
			sqt = ROOK_MOVES[sqt][d];
			if (data->test_board[sqt] == NP)
				if (TestMove(data->test_board, king, sqf, sqt))
				{
					SET_BIT(options, sqt);
					if (piece != WK && piece != BK)
						continue;
				}
			if (PCOLOR(data->test_board[sqt]) == xside)
				if (TestMove(data->test_board, king, sqf, sqt))
					SET_BIT(options, sqt);
			break;
		}
	}

	data->all_options[data->side2move] |= options;
	return options;
}

/// @brief Calculate castle options
/// @param piece WK or BK
/// @return 64 bit options
U64 Board::CastleOptions(int piece)
{
	U64 options = 0ULL;

	if (piece == WK && !data->loc_pieces[data->king_loc[PBLACK]].incheck)
	{
		if (data->castle_rules & WKS)
		{
			if (data->loc_pieces[F1].piecetype == NP &&
				data->loc_pieces[G1].piecetype == NP &&
				!AttackSquare(data->test_board, E1, PBLACK) &&
				!AttackSquare(data->test_board, G1, PBLACK) &&
				TestMove(data->test_board, E1, E1, G1))
				SET_BIT(options, G1);
		}
		if (data->castle_rules & WQS)
		{
			if (data->loc_pieces[B1].piecetype == NP &&
				data->loc_pieces[C1].piecetype == NP &&
				data->loc_pieces[D1].piecetype == NP &&
				!AttackSquare(data->test_board, D1, PBLACK) &&
				!AttackSquare(data->test_board, C1, PBLACK) &&
				TestMove(data->test_board, E1, E1, C1))
				SET_BIT(options, C1);
		}
	}
	if (piece == BK && !data->loc_pieces[data->king_loc[PBLACK]].incheck)
	{
		if (data->castle_rules & BKS)
		{
			if (data->loc_pieces[F8].piecetype == NP &&
				data->loc_pieces[G8].piecetype == NP &&
				!AttackSquare(data->test_board, F8, PWHITE) &&
				!AttackSquare(data->test_board, G8, PWHITE) &&
				TestMove(data->test_board, E8, E8, G8))
				SET_BIT(options, G8);
		}
		if (data->castle_rules & BQS)
		{
			if (data->loc_pieces[B8].piecetype == NP &&
				data->loc_pieces[C8].piecetype == NP &&
				data->loc_pieces[D8].piecetype == NP &&
				!AttackSquare(data->test_board, D8, PWHITE) &&
				!AttackSquare(data->test_board, C8, PWHITE) &&
				TestMove(data->test_board, E8, E8, C8))
				SET_BIT(options, C8);
		}
	}
	data->all_options[data->side2move] |= options;
	return options;
}

/// @brief Set up the board
void Board::SetupBoard64()
{
	data->loc_pieces.fill(empty_piece);
	for (int i = 0; i < 8; i++)
	{
		data->loc_pieces[TOPROW[i][1]].piecetype = TOPROW[i][0];
		data->loc_pieces[TOPROW[i][1]].location = TOPROW[i][1];
		data->loc_pieces[BOTROW[i][1]].piecetype = BOTROW[i][0];
		data->loc_pieces[BOTROW[i][1]].location = BOTROW[i][1];
		data->loc_pieces[A7 + i].piecetype = BP;
		data->loc_pieces[A7 + i].location = A7 + i;
		data->loc_pieces[A2 + i].piecetype = WP;
		data->loc_pieces[A2 + i].location = A2 + i;
	}
	data->king_loc[PWHITE] = E1;
	data->king_loc[PBLACK] = E8;
	FillBoard64();
}

/// @brief Fill the board
void Board::FillBoard64()
{
	for (int idx = 0; idx < 64; idx++)
	{
		data->test_board[idx] = NP;
		if (idx < 32)
			data->test_cap_pieces[idx] = NP;
	}
	data->test_board[EDGE] = EDGE;
	data->pos_pieces[PWHITE].clear();
	data->pos_pieces[PBLACK].clear();
	for (auto piece : data->loc_pieces)
	{
		if (piece.piecetype == NP)
			continue;
		if (PCOLOR(piece.piecetype) == PWHITE)
			data->pos_pieces[PWHITE].push_back(piece.location);
		else
			data->pos_pieces[PBLACK].push_back(piece.location);
		data->test_board[piece.location] = piece.piecetype;
	}
	int idx = 0;
	for (Piece piece : data->cap_pieces[PWHITE])
	{
		data->test_cap_pieces[idx++] = piece.piecetype;
	}
	for (Piece piece : data->cap_pieces[PBLACK])
	{
		data->test_cap_pieces[idx++] = piece.piecetype;
	}
}

/// @brief Do a move
/// @param iswhite true or false
/// @param sqf square from
/// @param sqt square to
void Board::DoMove(bool iswhite, int sqf, int sqt)
{
#if LOGPRN
	std::cout << "MOVE: " << SQUARENAMES[sqf] << SQUARENAMES[sqt];
#endif
	data->fifty_moves++;
	if (iswhite)
	{
		// remove black piece if enpassant move
		if (sqt == data->enpassant_square)
		{
			int ep = sqt + 8;
			data->captured_piece = data->loc_pieces[ep];
			data->loc_pieces[ep] = empty_piece;
			data->fifty_moves = 0;
		}
		// remove possible captured piece
		else if (data->loc_pieces[sqt].piecetype != NP)
		{
			data->captured_piece = data->loc_pieces[sqt];
			data->loc_pieces[sqt] = empty_piece;
			data->fifty_moves = 0;
		}
		// do move
		data->loc_pieces[sqt] = data->loc_pieces[sqf];
		data->loc_pieces[sqt].location = sqt;
		data->loc_pieces[sqf] = empty_piece;
		if (data->king_loc[PWHITE] == sqf)
		{
			data->king_loc[PWHITE] = sqt;
			// castle move?
			if (ABS(SQCOL(sqf) - SQCOL(sqt)) == 2)
			{
				// castle
				if (sqt > sqf)
				{
					// kingside
					data->loc_pieces[F1] = data->loc_pieces[H1];
					data->loc_pieces[F1].location = F1;
					data->loc_pieces[H1] = empty_piece;
				}
				else
				{
					// queenside
					data->loc_pieces[D1] = data->loc_pieces[A1];
					data->loc_pieces[D1].location = D1;
					data->loc_pieces[A1] = empty_piece;
				}
			}
		}
		if (data->loc_pieces[sqt].piecetype == WP)
		{
			data->fifty_moves = 0;
			// is move a double move pawn ? == set new enpassant_sqaure else clear enpassant_square
			if (ABS(SQROW(sqf) - SQROW(sqt)) == 2)
				data->enpassant_square = sqt + 8;
			else
				data->enpassant_square = NOVALUE;
			// pawn promotion ?
			if (SQROW(sqt) == 0)
			{
				data->promotion = true;
				data->promotion_square_from = sqf;
				data->promotion_square_to = sqt;
				return;
			}
		}
	}
	else // PBLACK
	{
		// remove white piece if enpassant move
		if (sqt == data->enpassant_square)
		{
			int ep = sqt - 8;
			data->captured_piece = data->loc_pieces[ep];
			data->loc_pieces[ep] = empty_piece;
			data->fifty_moves = 0;
		}
		// remove possible captured piece
		else if (data->loc_pieces[sqt].piecetype != NP)
		{
			data->captured_piece = data->loc_pieces[sqt];
			data->loc_pieces[sqt] = empty_piece;
			data->fifty_moves = 0;
		}
		// do move
		data->loc_pieces[sqt] = data->loc_pieces[sqf];
		data->loc_pieces[sqt].location = sqt;
		data->loc_pieces[sqf] = empty_piece;
		if (data->king_loc[PBLACK] == sqf)
		{
			data->king_loc[PBLACK] = sqt;
			// castle move?
			if (ABS(SQCOL(sqf) - SQCOL(sqt)) == 2)
			{
				// castle
				if (sqt > sqf)
				{
					// kingside
					data->loc_pieces[F8] = data->loc_pieces[H8];
					data->loc_pieces[F8].location = F8;
					data->loc_pieces[H8] = empty_piece;
				}
				else
				{
					// queenside
					data->loc_pieces[D8] = data->loc_pieces[A8];
					data->loc_pieces[D8].location = D8;
					data->loc_pieces[A8] = empty_piece;
				}
			}
		}
		if (data->loc_pieces[sqt].piecetype == BP)
		{
			data->fifty_moves = 0;
			// is move a double move pawn ? == set new enpassant_sqaure else clear enpassant_square
			if (ABS(SQROW(sqf) - SQROW(sqt)) == 2)
				data->enpassant_square = sqt - 8;
			else
				data->enpassant_square = NOVALUE;
			// pawn promotion ?
			if (SQROW(sqt) == 7)
			{
				data->promotion = true;
				data->promotion_square_from = sqf;
				data->promotion_square_to = sqt;
				return;
			}
		}
	}
	if (data->plustimer[data->side2move] > 0)
		data->timer[data->side2move] += data->plustimer[data->side2move];
	ChangeSide(sqf, sqt);
}

/// @brief Change side
/// @param sqf square from
/// @param sqt square to
void Board::ChangeSide(int sqf, int sqt)
{
	data->castle_rules &= CASTLE_MASK[sqf] & CASTLE_MASK[sqt];
	data->side2move ^= 1;
	data->starttime[data->side2move] = (int)(GetTime() * 1000);
	FillBoard64();
	PieceOptions();
	CapPieceOptions();
	data->hash_key = haskey_generator->Get(data->test_board, data->test_cap_pieces, data->enpassant_square, data->castle_rules, data->side2move);
	if (data->hply < 63)
		data->hply++;
	else
	{
		for (int i = 0; i < 62; i++)
			data->hash_keys[i] = data->hash_keys[i + 1];
	}
	data->hash_keys[data->hply] = data->hash_key;
	int wking = data->king_loc[PWHITE];
	data->loc_pieces[data->king_loc[PWHITE]].incheck = AttackSquare(data->test_board, wking, PBLACK);
	int bking = data->king_loc[PBLACK];
	data->loc_pieces[data->king_loc[PBLACK]].incheck = AttackSquare(data->test_board, bking, PWHITE);
	TestGameOver();
}

/// @brief Print a bitboard
/// @param bb bitboard
/// @param title title 
void Board::PrintBitboard(U64 bb, std::string title)
{
	std::string b;
	// print offset
	std::cout << std::endl
			  << title << std::endl;

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
				std::cout << "  " << 8 - rank << " ";

			// print bit state (either 1 or 0)
			b = GET_BIT(bb, square) ? "1" : "0";
			std::cout << " " << b;
		}
		// print new line every rank
		std::cout << std::endl;
	}

	// print board files
	std::cout << "\n     a b c d e f g h\n\n";

	// print bitboard as unsigned decimal number
	std::cout << "     Bitboard: " << bb << std::endl
			  << std::endl;
}

/// @brief Is the square attacked?
/// @param brd_piece piece
/// @param sqr square to attack
/// @param attack_color color of the attacker
/// @return true or false
bool Board::AttackSquare(int *brd_piece, int sqr, int attack_color)
{
	int sqf;
	// attack by white_pawn
	for (int d = 0; d <= 2; d += 2)
	{
		sqf = BPAWN_MOVES[sqr][d];
		if (brd_piece[sqf] == WP)
			return true;
	}
	// attack by black_pawn
	for (int d = 0; d <= 2; d += 2)
	{
		sqf = WPAWN_MOVES[sqr][d];
		if (brd_piece[sqf] == BP)
			return true;
	}
	// attack by knight
	for (int d = 0; d < 8; d++)
	{
		sqf = KNIGHT_MOVES[sqr][d];
		if (PCOLOR(brd_piece[sqf]) == attack_color && (brd_piece[sqf] == WN || brd_piece[sqf] == BN))
			return true;
	}
	// attack by bishop, queen or king
	for (int d = 0; d < 4; d++)
	{
		int step = 0;
		for (sqf = sqr;;)
		{
			step++;
			sqf = BISHOP_MOVES[sqf][d];
			if (PCOLOR(brd_piece[sqf]) == attack_color &&
				(brd_piece[sqf] == WB || brd_piece[sqf] == BB || brd_piece[sqf] == WQ || brd_piece[sqf] == BQ || (brd_piece[sqf] == WK && step == 1) || (brd_piece[sqf] == BK && step == 1)))
				return true;
			if (brd_piece[sqf] == NP)
				continue;
			else
				break;
		}
	}
	// attack by rook, queen or king
	for (int d = 0; d < 4; d++)
	{
		int step = 0;
		for (sqf = sqr;;)
		{
			step++;
			sqf = ROOK_MOVES[sqf][d];
			if (PCOLOR(brd_piece[sqf]) == attack_color &&
				(brd_piece[sqf] == WR || brd_piece[sqf] == BR || brd_piece[sqf] == WQ || brd_piece[sqf] == BQ || (brd_piece[sqf] == WK && step == 1) || (brd_piece[sqf] == BK && step == 1)))
				return true;
			if (brd_piece[sqf] == NP)
				continue;
			else
				break;
		}
	}
	return false;
}

/// @brief Test a move
/// @param brd_piece piece
/// @param king king position
/// @param sqf square from
/// @param sqt square to
/// @return 
bool Board::TestMove(int *brd_piece, int king, int sqf, int sqt)
{
	int kopie_piece[65];
	int side = PCOLOR(brd_piece[sqf]);

	for (int i = 0; i < 65; i++)
	{
		kopie_piece[i] = brd_piece[i];
	}

	if (brd_piece[sqf] == WK || brd_piece[sqf] == BK)
	{
		king = sqt;
		if (ABS(SQCOL(sqf) - SQCOL(sqt)) == 2)
		{
			// castle;
			if (sqt > sqf)
			{
				// move rook kingside
				kopie_piece[sqt - 1] = kopie_piece[sqt + 1];
				kopie_piece[sqt + 1] = NP;
			}
			else
			{
				// move rook queenside
				kopie_piece[sqt + 1] = kopie_piece[sqt - 2];
				kopie_piece[sqt - 2] = NP;
			}
		}
	}

	kopie_piece[sqt] = kopie_piece[sqf];
	kopie_piece[sqf] = NP;

	// enpassant
	if ((brd_piece[sqf] == WP || brd_piece[sqf] == BP) &&
		ABS(SQCOL(sqf) - SQCOL(sqt)) == 1 &&
		brd_piece[sqt] == NP)
	{
		if (side == PWHITE)
			kopie_piece[sqt + 8] = NP;
		else
			kopie_piece[sqt - 8] = BP;
	}

	return !AttackSquare(kopie_piece, king, side ^ 1);
}

/// @brief Test a put move
/// @param brd_piece array of the board 
/// @param piece piece to put
/// @param king own king position
/// @param xking other king position
/// @param sqt square to
/// @return true or false
bool Board::TestPutMove(int *brd_piece, int piece, int king, int xking, int sqt)
{
	int kopie_piece[65];
	int side = PCOLOR(piece);
	for (int i = 0; i < 65; i++)
	{
		kopie_piece[i] = brd_piece[i];
	}
	kopie_piece[sqt] = piece;

	return !AttackSquare(kopie_piece, king, side ^ 1) && !AttackSquare(kopie_piece, xking, side);
}

/// @brief Calculate piece options
void Board::PieceOptions()
{
	data->all_options[data->side2move] = 0ULL;
	if (data->side2move == PWHITE)
	{
		for (auto sqr : data->pos_pieces[PWHITE])
		{
			if (data->loc_pieces[sqr].piecetype == WP)
			{
				data->loc_pieces[sqr].options = PawnOptions(sqr, WP);
			}
			else if (data->loc_pieces[sqr].piecetype == WN)
			{
				data->loc_pieces[sqr].options = KnightOptions(sqr, WN);
			}
			else if (data->loc_pieces[sqr].piecetype == WB)
			{
				data->loc_pieces[sqr].options = BishopOptions(sqr, WB);
			}
			else if (data->loc_pieces[sqr].piecetype == WR)
			{
				data->loc_pieces[sqr].options = RookOptions(sqr, WR);
			}
			else if (data->loc_pieces[sqr].piecetype == WQ)
			{
				U64 o1 = BishopOptions(sqr, WQ);
				U64 o2 = RookOptions(sqr, WQ);
				data->loc_pieces[sqr].options = o1 | o2;
			}
			else if (data->loc_pieces[sqr].piecetype == WK)
			{
				U64 o1 = BishopOptions(sqr, WK);
				U64 o2 = RookOptions(sqr, WK);
				U64 o3 = CastleOptions(WK);
				data->loc_pieces[sqr].options = o1 | o2 | o3;
			}
		}
	}
	else
	{
		for (auto sqr : data->pos_pieces[PBLACK])
		{
			if (data->loc_pieces[sqr].piecetype == BP)
			{
				data->loc_pieces[sqr].options = PawnOptions(sqr, BP);
			}
			else if (data->loc_pieces[sqr].piecetype == BN)
			{
				data->loc_pieces[sqr].options = KnightOptions(sqr, BN);
			}
			else if (data->loc_pieces[sqr].piecetype == WB)
			{
				data->loc_pieces[sqr].options = BishopOptions(sqr, BB);
			}
			else if (data->loc_pieces[sqr].piecetype == BR)
			{
				data->loc_pieces[sqr].options = RookOptions(sqr, BR);
			}
			else if (data->loc_pieces[sqr].piecetype == BQ)
			{
				U64 o1 = BishopOptions(sqr, BQ);
				U64 o2 = RookOptions(sqr, BQ);
				data->loc_pieces[sqr].options = o1 | o2;
			}
			else if (data->loc_pieces[sqr].piecetype == BK)
			{
				U64 o1 = BishopOptions(sqr, BK);
				U64 o2 = RookOptions(sqr, BK);
				U64 o3 = CastleOptions(BK);
				data->loc_pieces[sqr].options = o1 | o2 | o3;
			}
		}
	}
#if LOGPRN
	if (data->reverse)
		PrintBitboard(data->all_options[data->side2move], COLORNAMES[data->side2move] + ":All options left board.");
	else
		PrintBitboard(data->all_options[data->side2move], COLORNAMES[data->side2move] + ":All options right board.");
#endif
}

/// @brief Calculete capture options
void Board::CapPieceOptions()
{
	data->all_cap_options[data->side2move] = 0ULL;
	if (data->cnt_cap_pieces[data->side2move] > 0)
	{
		for (int i = 0; i < data->cnt_cap_pieces[data->side2move]; i++)
		{
			data->cap_pieces[data->side2move][i].options = PutPieceOptions(data->cap_pieces[data->side2move][i].piecetype);
		}
	}
#if LOGPRN
	if (data->reverse)
		PrintBitboard(data->all_cap_options[data->side2move], COLORNAMES[data->side2move] + ":All cap options left board.");
	else
		PrintBitboard(data->all_cap_options[data->side2move], COLORNAMES[data->side2move] + ":All cap options right board.");
#endif
}

/// @brief Test if the game is over
void Board::TestGameOver()
{
	// mat?
	if (data->all_options[data->side2move] == 0ULL)
	{
		// mate
		if (data->loc_pieces[data->king_loc[data->side2move]].incheck)
		{
			data->game_state = GAMEOVER;
			data->game_over_state = data->side2move == PWHITE ? BMATE : WMATE;
		}
		else
		// pat -- to do check all-put-options to
		{
			data->game_state = GAMEOVER;
			data->game_over_state = data->side2move == PWHITE ? DRAWPATW : DRAWPATB;
		}
	}
	// time over
	if (data->timer[data->side2move] == 0)
	{
		data->game_state = GAMEOVER;
		data->game_over_state = data->side2move == PWHITE ? BTIME : WTIME;
	}
	// draw by repeating moves
	if (TestRepeatingMove())
	{
		data->game_state = GAMEOVER;
		data->game_over_state = DRAWREP;
	}
	// draw by 50 moves
	if (data->fifty_moves == MAX_FIFTY_MOVES_RULE)
	{
		data->game_state = GAMEOVER;
		data->game_over_state = DRAW50;
	}
	if (TestMaterial())
	{
		data->game_state = GAMEOVER;
		data->game_over_state = DRAWMAT;
	}
}

/// @brief Test if the move is a repating move
/// @return true or false
bool Board::TestRepeatingMove()
{
	int cnt = 0;
	for (int i = 0; i < data->hply; i++)
	{
		if (data->hash_keys[i] == data->hash_key)
			cnt++;
	}
	return cnt >= 2;
}

/// @brief Test if the not sufficient material to finish the game
/// @return true or false
bool Board::TestMaterial()
{
	// to do
	return false;
}

/// @brief Constructor
/// @param rev left or right board
Board::Board(bool rev)
{
	data = new Board_Data();
	data->reverse = rev;
	data->brd_col = (rev) ? SQUARESIZE : BOARDSIZE + SQUARESIZE * 3;
	data->brd_row = SQUARESIZE * 2;
	for (int i = PWHITE; i <= PBLACK; i++)
		for (int j = 0; j < 5; j++)
			data->clocktime[i][j] = 0;
}

/// @brief Destructor
Board::~Board()
{
	delete data;
}

/// @brief Retrieve the board data
/// @return board data
Board_Data *Board::Get()
{
	return data;
}

/// @brief Setup the board
/// @param playerW Player or Ai
/// @param playerB Player or Ai
/// @param time time for the game
/// @param plustime plustime per move
void Board::Setup(int playerW, int playerB, int time, int plustime)
{
	data->player[PWHITE] = playerW;
	data->player[PBLACK] = playerB;
	data->timer[PWHITE] = data->timer[PBLACK] = time;
	data->plustimer[PWHITE] = data->plustimer[PBLACK] = plustime;
	data->side2move = PWHITE;
	data->promotion = false;
	data->promotion_choice = NP;
	data->promotion_square_from = EDGE;
	data->promotion_square_to = EDGE;
	data->game_over_state = NONE;
	data->loc_pieces.fill(empty_piece);
	data->captured_piece = empty_piece;
	data->cap_pieces[PWHITE].fill(empty_piece);
	data->cap_pieces[PBLACK].fill(empty_piece);
	data->captured_piece = empty_piece;
	data->pos_pieces[PWHITE].clear();
	data->pos_pieces[PBLACK].clear();
	data->cnt_cap_pieces[PWHITE] = 0;
	data->cnt_cap_pieces[PBLACK] = 0;
	data->enpassant_square = NOVALUE;
	data->castle_rules = WKS | WQS | BKS | BQS;
	// setup pieces
	SetupBoard64();
	PieceOptions();
	CapPieceOptions();
	data->hply = 0;
	data->hash_key = haskey_generator->Get(data->test_board, data->test_cap_pieces, data->enpassant_square, data->castle_rules, data->side2move);
	data->hash_keys[0] = data->hash_key;
	data->selection = SELECT_PIECE;
	data->selection_piece = EDGE;  // square from the piece
	data->selection_square = EDGE; // square to move to
	data->starttime[data->side2move] = (int)(GetTime() * 1000.0);
	data->game_state = RUNNING; // RUNNING
}

/// @brief Update the board
void Board::Update()
{
	if (data->game_state != GAMEOVER)
	{
		int duration = (int)(GetTime() * 1000.0);
		data->thinktime[data->side2move] += duration - data->starttime[data->side2move];
		data->starttime[data->side2move] = duration;
		if (data->thinktime[data->side2move] > 1000)
		{
			if (data->timer[data->side2move] > 0)
				data->timer[data->side2move] -= 1;
			else
			{
				data->game_state = GAMEOVER;
				data->game_over_state = data->side2move == PWHITE ? BTIME : WTIME;
			}
			data->thinktime[data->side2move] -= 1000;
		}
		for (int i = PWHITE; i <= PBLACK; i++)
		{
			int hr = (int)(data->timer[i] / 3600);
			int rst = data->timer[i] - hr * 3600;
			int sec = rst % 60;
			int min = (int)((rst - sec) / 60);
			data->clocktime[i][4] = sec % 10;
			data->clocktime[i][3] = (int)((sec - data->clocktime[i][4]) / 10);
			data->clocktime[i][2] = min % 10;
			data->clocktime[i][1] = (int)((min - data->clocktime[i][2]) / 10);
			data->clocktime[i][0] = hr;
		}
	}
}

/// @brief Draw the board
/// @param game_over_other_brd other board game over?
void Board::Draw(bool game_over_other_brd)
{
	int clock_col, piece_col;
	int clock_row, piece_row;
	Color w_c = {255, 255, 255, 128};
	Color sel = {201, 255, 229, 96};
	Color col = {201, 255, 229, 64};
	Color red = {241, 156, 187, 128};
	bool isoption;
	int king = data->king_loc[data->side2move];

	// Draw board
	DrawRectangle(data->brd_col, data->brd_row, BOARDSIZE + 2, BOARDSIZE + 2, YELLOW);
	DrawTexture(images->Get(IMG_CHESS_BOARD, 0), data->brd_col + 1, data->brd_row + 1, RAYWHITE);
	// draw coordinates
	for (int i = 0; i < 8; i++)
	{
		if (data->reverse)
		{
			DrawText(X_CO[7 - i].c_str(), data->brd_col + i * SQUARESIZE + HALFSQUARESIZE, data->brd_row - HALFSQUARESIZE, 20, WHITE);
			DrawText(Y_CO[7 - i].c_str(), data->brd_col - HALFSQUARESIZE, data->brd_row + i * SQUARESIZE + HALFSQUARESIZE, 20, WHITE);
		}
		else
		{
			DrawText(X_CO[i].c_str(), data->brd_col + i * SQUARESIZE + HALFSQUARESIZE, data->brd_row - HALFSQUARESIZE, 20, WHITE);
			DrawText(Y_CO[i].c_str(), data->brd_col - HALFSQUARESIZE, data->brd_row + i * SQUARESIZE + HALFSQUARESIZE, 20, WHITE);
		}
	}
	// draw pieces and possible options
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			int sqr = y * 8 + x;
			isoption = GET_BIT(data->selection_options, sqr);
			if (data->reverse)
			{
				piece_row = data->brd_row + (7 - y) * SQUARESIZE;
				piece_col = data->brd_col + (7 - x) * SQUARESIZE;
			}
			else
			{
				piece_row = data->brd_row + y * SQUARESIZE;
				piece_col = data->brd_col + x * SQUARESIZE;
			}
			if (data->selection_piece == sqr)
				DrawRectangle(piece_col + 3, piece_row + 3, SQUARESIZE - 4, SQUARESIZE - 4, sel);
			if (isoption)
				DrawRectangle(piece_col + 3, piece_row + 3, SQUARESIZE - 4, SQUARESIZE - 4, col);
			if (sqr == king && data->loc_pieces[king].incheck)
				DrawRectangle(piece_col + 3, piece_row + 3, SQUARESIZE - 4, SQUARESIZE - 4, red);
			if (data->loc_pieces[sqr].piecetype != NP)
			{
				DrawTexture(images->Get(IMG_LARGE_PIECES, data->loc_pieces[sqr].piecetype), piece_col + 5, piece_row + 5, RAYWHITE);
			}
		}
	}
	// draw captured pieces
	int cnt = -1;
	if (data->cnt_cap_pieces[PWHITE] > 0)
	{
		for (int i = 0; i < data->cnt_cap_pieces[PWHITE]; i++)
		{
			cnt += 1;
			if (data->reverse)
			{
				piece_row = data->brd_row - SQUARESIZE;
				piece_col = data->brd_col + cnt * HALFSQUARESIZE;
			}
			else
			{
				piece_row = data->brd_row + BOARDSIZE + HALFSQUARESIZE;
				piece_col = data->brd_col + cnt * HALFSQUARESIZE;
			}
			DrawTexture(images->Get(IMG_SMALL_PIECES, data->cap_pieces[PWHITE][i].piecetype), piece_col + 2, piece_row + 2, RAYWHITE);
		}
	};
	cnt = -1;
	if (data->cnt_cap_pieces[PBLACK] > 0)
	{
		for (int i = 0; i < data->cnt_cap_pieces[PBLACK]; i++)
		{
			cnt += 1;
			if (data->reverse)
			{
				piece_row = data->brd_row + BOARDSIZE + HALFSQUARESIZE;
				piece_col = data->brd_col + cnt * HALFSQUARESIZE;
			}
			else
			{
				piece_row = data->brd_row - SQUARESIZE;
				piece_col = data->brd_col + cnt * HALFSQUARESIZE;
			}
			DrawTexture(images->Get(IMG_SMALL_PIECES, data->cap_pieces[PBLACK][i].piecetype), piece_col + 2, piece_row + 2, RAYWHITE);
		}
	}
	// draw players
	if (data->reverse)
	{
		DrawTexture(images->Get(IMG_PLAYERS, data->player[PWHITE]), data->brd_col + BOARDSIZE - SQUARESIZE, 0, w_c);
		DrawTexture(images->Get(IMG_PLAYERS, data->player[PBLACK]), data->brd_col + BOARDSIZE - SQUARESIZE, data->brd_row + BOARDSIZE + SQUARESIZE, w_c);
	}
	else
	{
		DrawTexture(images->Get(IMG_PLAYERS, data->player[PBLACK]), data->brd_col + BOARDSIZE - SQUARESIZE, 0, w_c);
		DrawTexture(images->Get(IMG_PLAYERS, data->player[PWHITE]), data->brd_col + BOARDSIZE - SQUARESIZE, data->brd_row + BOARDSIZE + SQUARESIZE, w_c);
	}
	// draw clocks
	clock_col = data->brd_col + BOARDSIZE - SQUARESIZE * 3;
	if (data->side2move == PWHITE)
	{
		if (data->reverse)
			clock_row = 0;
		else
			clock_row = data->brd_row + BOARDSIZE + SQUARESIZE;
	}
	else
	{
		if (data->reverse)
			clock_row = data->brd_row + BOARDSIZE + SQUARESIZE;
		else
			clock_row = 0;
	}
	DrawTexture(images->Get(IMG_CHESS_CLOCK, 0), clock_col, clock_row, BROWN);
	DrawRectangle(clock_col + 16, clock_row + 30, 115, 20, BROWN);
	Color clock_color = data->side2move == PWHITE ? RAYWHITE : BLACK;
	std::string m = ":";
	DrawText(NUMBER[data->clocktime[data->side2move][0]].c_str(), clock_col + 18, clock_row + 30, 20, clock_color);
	DrawText(m.c_str(), clock_col + 38, clock_row + 30, 20, clock_color);
	DrawText(NUMBER[data->clocktime[data->side2move][1]].c_str(), clock_col + 48, clock_row + 30, 20, clock_color);
	DrawText(NUMBER[data->clocktime[data->side2move][2]].c_str(), clock_col + 68, clock_row + 30, 20, clock_color);
	DrawText(m.c_str(), clock_col + 88, clock_row + 30, 20, clock_color);
	DrawText(NUMBER[data->clocktime[data->side2move][3]].c_str(), clock_col + 98, clock_row + 30, 20, clock_color);
	DrawText(NUMBER[data->clocktime[data->side2move][4]].c_str(), clock_col + 118, clock_row + 30, 20, clock_color);
	// draw game over
	if (data->game_state == GAMEOVER)
	{
		Color go_col = {255, 255, 255, 96};
		DrawRectangle(data->brd_col + 1, data->brd_row + 1, BOARDSIZE - 1, BOARDSIZE - 1, go_col);
		DrawRectangle(data->brd_col + 1, data->brd_row + BOARDSIZE + 1, BOARDSIZE - 2, SQUARESIZE * 4 - 2, go_col);
		DrawText(GAMEOVERTXT[data->game_over_state].c_str(), data->brd_col + HALFSQUARESIZE, data->brd_row + BOARDSIZE + HALFSQUARESIZE, 20, YELLOW);
		if (game_over_other_brd)
		{
			DrawText("druk op enter...", data->brd_col + HALFSQUARESIZE, data->brd_row + BOARDSIZE + HALFSQUARESIZE + SQUARESIZE, 20, YELLOW);
		}
	}
	// draw promotion
	if (data->promotion)
	{
		Color prm_col = {255, 255, 255, 96};
		DrawRectangle(data->brd_col + 1, data->brd_row + 1, BOARDSIZE - 1, BOARDSIZE - 1, prm_col);
		DrawRectangle(data->brd_col + SQUARESIZE * 2, data->brd_row + SQUARESIZE * 3, SQUARESIZE * 4, SQUARESIZE * 2, DARKGRAY);
		DrawText("Kies een stuk:", data->brd_col + SQUARESIZE * 2 + HALFSQUARESIZE, data->brd_row + SQUARESIZE * 3 + HALFSQUARESIZE, 20, data->side2move == PWHITE ? WHITE : BLACK);
		for (int i = 0; i < 4; i++)
		{
			int pidx = PROMOTEPIECE[data->side2move][i];
			DrawTexture(images->Get(IMG_LARGE_PIECES, pidx), data->brd_col + SQUARESIZE * (2 + i) + 4, data->brd_row + SQUARESIZE * 4 + 4, LIGHTGRAY);
		}
	}
}

/// @brief Mouse handler board
/// @param x column
/// @param y row
void Board::Mouse_handler(int x, int y)
{
	if (data->game_state == GAMEOVER)
		return;
	int col = (int)((x - data->brd_col) / SQUARESIZE);
	int row = y >= data->brd_row ? (int)((y - data->brd_row) / SQUARESIZE) : y >= data->brd_row - SQUARESIZE ? -1
																											 : -2;
	// promotion ?
	if (data->promotion)
	{
		if (col >= 2 && col <= 5 && row == 4)
		{
			data->promotion_choice = PROMOTEPIECE[data->side2move][col - 2];
			data->loc_pieces[data->promotion_square_to].piecetype = data->promotion_choice;
			data->loc_pieces[data->promotion_square_to].promoted = true;
			if (data->plustimer[data->side2move] > 0)
				data->timer[data->side2move] += data->plustimer[data->side2move];
			ChangeSide(data->promotion_square_from, data->promotion_square_to);
			data->promotion = false;
		}
	}
	else if (data->selection == SELECT_PIECE)
	{
		if (row >= 0 && row < 8)
		{
			int sqr = data->reverse ? (7 - row) * 8 + 7 - col : row * 8 + col;
			if (PCOLOR(data->loc_pieces[sqr].piecetype) == data->side2move)
			{
				data->selection_piece = sqr;
				data->selection_options = data->loc_pieces[sqr].options;
#if LOGPRN
				if (data->reverse)
					PrintBitboard(data->selection_options, COLORNAMES[data->side2move] + ":Selection piece left board.");
				else
					PrintBitboard(data->selection_options, COLORNAMES[data->side2move] + ":Selection piece right board.");
#endif
				data->selection = SELECT_SQUARE;
			}
		}
	}
	else if (data->selection == SELECT_SQUARE)
	{
		if (row >= 0 && row < 8)
		{
			int sqr = data->reverse ? (7 - row) * 8 + 7 - col : row * 8 + col;
			if (PCOLOR(data->loc_pieces[sqr].piecetype) == data->side2move)
			{
				data->selection_piece = sqr;
				data->selection_options = data->loc_pieces[sqr].options;
#if LOGPRN
				if (data->reverse)
					PrintBitboard(data->selection_options, COLORNAMES[data->side2move] + ":Selection piece left board.");
				else
					PrintBitboard(data->selection_options, COLORNAMES[data->side2move] + ":Selection piece right board.");
#endif
			}
			else
			{
				if (GET_BIT(data->selection_options, sqr))
				{
					data->selection_square = sqr;
					data->selection = DO_MOVE;
				}
			}
		}
	}
	if (data->selection == DO_MOVE)
	{
		DoMove(data->side2move == PWHITE, data->selection_piece, data->selection_square);
		data->selection_piece = EDGE;  // square from the piece
		data->selection_square = EDGE; // square to move to
		data->selection = SELECT_PIECE;
		data->selection_options = 0ULL;
	}
}

// ------------------------------------------------------------------------------------------------
// End Board
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Tables
// ------------------------------------------------------------------------------------------------

const std::string PIECENAMES[13] = {
	"P",
	"N",
	"B",
	"R",
	"Q",
	"K",
	"p",
	"n",
	"b",
	"r",
	"q",
	"k",
	".",
};
const std::string RULES[26] = {
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
const std::string COLORSTR[2] = {"Speler kleur : Wit", "Speler kleur : Zwart"};
const int COLORINT[2] = {PWHITE, PBLACK};
const std::string TIMESTR[7] = {
	"Speeltijd : 5 min",
	"Speeltijd : 10 min",
	"Speeltijd : 15 min",
	"Speeltijd : 30 min",
	"Speeltijd : 45 min",
	"Speeltijd : 60 min",
	"Speeltijd : 100 min",
};
const int TIMEINT[7] = {300, 600, 900, 1800, 2700, 3600, 6000};
const std::string PLUSSTR[7] = {
	"Plus tijd per zet : 0 sec",
	"Plus tijd per zet : 2 sec",
	"Plus tijd per zet : 3 sec",
	"Plus tijd per zet : 5 sec",
	"Plus tijd per zet : 10 sec",
	"Plus tijd per zet : 15 sec",
	"Plus tijd per zet : 30 sec",
};
const int PLUSINT[7] = {0, 2, 3, 5, 10, 15, 30};
const std::string X_CO[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
const std::string Y_CO[8] = {"8", "7", "6", "5", "4", "3", "2", "1"};
const char TOPROW[8][2] = {
	{BK, E8},
	{BQ, D8},
	{BR, A8},
	{BR, H8},
	{BB, C8},
	{BB, F8},
	{BN, B8},
	{BN, G8}};
const char BOTROW[8][2] = {
	{WK, E1},
	{WQ, D1},
	{WR, A1},
	{WR, H1},
	{WB, C1},
	{WB, F1},
	{WN, B1},
	{WN, G1}};
const std::string NUMBER[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
const std::string GAMEOVERTXT[10] = {
	"Onbekend",
	"Wit wint, zwart staat mat",
	"Wit wint, zwart heeft geen bedenktijd meer",
	"Zwart wint, wit staat mat",
	"Zwart wint, wit heeft geen bedenktijd meer",
	"Remise door onvoldoende mat materiaal",
	"Remise de witte koning staat pat",
	"Remise de zwart koning staat pat",
	"Remise door herhaling van zetten",
	"Remise door de 50 zetten regel",
};
const int PROMOTEPIECE[2][4] = {
	{WQ, WR, WB, WN},
	{BQ, BR, BB, BN},
};
const std::string SQUARENAMES[65] = {
	"A8",
	"B8",
	"C8",
	"D8",
	"E8",
	"F8",
	"G8",
	"H8",
	"A7",
	"B7",
	"C7",
	"D7",
	"E7",
	"F7",
	"G7",
	"H7",
	"A6",
	"B6",
	"C6",
	"D6",
	"E6",
	"F6",
	"G6",
	"H6",
	"A5",
	"B5",
	"C5",
	"D5",
	"E5",
	"F5",
	"G5",
	"H5",
	"A4",
	"B4",
	"C4",
	"D4",
	"E4",
	"F4",
	"G4",
	"H4",
	"A3",
	"B3",
	"C3",
	"D3",
	"E3",
	"F3",
	"G3",
	"H3",
	"A2",
	"B2",
	"C2",
	"D2",
	"E2",
	"F2",
	"G2",
	"H2",
	"A1",
	"B1",
	"C1",
	"D1",
	"E1",
	"F1",
	"G1",
	"H1",
	"EDGE",
};
const std::string COLORNAMES[2] = {
	"White", "Black"};
const int CASTLE_MASK[64] = {
	7, 15, 15, 15, 3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14};
const int KNIGHT_MOVES[64][8] = {
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		C7,
		EDGE,
		B6,
	}, // A8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		D7,
		A6,
		C6,
	}, // B8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		A7,
		E7,
		B6,
		D6,
	}, // C8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		B7,
		F7,
		C6,
		E6,
	}, // D8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		C7,
		G7,
		D6,
		F6,
	}, // E8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		D7,
		H7,
		E6,
		G6,
	}, // F8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		E7,
		EDGE,
		F6,
		H6,
	}, // G8
	{
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		F7,
		EDGE,
		G6,
		EDGE,
	}, // H8
	{
		EDGE,
		EDGE,
		EDGE,
		C8,
		EDGE,
		C6,
		EDGE,
		B5,
	}, // A7
	{
		EDGE,
		EDGE,
		EDGE,
		D8,
		EDGE,
		D6,
		A5,
		C5,
	}, // B7
	{
		EDGE,
		EDGE,
		A8,
		E8,
		A6,
		E6,
		B5,
		D5,
	}, // C7
	{
		EDGE,
		EDGE,
		B8,
		F8,
		B6,
		F6,
		C5,
		E5,
	}, // D7
	{
		EDGE,
		EDGE,
		C8,
		G8,
		C6,
		G6,
		D5,
		F5,
	}, // E7
	{
		EDGE,
		EDGE,
		D8,
		H8,
		D6,
		H6,
		E5,
		G5,
	}, // F7
	{
		EDGE,
		EDGE,
		E8,
		EDGE,
		E6,
		EDGE,
		F5,
		H5,
	}, // G7
	{
		EDGE,
		EDGE,
		F8,
		EDGE,
		F6,
		EDGE,
		G5,
		EDGE,
	}, // H7
	{
		EDGE,
		B8,
		EDGE,
		C7,
		EDGE,
		C5,
		EDGE,
		B4,
	}, // A6
	{
		A8,
		C8,
		EDGE,
		D7,
		EDGE,
		D5,
		A4,
		C4,
	}, // B6
	{
		B8,
		D8,
		A7,
		E7,
		A5,
		E5,
		B4,
		D4,
	}, // C6
	{
		C8,
		E8,
		B7,
		F7,
		B5,
		F5,
		C4,
		E4,
	}, // D6
	{
		D8,
		F8,
		C7,
		G7,
		C5,
		G5,
		D4,
		F4,
	}, // E6
	{
		E8,
		G8,
		D7,
		H7,
		D5,
		H5,
		E4,
		G4,
	}, // F6
	{
		F8,
		H8,
		E7,
		EDGE,
		E5,
		EDGE,
		F4,
		H4,
	}, // G6
	{
		G8,
		EDGE,
		F7,
		EDGE,
		F5,
		EDGE,
		G4,
		EDGE,
	}, // H6
	{
		EDGE,
		B7,
		EDGE,
		C6,
		EDGE,
		C4,
		EDGE,
		B3,
	}, // A5
	{
		A7,
		C7,
		EDGE,
		D6,
		EDGE,
		D4,
		A3,
		C3,
	}, // B5
	{
		B7,
		D7,
		A6,
		E6,
		A4,
		E4,
		B3,
		D3,
	}, // C5
	{
		C7,
		E7,
		B6,
		F6,
		B4,
		F4,
		C3,
		E3,
	}, // D5
	{
		D7,
		F7,
		C6,
		G6,
		C4,
		G4,
		D3,
		F3,
	}, // E5
	{
		E7,
		G7,
		D6,
		H6,
		D4,
		H4,
		E3,
		G3,
	}, // F5
	{
		F7,
		H7,
		E6,
		EDGE,
		E4,
		EDGE,
		F3,
		H3,
	}, // G5
	{
		G7,
		EDGE,
		F6,
		EDGE,
		F4,
		EDGE,
		G3,
		EDGE,
	}, // H5
	{
		EDGE,
		B6,
		EDGE,
		C5,
		EDGE,
		C3,
		EDGE,
		B2,
	}, // A4
	{
		A6,
		C6,
		EDGE,
		D5,
		EDGE,
		D3,
		A2,
		C2,
	}, // B4
	{
		B6,
		D6,
		A5,
		E5,
		A3,
		E3,
		B2,
		D2,
	}, // C4
	{
		C6,
		E6,
		B5,
		F5,
		B3,
		F3,
		C2,
		E2,
	}, // D4
	{
		D6,
		F6,
		C5,
		G5,
		C3,
		G3,
		D2,
		F2,
	}, // E4
	{
		E6,
		G6,
		D5,
		H5,
		D3,
		H3,
		E2,
		G2,
	}, // F4
	{
		F6,
		H6,
		E5,
		EDGE,
		E3,
		EDGE,
		F2,
		H2,
	}, // G4
	{
		G6,
		EDGE,
		F5,
		EDGE,
		F3,
		EDGE,
		G2,
		EDGE,
	}, // H4
	{
		EDGE,
		B5,
		EDGE,
		C4,
		EDGE,
		C2,
		EDGE,
		B1,
	}, // A3
	{
		A5,
		C5,
		EDGE,
		D4,
		EDGE,
		D2,
		A1,
		C1,
	}, // B3
	{
		B5,
		D5,
		A4,
		E4,
		A2,
		E2,
		B1,
		D1,
	}, // C3
	{
		C5,
		E5,
		B4,
		F4,
		B2,
		F2,
		C1,
		E1,
	}, // D3
	{
		D5,
		F5,
		C4,
		G4,
		C2,
		G2,
		D1,
		F1,
	}, // E3
	{
		E5,
		G5,
		D4,
		H4,
		D2,
		H2,
		E1,
		G1,
	}, // F3
	{
		F5,
		H5,
		E4,
		EDGE,
		E2,
		EDGE,
		F1,
		H1,
	}, // G3
	{
		G5,
		EDGE,
		F4,
		EDGE,
		F2,
		EDGE,
		G1,
		EDGE,
	}, // H3
	{
		EDGE,
		B4,
		EDGE,
		C3,
		EDGE,
		C1,
		EDGE,
		EDGE,
	}, // A2
	{
		A4,
		C4,
		EDGE,
		D3,
		EDGE,
		D1,
		EDGE,
		EDGE,
	}, // B2
	{
		B4,
		D4,
		A3,
		E3,
		A1,
		E1,
		EDGE,
		EDGE,
	}, // C2
	{
		C4,
		E4,
		B3,
		F3,
		B1,
		F1,
		EDGE,
		EDGE,
	}, // D2
	{
		D4,
		F4,
		C3,
		G3,
		C1,
		G1,
		EDGE,
		EDGE,
	}, // E2
	{
		E4,
		G4,
		D3,
		H3,
		D1,
		H1,
		EDGE,
		EDGE,
	}, // F2
	{
		F4,
		H4,
		E3,
		EDGE,
		E1,
		EDGE,
		EDGE,
		EDGE,
	}, // G2
	{
		G4,
		EDGE,
		F3,
		EDGE,
		F1,
		EDGE,
		EDGE,
		EDGE,
	}, // H2
	{
		EDGE,
		B3,
		EDGE,
		C2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // A1
	{
		A3,
		C3,
		EDGE,
		D2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // B1
	{
		B3,
		D3,
		A2,
		E2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // C1
	{
		C3,
		E3,
		B2,
		F2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // D1
	{
		D3,
		F3,
		C2,
		G2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // E1
	{
		E3,
		G3,
		D2,
		H2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // F1
	{
		F3,
		H3,
		E2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // G1
	{
		G3,
		EDGE,
		F2,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
		EDGE,
	}, // H1
};
const int BISHOP_MOVES[64][4] = {
	{
		EDGE,
		EDGE,
		EDGE,
		B7,
	}, // A8
	{
		EDGE,
		EDGE,
		A7,
		C7,
	}, // B8
	{
		EDGE,
		EDGE,
		B7,
		D7,
	}, // C8
	{
		EDGE,
		EDGE,
		C7,
		E7,
	}, // D8
	{
		EDGE,
		EDGE,
		D7,
		F7,
	}, // E8
	{
		EDGE,
		EDGE,
		E7,
		G7,
	}, // F8
	{
		EDGE,
		EDGE,
		F7,
		H7,
	}, // G8
	{
		EDGE,
		EDGE,
		G7,
		EDGE,
	}, // H8
	{
		EDGE,
		B8,
		EDGE,
		B6,
	}, // A7
	{
		A8,
		C8,
		A6,
		C6,
	}, // B7
	{
		B8,
		D8,
		B6,
		D6,
	}, // C7
	{
		C8,
		E8,
		C6,
		E6,
	}, // D7
	{
		D8,
		F8,
		D6,
		F6,
	}, // E7
	{
		E8,
		G8,
		E6,
		G6,
	}, // F7
	{
		F8,
		H8,
		F6,
		H6,
	}, // G7
	{
		G8,
		EDGE,
		G6,
		EDGE,
	}, // H7
	{
		EDGE,
		B7,
		EDGE,
		B5,
	}, // A6
	{
		A7,
		C7,
		A5,
		C5,
	}, // B6
	{
		B7,
		D7,
		B5,
		D5,
	}, // C6
	{
		C7,
		E7,
		C5,
		E5,
	}, // D6
	{
		D7,
		F7,
		D5,
		F5,
	}, // E6
	{
		E7,
		G7,
		E5,
		G5,
	}, // F6
	{
		F7,
		H7,
		F5,
		H5,
	}, // G6
	{
		G7,
		EDGE,
		G5,
		EDGE,
	}, // H6
	{
		EDGE,
		B6,
		EDGE,
		B4,
	}, // A5
	{
		A6,
		C6,
		A4,
		C4,
	}, // B5
	{
		B6,
		D6,
		B4,
		D4,
	}, // C5
	{
		C6,
		E6,
		C4,
		E4,
	}, // D5
	{
		D6,
		F6,
		D4,
		F4,
	}, // E5
	{
		E6,
		G6,
		E4,
		G4,
	}, // F5
	{
		F6,
		H6,
		F4,
		H4,
	}, // G5
	{
		G6,
		EDGE,
		G4,
		EDGE,
	}, // H5
	{
		EDGE,
		B5,
		EDGE,
		B3,
	}, // A4
	{
		A5,
		C5,
		A3,
		C3,
	}, // B4
	{
		B5,
		D5,
		B3,
		D3,
	}, // C4
	{
		C5,
		E5,
		C3,
		E3,
	}, // D4
	{
		D5,
		F5,
		D3,
		F3,
	}, // E4
	{
		E5,
		G5,
		E3,
		G3,
	}, // F4
	{
		F5,
		H5,
		F3,
		H3,
	}, // G4
	{
		G5,
		EDGE,
		G3,
		EDGE,
	}, // H4
	{
		EDGE,
		B4,
		EDGE,
		B2,
	}, // A3
	{
		A4,
		C4,
		A2,
		C2,
	}, // B3
	{
		B4,
		D4,
		B2,
		D2,
	}, // C3
	{
		C4,
		E4,
		C2,
		E2,
	}, // D3
	{
		D4,
		F4,
		D2,
		F2,
	}, // E3
	{
		E4,
		G4,
		E2,
		G2,
	}, // F3
	{
		F4,
		H4,
		F2,
		H2,
	}, // G3
	{
		G4,
		EDGE,
		G2,
		EDGE,
	}, // H3
	{
		EDGE,
		B3,
		EDGE,
		B1,
	}, // A2
	{
		A3,
		C3,
		A1,
		C1,
	}, // B2
	{
		B3,
		D3,
		B1,
		D1,
	}, // C2
	{
		C3,
		E3,
		C1,
		E1,
	}, // D2
	{
		D3,
		F3,
		D1,
		F1,
	}, // E2
	{
		E3,
		G3,
		E1,
		G1,
	}, // F2
	{
		F3,
		H3,
		F1,
		H1,
	}, // G2
	{
		G3,
		EDGE,
		G1,
		EDGE,
	}, // H2
	{
		EDGE,
		B2,
		EDGE,
		EDGE,
	}, // A1
	{
		A2,
		C2,
		EDGE,
		EDGE,
	}, // B1
	{
		B2,
		D2,
		EDGE,
		EDGE,
	}, // C1
	{
		C2,
		E2,
		EDGE,
		EDGE,
	}, // D1
	{
		D2,
		F2,
		EDGE,
		EDGE,
	}, // E1
	{
		E2,
		G2,
		EDGE,
		EDGE,
	}, // F1
	{
		F2,
		H2,
		EDGE,
		EDGE,
	}, // G1
	{
		G2,
		EDGE,
		EDGE,
		EDGE,
	}, // H1
};
const int ROOK_MOVES[64][4] = {
	{
		EDGE,
		EDGE,
		B8,
		A7,
	}, // A8
	{
		EDGE,
		A8,
		C8,
		B7,
	}, // B8
	{
		EDGE,
		B8,
		D8,
		C7,
	}, // C8
	{
		EDGE,
		C8,
		E8,
		D7,
	}, // D8
	{
		EDGE,
		D8,
		F8,
		E7,
	}, // E8
	{
		EDGE,
		E8,
		G8,
		F7,
	}, // F8
	{
		EDGE,
		F8,
		H8,
		G7,
	}, // G8
	{
		EDGE,
		G8,
		EDGE,
		H7,
	}, // H8
	{
		A8,
		EDGE,
		B7,
		A6,
	}, // A7
	{
		B8,
		A7,
		C7,
		B6,
	}, // B7
	{
		C8,
		B7,
		D7,
		C6,
	}, // C7
	{
		D8,
		C7,
		E7,
		D6,
	}, // D7
	{
		E8,
		D7,
		F7,
		E6,
	}, // E7
	{
		F8,
		E7,
		G7,
		F6,
	}, // F7
	{
		G8,
		F7,
		H7,
		G6,
	}, // G7
	{
		H8,
		G7,
		EDGE,
		H6,
	}, // H7
	{
		A7,
		EDGE,
		B6,
		A5,
	}, // A6
	{
		B7,
		A6,
		C6,
		B5,
	}, // B6
	{
		C7,
		B6,
		D6,
		C5,
	}, // C6
	{
		D7,
		C6,
		E6,
		D5,
	}, // D6
	{
		E7,
		D6,
		F6,
		E5,
	}, // E6
	{
		F7,
		E6,
		G6,
		F5,
	}, // F6
	{
		G7,
		F6,
		H6,
		G5,
	}, // G6
	{
		H7,
		G6,
		EDGE,
		H5,
	}, // H6
	{
		A6,
		EDGE,
		B5,
		A4,
	}, // A5
	{
		B6,
		A5,
		C5,
		B4,
	}, // B5
	{
		C6,
		B5,
		D5,
		C4,
	}, // C5
	{
		D6,
		C5,
		E5,
		D4,
	}, // D5
	{
		E6,
		D5,
		F5,
		E4,
	}, // E5
	{
		F6,
		E5,
		G5,
		F4,
	}, // F5
	{
		G6,
		F5,
		H5,
		G4,
	}, // G5
	{
		H6,
		G5,
		EDGE,
		H4,
	}, // H5
	{
		A5,
		EDGE,
		B4,
		A3,
	}, // A4
	{
		B5,
		A4,
		C4,
		B3,
	}, // B4
	{
		C5,
		B4,
		D4,
		C3,
	}, // C4
	{
		D5,
		C4,
		E4,
		D3,
	}, // D4
	{
		E5,
		D4,
		F4,
		E3,
	}, // E4
	{
		F5,
		E4,
		G4,
		F3,
	}, // F4
	{
		G5,
		F4,
		H4,
		G3,
	}, // G4
	{
		H5,
		G4,
		EDGE,
		H3,
	}, // H4
	{
		A4,
		EDGE,
		B3,
		A2,
	}, // A3
	{
		B4,
		A3,
		C3,
		B2,
	}, // B3
	{
		C4,
		B3,
		D3,
		C2,
	}, // C3
	{
		D4,
		C3,
		E3,
		D2,
	}, // D3
	{
		E4,
		D3,
		F3,
		E2,
	}, // E3
	{
		F4,
		E3,
		G3,
		F2,
	}, // F3
	{
		G4,
		F3,
		H3,
		G2,
	}, // G3
	{
		H4,
		G3,
		EDGE,
		H2,
	}, // H3
	{
		A3,
		EDGE,
		B2,
		A1,
	}, // A2
	{
		B3,
		A2,
		C2,
		B1,
	}, // B2
	{
		C3,
		B2,
		D2,
		C1,
	}, // C2
	{
		D3,
		C2,
		E2,
		D1,
	}, // D2
	{
		E3,
		D2,
		F2,
		E1,
	}, // E2
	{
		F3,
		E2,
		G2,
		F1,
	}, // F2
	{
		G3,
		F2,
		H2,
		G1,
	}, // G2
	{
		H3,
		G2,
		EDGE,
		H1,
	}, // H2
	{
		A2,
		EDGE,
		B1,
		EDGE,
	}, // A1
	{
		B2,
		A1,
		C1,
		EDGE,
	}, // B1
	{
		C2,
		B1,
		D1,
		EDGE,
	}, // C1
	{
		D2,
		C1,
		E1,
		EDGE,
	}, // D1
	{
		E2,
		D1,
		F1,
		EDGE,
	}, // E1
	{
		F2,
		E1,
		G1,
		EDGE,
	}, // F1
	{
		G2,
		F1,
		H1,
		EDGE,
	}, // G1
	{
		H2,
		G1,
		EDGE,
		EDGE,
	}, // H1
};
const int WPAWN_MOVES[64][3] = {
	{
		EDGE,
		EDGE,
		EDGE,
	}, // A8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // B8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // C8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // D8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // E8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // F8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // G8
	{
		EDGE,
		EDGE,
		EDGE,
	}, // H8
	{
		EDGE,
		A8,
		B8,
	}, // A7
	{
		A8,
		B8,
		C8,
	}, // B7
	{
		B8,
		C8,
		D8,
	}, // C7
	{
		C8,
		D8,
		E8,
	}, // D7
	{
		D8,
		E8,
		F8,
	}, // E7
	{
		E8,
		F8,
		G8,
	}, // F7
	{
		F8,
		G8,
		H8,
	}, // G7
	{
		G8,
		H8,
		EDGE,
	}, // H7
	{
		EDGE,
		A7,
		B7,
	}, // A6
	{
		A7,
		B7,
		C7,
	}, // B6
	{
		B7,
		C7,
		D7,
	}, // C6
	{
		C7,
		D7,
		E7,
	}, // D6
	{
		D7,
		E7,
		F7,
	}, // E6
	{
		E7,
		F7,
		G7,
	}, // F6
	{
		F7,
		G7,
		H7,
	}, // G6
	{
		G7,
		H7,
		EDGE,
	}, // H6
	{
		EDGE,
		A6,
		B6,
	}, // A5
	{
		A6,
		B6,
		C6,
	}, // B5
	{
		B6,
		C6,
		D6,
	}, // C5
	{
		C6,
		D6,
		E6,
	}, // D5
	{
		D6,
		E6,
		F6,
	}, // E5
	{
		E6,
		F6,
		G6,
	}, // F5
	{
		F6,
		G6,
		H6,
	}, // G5
	{
		G6,
		H6,
		EDGE,
	}, // H5
	{
		EDGE,
		A5,
		B5,
	}, // A4
	{
		A5,
		B5,
		C5,
	}, // B4
	{
		B5,
		C5,
		D5,
	}, // C4
	{
		C5,
		D5,
		E5,
	}, // D4
	{
		D5,
		E5,
		F5,
	}, // E4
	{
		E5,
		F5,
		G5,
	}, // F4
	{
		F5,
		G5,
		H5,
	}, // G4
	{
		G5,
		H5,
		EDGE,
	}, // H4
	{
		EDGE,
		A4,
		B4,
	}, // A3
	{
		A4,
		B4,
		C4,
	}, // B3
	{
		B4,
		C4,
		D4,
	}, // C3
	{
		C4,
		D4,
		E4,
	}, // D3
	{
		D4,
		E4,
		F4,
	}, // E3
	{
		E4,
		F4,
		G4,
	}, // F3
	{
		F4,
		G4,
		H4,
	}, // G3
	{
		G4,
		H4,
		EDGE,
	}, // H3
	{
		EDGE,
		A3,
		B3,
	}, // A2
	{
		A3,
		B3,
		C3,
	}, // B2
	{
		B3,
		C3,
		D3,
	}, // C2
	{
		C3,
		D3,
		E3,
	}, // D2
	{
		D3,
		E3,
		F3,
	}, // E2
	{
		E3,
		F3,
		G3,
	}, // F2
	{
		F3,
		G3,
		H3,
	}, // G2
	{
		G3,
		H3,
		EDGE,
	}, // H2
	{
		EDGE,
		A2,
		B2,
	}, // A1
	{
		A2,
		B2,
		C2,
	}, // B1
	{
		B2,
		C2,
		D2,
	}, // C1
	{
		C2,
		D2,
		E2,
	}, // D1
	{
		D2,
		E2,
		F2,
	}, // E1
	{
		E2,
		F2,
		G2,
	}, // F1
	{
		F2,
		G2,
		H2,
	}, // G1
	{
		G2,
		H2,
		EDGE,
	}, // H1
};
const int BPAWN_MOVES[64][3] = {
	{
		EDGE,
		A7,
		B7,
	}, // A8
	{
		A7,
		B7,
		C7,
	}, // B8
	{
		B7,
		C7,
		D7,
	}, // C8
	{
		C7,
		D7,
		E7,
	}, // D8
	{
		D7,
		E7,
		F7,
	}, // E8
	{
		E7,
		F7,
		G7,
	}, // F8
	{
		F7,
		G7,
		H7,
	}, // G8
	{
		G7,
		H7,
		EDGE,
	}, // H8
	{
		EDGE,
		A6,
		B6,
	}, // A7
	{
		A6,
		B6,
		C6,
	}, // B7
	{
		B6,
		C6,
		D6,
	}, // C7
	{
		C6,
		D6,
		E6,
	}, // D7
	{
		D6,
		E6,
		F6,
	}, // E7
	{
		E6,
		F6,
		G6,
	}, // F7
	{
		F6,
		G6,
		H6,
	}, // G7
	{
		G6,
		H6,
		EDGE,
	}, // H7
	{
		EDGE,
		A5,
		B5,
	}, // A6
	{
		A5,
		B5,
		C5,
	}, // B6
	{
		B5,
		C5,
		D5,
	}, // C6
	{
		C5,
		D5,
		E5,
	}, // D6
	{
		D5,
		E5,
		F5,
	}, // E6
	{
		E5,
		F5,
		G5,
	}, // F6
	{
		F5,
		G5,
		H5,
	}, // G6
	{
		G5,
		H5,
		EDGE,
	}, // H6
	{
		EDGE,
		A4,
		B4,
	}, // A5
	{
		A4,
		B4,
		C4,
	}, // B5
	{
		B4,
		C4,
		D4,
	}, // C5
	{
		C4,
		D4,
		E4,
	}, // D5
	{
		D4,
		E4,
		F4,
	}, // E5
	{
		E4,
		F4,
		G4,
	}, // F5
	{
		F4,
		G4,
		H4,
	}, // G5
	{
		G4,
		H4,
		EDGE,
	}, // H5
	{
		EDGE,
		A3,
		B3,
	}, // A4
	{
		A3,
		B3,
		C3,
	}, // B4
	{
		B3,
		C3,
		D3,
	}, // C4
	{
		C3,
		D3,
		E3,
	}, // D4
	{
		D3,
		E3,
		F3,
	}, // E4
	{
		E3,
		F3,
		G3,
	}, // F4
	{
		F3,
		G3,
		H3,
	}, // G4
	{
		G3,
		H3,
		EDGE,
	}, // H4
	{
		EDGE,
		A2,
		B2,
	}, // A3
	{
		A2,
		B2,
		C2,
	}, // B3
	{
		B2,
		C2,
		D2,
	}, // C3
	{
		C2,
		D2,
		E2,
	}, // D3
	{
		D2,
		E2,
		F2,
	}, // E3
	{
		E2,
		F2,
		G2,
	}, // F3
	{
		F2,
		G2,
		H2,
	}, // G3
	{
		G2,
		H2,
		EDGE,
	}, // H3
	{
		EDGE,
		A1,
		B1,
	}, // A2
	{
		A1,
		B1,
		C1,
	}, // B2
	{
		B1,
		C1,
		D1,
	}, // C2
	{
		C1,
		D1,
		E1,
	}, // D2
	{
		D1,
		E1,
		F1,
	}, // E2
	{
		E1,
		F1,
		G1,
	}, // F2
	{
		F1,
		G1,
		H1,
	}, // G2
	{
		G1,
		H1,
		EDGE,
	}, // H2
	{
		EDGE,
		EDGE,
		EDGE,
	}, // A1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // B1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // C1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // D1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // E1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // F1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // G1
	{
		EDGE,
		EDGE,
		EDGE,
	}, // H1
};

// ------------------------------------------------------------------------------------------------
// End Tables
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Hashcode
// ------------------------------------------------------------------------------------------------

Hashcode *haskey_generator;

/// @brief Constructor
Hashcode::Hashcode()
{
    random_state = 1804289383;
    for (int piece = WP; piece <= BK; piece++)
    {
        for (int square = 0; square < 64; square++)
            piece_keys[piece][square] = Get64BitRandom();
        cap_keys[piece] = Get64BitRandom();    
    }
    for (int square = 0; square < 64; square++)
        enpassant_keys[square] = Get64BitRandom();
    for (int index = 0; index < 16; index++)
        castle_keys[index] = Get64BitRandom();
    side_key = Get64BitRandom();
}

/// @brief Get a 32 bit random value
/// @return 32 bit unsigned value
U32 Hashcode::Get32BitRandom()
{
    // get current state
    U32 number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}

/// @brief Get a 64 bit random value
/// @return 64 bit unsigned value 
U64 Hashcode::Get64BitRandom()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;
    
    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(Get32BitRandom()) & 0xFFFF;
    n2 = (U64)(Get32BitRandom()) & 0xFFFF;
    n3 = (U64)(Get32BitRandom()) & 0xFFFF;
    n4 = (U64)(Get32BitRandom()) & 0xFFFF;
    
    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

/// @brief Get the hashcode
/// @param board        array of board
/// @param cap          array of captures
/// @param enpassant    en passant move or NP
/// @param castle       castle code 0 = None, or WKS(1)|WQS(2)|BKS(3)|BQS(4)  
/// @param side2move    black or white
/// @return 64 bit hashcode
U64 Hashcode::Get(int *board, int *cap, int enpassant, int castle, int side2move)
{
    U64 key = 0ULL;

    for (int square = 0; square < 64; square++)
    {
        if (board[square] == NP)
            continue;
        key ^= piece_keys[board[square]][square];
    }
    for (int piece = 0; piece < 32; piece++)
    {
        if (cap[piece] == NP)
            continue;    
        key ^= cap_keys[cap[piece]];
    }
    key ^= castle_keys[castle];
    if (enpassant != NOVALUE)
        key ^= enpassant_keys[enpassant];
    if (side2move == PBLACK)
        key ^= side_key; 

    return key;   
}

// ------------------------------------------------------------------------------------------------
// End hashcode
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Images
// ------------------------------------------------------------------------------------------------

Images *images;

/// @brief Constructor
Images::Images()
{
	texture = new Texture2D[32];
	texture[IMG_TABLE] = LoadTexture("assets/images/Table.png");
	texture[IMG_TABLE].width = SCREENWIDTH;
	texture[IMG_TABLE].height = SCREENHEIGHT;
	texture[IMG_CHESS_BOARD] = LoadTexture("assets/images/Board.png");
	texture[IMG_CHESS_BOARD].width = BOARDSIZE;
	texture[IMG_CHESS_BOARD].height = BOARDSIZE;
	texture[IMG_LARGE_PIECES + WP] = LoadTexture("assets/images/PawnW.png");
	texture[IMG_SMALL_PIECES + WP] = LoadTexture("assets/images/PawnW.png");
	texture[IMG_LARGE_PIECES + WN] = LoadTexture("assets/images/KnightW.png");
	texture[IMG_SMALL_PIECES + WN] = LoadTexture("assets/images/KnightW.png");
	texture[IMG_LARGE_PIECES + WB] = LoadTexture("assets/images/BishopW.png");
	texture[IMG_SMALL_PIECES + WB] = LoadTexture("assets/images/BishopW.png");
	texture[IMG_LARGE_PIECES + WR] = LoadTexture("assets/images/RookW.png");
	texture[IMG_SMALL_PIECES + WR] = LoadTexture("assets/images/RookW.png");
	texture[IMG_LARGE_PIECES + WQ] = LoadTexture("assets/images/QueenW.png");
	texture[IMG_SMALL_PIECES + WQ] = LoadTexture("assets/images/QueenW.png");
	texture[IMG_LARGE_PIECES + WK] = LoadTexture("assets/images/KingW.png");
	texture[IMG_SMALL_PIECES + WK] = LoadTexture("assets/images/KingW.png");
	texture[IMG_LARGE_PIECES + BP] = LoadTexture("assets/images/PawnB.png");
	texture[IMG_SMALL_PIECES + BP] = LoadTexture("assets/images/PawnB.png");
	texture[IMG_LARGE_PIECES + BN] = LoadTexture("assets/images/KnightB.png");
	texture[IMG_SMALL_PIECES + BN] = LoadTexture("assets/images/KnightB.png");
	texture[IMG_LARGE_PIECES + BB] = LoadTexture("assets/images/BishopB.png");
	texture[IMG_SMALL_PIECES + BB] = LoadTexture("assets/images/BishopB.png");
	texture[IMG_LARGE_PIECES + BR] = LoadTexture("assets/images/RookB.png");
	texture[IMG_SMALL_PIECES + BR] = LoadTexture("assets/images/RookB.png");
	texture[IMG_LARGE_PIECES + BQ] = LoadTexture("assets/images/QueenB.png");
	texture[IMG_SMALL_PIECES + BQ] = LoadTexture("assets/images/QueenB.png");
	texture[IMG_LARGE_PIECES + BK] = LoadTexture("assets/images/KingB.png");
	texture[IMG_SMALL_PIECES + BK] = LoadTexture("assets/images/KingB.png");
	for (int i = 0; i < 12; i++)
	{
		texture[IMG_LARGE_PIECES + i].width = LARGE_PIECE_SIZE;
		texture[IMG_LARGE_PIECES + i].height = LARGE_PIECE_SIZE;
		texture[IMG_SMALL_PIECES + i].width = SMALL_PIECE_SIZE;
		texture[IMG_SMALL_PIECES + i].height = SMALL_PIECE_SIZE;
	}
	texture[IMG_PLAYERS + HUMAN] = LoadTexture("assets/images/HUMAN.png");
	texture[IMG_PLAYERS + HUMAN].width = SQUARESIZE;
	texture[IMG_PLAYERS + HUMAN].height = SQUARESIZE;
	texture[IMG_PLAYERS + AI] = LoadTexture("assets/images/AI.png");
	texture[IMG_PLAYERS + AI].width = SQUARESIZE;
	texture[IMG_PLAYERS + AI].height = SQUARESIZE;
	texture[IMG_ENTER_BTN] = LoadTexture("assets/images/Enter.png");
	texture[IMG_ENTER_BTN].height = SQUARESIZE;
	texture[IMG_ENTER_BTN].width = SQUARESIZE * 2;
	texture[IMG_PLUS_MIN_BTN] = LoadTexture("assets/images/Plusmin.png");
	texture[IMG_PLUS_MIN_BTN].height = SQUARESIZE;
	texture[IMG_PLUS_MIN_BTN].width = SQUARESIZE * 2;
	texture[IMG_CHOICE_BTN] = LoadTexture("assets/images/Choice.png");
	texture[IMG_CHOICE_BTN].height = SQUARESIZE;
	texture[IMG_CHOICE_BTN].width = SQUARESIZE;
	texture[IMG_CHESS_CLOCK] = LoadTexture("assets/images/clock.png");
	texture[IMG_CHESS_CLOCK].width = SQUARESIZE * 2;
	texture[IMG_CHESS_CLOCK].height = SQUARESIZE;
}

/// @brief Destructor
Images::~Images()
{
	for (int i = 0; i < 32; i++)
	{
		UnloadTexture(texture[i]);
	}
}

/// @brief Get an image
/// @param imagetype type image
/// @param index index or 0
/// @return image
Texture2D Images::Get(int imagetype, int index)
{
	return texture[imagetype + index];
}

// ------------------------------------------------------------------------------------------------
// End Images
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Piece
// ------------------------------------------------------------------------------------------------

Piece empty_piece = {NP, 0UL, false, false};

// ------------------------------------------------------------------------------------------------
// End Piece
// ------------------------------------------------------------------------------------------------

int main()
{
#if GENERATE
    int sqf, sqt;
    // knight moves
    std::cout << "const int KNIGHT_MOVES[64][8] = {\n";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            sqf = y * 8 + x;
            std::cout << "{";
            for (int dy = -2; dy <= 2; dy++)
            {
                for (int dx = -2; dx <= 2; dx++)
                {
                    if (dy == 0 || dx == 0 || ABS(dy) == ABS(dx))
                        continue;
                    if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
                        sqt = (y + dy) * 8 + x + dx;
                    else
                        sqt = EDGE;
                    std::cout << SQUARENAMES[sqt] << ",";
                }
            }
            std::cout << "}, //" << SQUARENAMES[sqf] << std::endl; 
        }
    }
    std::cout << "};\n";
    // bishop moves
    std::cout << "const int BISHOP_MOVES[64][4] = {\n";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            sqf = y * 8 + x;
            std::cout << "{";
            for (int dy = -1; dy <= 1; dy+=2)
            {
                for (int dx = -1; dx <= 1; dx+=2)
                {
                    if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
                        sqt = (y + dy) * 8 + x + dx;
                    else
                        sqt = EDGE;
                    std::cout << SQUARENAMES[sqt] << ",";
                }
            }
            std::cout << "}, //" << SQUARENAMES[sqf] << std::endl; 
        }
    }
    std::cout << "};\n";
    // rook moves
    std::cout << "const int ROOK_MOVES[64][4] = {\n";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            sqf = y * 8 + x;
            std::cout << "{";
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (ABS(dy) == ABS(dx))
                        continue;
                    if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
                        sqt = (y + dy) * 8 + x + dx;
                    else
                        sqt = EDGE;
                    std::cout << SQUARENAMES[sqt] << ",";
                }
            }
            std::cout << "}, //" << SQUARENAMES[sqf] << std::endl; 
        }
    }
    std::cout << "};\n";
    // white pawn moves
    std::cout << "const int WPAWN_MOVES[64][3] = {\n";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            sqf = y * 8 + x;
            std::cout << "{";
            int dy = -1;
            for (int dx = -1; dx <= 1; dx++)
            {
                if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
                    sqt = (y + dy) * 8 + x + dx;
                else
                    sqt = EDGE;
                std::cout << SQUARENAMES[sqt] << ",";
            }
            std::cout << "}, //" << SQUARENAMES[sqf] << std::endl; 
        }
    }
    std::cout << "};\n";
    // black pawn moves
    std::cout << "const int BPAWN_MOVES[64][3] = {\n";
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            sqf = y * 8 + x;
            std::cout << "{";
            int dy = 1;
            for (int dx = -1; dx <= 1; dx++)
            {
                if (y + dy >= 0 && y + dy <= 7 && x + dx >= 0 && x + dx <= 7)
                    sqt = (y + dy) * 8 + x + dx;
                else
                    sqt = EDGE;
                std::cout << SQUARENAMES[sqt] << ",";
            }
            std::cout << "}, //" << SQUARENAMES[sqf] << std::endl; 
        }
    }
    std::cout << "};\n";
    return 0;
#endif

    // initialize
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "RAYLIB Doorgeefschaak");
    images = new Images();
    game = new Game();
    left_board = new Board(true);
    right_board = new Board(false);
    haskey_generator = new Hashcode(); 
    SetTargetFPS(10);

    // mainloop
    while (!WindowShouldClose())
    {
        // update
        game->Update();

        // draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game->Draw();
        DrawFPS(1, 1);
        EndDrawing();

        // keypress
        if (IsKeyPressed(KEY_ENTER))
        {
            game->Enter_handler();
        }
        else if (IsKeyPressed(KEY_F9))
        {
            game->Key_handler(298);
        }
        else if (IsKeyPressed(KEY_F10))
        {
            game->Key_handler(299);
        }
        else if (IsKeyPressed(KEY_F11))
        {
            game->Key_handler(300);
        }

        // Mouse Press
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x = GetMouseX();
            int y = GetMouseY();
            game->Mouse_handler(x, y);
        }

        // Ai Move
        game->AiMove();
    }

    // clean up
    delete game;
    delete left_board;
    delete right_board;
    delete images;
    delete haskey_generator;
    CloseWindow();
    return 0;
}
