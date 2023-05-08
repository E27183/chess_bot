#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct moveList {
    char from;
    char to;
    moveList* next;
};

struct moveListToOnly {
    char to;
    moveListToOnly* next;
};

moveListToOnly diagonalMove[64][4] = {};
moveListToOnly straightMove[64][4] = {};
moveListToOnly knightMove[64] = {};
moveListToOnly kingMove[64] = {};

class gameState {
    public:
        bool blackCanCastle;
        bool whiteCanCastle;
        bool canEnPassant;
        //Square the capturing piece would land on if accepted
        char enPassantTarget; 
        /*
        0 - empty
        x1 - Pawn
        x2 - Rook
        x3 - Knight
        x4 - Bishop
        x5 - Queen
        x6 - King
        1x - White
        2x - Black
        */
       char board[64];
       double time_remaining;
};

int main() {
    return 0;
};