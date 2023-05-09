#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct moveList {
    char from[2];
    char to[2];
    moveList* next;
};

struct moveListToOnly {
    char to[2];
    moveListToOnly* next;
};

struct fourMoveListToHeap {
    moveListToOnly* first;
    moveListToOnly* second;
    moveListToOnly* third;
    moveListToOnly* fourth;
};

moveListToOnly diagonalMove[8][8][4] = {};
moveListToOnly straightMove[8][8][4] = {};
moveListToOnly knightMove[8][8] = {};
moveListToOnly kingMove[8][8] = {};

void diagonalM(char* from, fourMoveListToHeap* outputPointer) {
    char position_x = *from;
    char position_y = *(from + 1);
    (*outputPointer).first = NULL;
    (*outputPointer).second = NULL;
    (*outputPointer).third = NULL;
    (*outputPointer).fourth = NULL;
    moveListToOnly* prev = NULL;
    while (position_x > 0 && position_y > 0) {
        position_x--;
        position_y--;
        moveListToOnly node;
        node.to[0] = position_x;
        node.to[1] = position_y;
        node.next = NULL;
        if (prev != NULL) {
            (*prev).next = &node;
        } else {
            (*outputPointer).first = &node;
        };
        prev = &node;
    };
    position_x = *from;
    position_y = *(from + 1);
    prev = NULL;
    while (position_x > 0 && position_y < 8) {
        position_x--;
        position_y++;
        moveListToOnly node;
        node.to[0] = position_x;
        node.to[1] = position_y;
        node.next = NULL;
        if (prev != NULL) {
            (*prev).next = &node;
        } else {
            (*outputPointer).second = &node;
        };
        prev = &node;
    };
    position_x = *from;
    position_y = *(from + 1);
    prev = NULL;
    while (position_x < 8 && position_y > 0) {
        position_x++;
        position_y--;
        moveListToOnly node;
        node.to[0] = position_x;
        node.to[1] = position_y;
        node.next = NULL;
        if (prev != NULL) {
            (*prev).next = &node;
        } else {
            (*outputPointer).third = &node;
        };
        prev = &node;
    };
    position_x = *from;
    position_y = *(from + 1);
    prev = NULL;
    while (position_x < 8 && position_y < 8) {
        position_x++;
        position_y++;
        moveListToOnly node;
        node.to[0] = position_x;
        node.to[1] = position_y;
        node.next = NULL;
        if (prev != NULL) {
            (*prev).next = &node;
        } else {
            (*outputPointer).fourth = &node;
        };
        prev = &node;
    };
};

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
       char board[8][8];
       double time_remaining;
};

int main() {
    char d[2] = {3, 0};
    fourMoveListToHeap dummyres;
    diagonalM(d, &dummyres);
    return 0;
};