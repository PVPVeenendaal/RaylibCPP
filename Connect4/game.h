#pragma once
#include <utility>

class Game_t
{
public:
	Game_t();
	void Update();
	void Draw(int counter) const;
	void DoHumanMove(int keyval);
	void KeyPress(int key);
	void MousePress(int x, int y);
	void CpuMove();
private:	
	void Init();
	int GetNextOpenRow(int* brd, int col);
	bool IsValidLocation(int* brd, int col);
	int GetValidLocations(int* brd, int* data);
	bool IsTerminalNode(int* brd, int* data);
	std::pair<int, int> MiniMax(int* brd, int depth, int alpha, int beta, bool maximizingPlayer);
	void WinningMove(int* brd, int* data, int piece);
	int WindowCount(int* window, int piece);
	int EvaluateWindow(int* window, int piece);
	int ScorePosition(int* brd, int piece);

	// data

	// board
	//    x  x  x  x  x  x  x
	//    0  1  2  3  4  5  6
	// y|--------------------
	// 5|35 36 37 38 39 40 41
	// 4|28 29 30 31 32 33 34
	// 3|21 22 23 24 25 26 27
	// 2|14 15 16 17 18 19 20
	// 1| 7  8  9 10 11 12 13
	// 0| 0  1  2  3  4  5  6
	int board[42]; // row * col;
	int gamestate;
	int side2move;
	int winner;
	int dir_winner[5];
};

