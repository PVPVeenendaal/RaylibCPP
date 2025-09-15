#pragma once

#include <vector>
#include <string>

#define SQR 0
#define PSQ 1
#define SQF 0
#define SQT 1

const std::string BOARDTEXT[51] = {
    "0",
    " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10",
    "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
    "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"
};

class Move
{
public:
    // default constructor
    Move();
    // constructor normal move
    Move(int sqf, char pcf, int sqt, char pct);
    // constructor capture move
    Move(int sqf, char pcf, int sqt, char pct, int sqo, char pco);
    // copy constructor
    Move(const Move &obj);
    // add a capture
    void AddCapture(int sqt, char pct, int sqo, char pco);
    // Search position
    int Search(int sqr);
    // Move to string
    std::string ToString();
    // properties
    int GetPosition(int idx) { return MoveData[idx][SQR]; }
    char GetPiece(int idx) { return MoveData[idx][PSQ]; }
    int GetSize() { return Step; }
    void SetPosition(int idx, int val) { MoveData[idx][SQR] = (char)val; }
    void SetPiece(int idx, char val) { MoveData[idx][PSQ] = val; }
    void Setsize(int val) { Step = val; }

private:
    char MoveData[22][2]; // [0] = square, [1] = piece
    int Step;
};

typedef std::vector<Move> MoveList;

typedef struct 
{
    Move move;
    int score;
} Gen_t;