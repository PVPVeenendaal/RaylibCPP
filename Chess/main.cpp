#include <iostream>
#include <raylib.h>
#include <string>

// version
const std::string version = "0.10";
const std::string title = "BbChess in Raylib-C++ (C)2025 Peter Veenendaal; versie: " + version;
const std::string pieces[12] = {
	"PawnW", "KnightW", "BishopW", "RookW", "QueenW", "KingW",
	"PawnB", "KnightB", "BishopB", "RookB", "QueenB", "KingB", 
};
	
// ------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------

int main()
{
	const int SQUARESIZE = 80;
	const int BOARDSIZE = 80 * 8;
	const int SCREENWIDTH = BOARDSIZE + 40;
	const int SCREENHEIGHT = SQUARESIZE * 9 + 40;
	const int PIECESIZE = 72;
	int DRAWBOARD[64] = {
		 9,  7,  8, 10, 11,  8,  7,  9,
		 6,  6,  6,  6,  6,  6,  6,  6,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 3,  1,  2,  4,  5,  2,  1,  3,   
	};
	// initialize
    InitWindow(SCREENWIDTH, SCREENHEIGHT, title.c_str());
	Texture2D table = LoadTexture("./assets/Table.png");
	table.width = SCREENWIDTH;
	table.height = SCREENHEIGHT;
	Texture2D board = LoadTexture("./assets/Board.png");
	board.width = BOARDSIZE;
	board.height = BOARDSIZE;
	Texture2D img_pieces[12];
	for (int i = 0; i < 12; ++i)
	{
		std::string txt = "./assets/" + pieces[i] + ".png";
		img_pieces[i] = LoadTexture(txt.c_str());
		img_pieces[i].width = PIECESIZE;
		img_pieces[i].height = PIECESIZE;
	}

    SetTargetFPS(10);

    // mainloop
    while (!WindowShouldClose())
    {
        // update
    
        // draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
		// Draw table
		DrawTexture(
			table,
			0,
			0,
			RAYWHITE);
		// Draw board
		DrawRectangle(
			10, 
			10, 
			BOARDSIZE + 20, 
			BOARDSIZE + 20, 
			BROWN);
		DrawTexture(
			board, 
			20, 
			20, 
			RAYWHITE);
		// Draw pieces
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				int p = DRAWBOARD[y * 8 + x];
				if (p < 12)
				{
					DrawTexture(
						img_pieces[p], 
						x * SQUARESIZE + 24, 
						y * SQUARESIZE + 24, 
						RAYWHITE);
				}
			}
		}
        EndDrawing();

        // keypress
        if (IsKeyPressed(KEY_ENTER))
        {
        }
        else if (IsKeyPressed(KEY_F5))
        {
        }

        // Mouse Press
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x = GetMouseX();
            int y = GetMouseY();
        }

        // Ai Move
    }

    // clean up
	for (int i = 0; i < 12; ++i)
	{
		UnloadTexture(img_pieces[i]);
	}
	UnloadTexture(table);
	UnloadTexture(board);
    CloseWindow();
    return 0;
}
