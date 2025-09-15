#pragma once

#include "raylib.h"

typedef struct
{
	int choice_color;
	int choice_time;
	int choice_plus;
	int game_time;
	int game_plus_time;
	int fkey;
	int game_state;
} Game_Data;

class Game
{
private:
	Game_Data *data;

public:
	Game();
	~Game();
	void Update();
	void Setup();
	void Draw();
	void Enter_handler();
	void Key_handler(int key);
	void Mouse_handler(int x, int y);
	void AiMove();
};

extern Game *game;
