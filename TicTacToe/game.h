#pragma once
#include <raylib.h>

class Game_t
{
public:
	Game_t();
	~Game_t();
	void Update();
	void Draw(int counter);
	void DoHumanMove(int keyval);
	int CpuMove();
	void DoAiMove(int move);
	void KeyPress(int key);
	void MousePress(int x, int y);
private:
	// methods
	void LoadImages();	
	void UnloadImages() const;
	void Init();
	bool IsWinner(int* brd, int plr);
	
	// data
	// image containers
	Texture2D img_plro;
	Texture2D img_plrx;
	// game data
	int board[9];
	int empty_squares;
	int side2move;
	int state;
	int winner;
	int dir_winner;
};
