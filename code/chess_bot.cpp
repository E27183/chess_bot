#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <cstdlib>
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

// void diagonalM(char* from, fourMoveListToHeap* outputPointer) {
//     cout << "       ah        ";
//     char position_x = *from;
//     char position_y = *(from + 1);
//     (*outputPointer).first = NULL;
//     (*outputPointer).second = NULL;
//     (*outputPointer).third = NULL;
//     (*outputPointer).fourth = NULL;
//     moveListToOnly* prev = NULL;
//     while (position_x > 0 && position_y > 0) {
//         position_x--;
//         position_y--;
//         moveListToOnly node;
//         node.to[0] = position_x;
//         node.to[1] = position_y;
//         node.next = NULL;
//         if (prev != NULL) {
//             (*prev).next = &node;
//         } else {
//             (*outputPointer).first = &node;
//         };
//         prev = &node;
//     };
//     position_x = *from;
//     position_y = *(from + 1);
//     prev = NULL;
//     while (position_x > 0 && position_y < 8) {
//         position_x--;
//         position_y++;
//         moveListToOnly node;
//         node.to[0] = position_x;
//         node.to[1] = position_y;
//         node.next = NULL;
//         if (prev != NULL) {
//             (*prev).next = &node;
//         } else {
//             (*outputPointer).second = &node;
//         };
//         prev = &node;
//     };
//     position_x = *from;
//     position_y = *(from + 1);
//     prev = NULL;
//     while (position_x < 8 && position_y > 0) {
//         position_x++;
//         position_y--;
//         moveListToOnly node;
//         node.to[0] = position_x;
//         node.to[1] = position_y;
//         node.next = NULL;
//         if (prev != NULL) {
//             (*prev).next = &node;
//         } else {
//             (*outputPointer).third = &node;
//         };
//         prev = &node;
//     };
//     position_x = *from;
//     position_y = *(from + 1);
//     prev = NULL;
//     while (position_x < 8 && position_y < 8) {
//         position_x++;
//         position_y++;
//         moveListToOnly node;
//         node.to[0] = position_x;
//         node.to[1] = position_y;
//         node.next = NULL;
//         if (prev != NULL) {
//             (*prev).next = &node;
//         } else {
//             (*outputPointer).fourth = &node;
//         };
//         prev = &node;
//     };
// };

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
       double timeRemaining;
       bool whiteToMove;
};

char initial_board[8][8] = {{12, 13, 14, 15, 16, 14, 13, 12}, {11, 11, 11, 11, 11, 11, 11, 11}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {21, 21, 21, 21, 21, 21, 21, 21}, {22, 23, 24, 25, 26, 24, 23, 22}};

int pieceValue(char* piece) {
    switch(*piece) {
        case 0:
            return 0;
        case 11:
            return 1;
        case 21:
            return -1;
        case 12:
            return 5;
        case 22:
            return -5;
        case 13:
            return 3;
        case 23:
            return -3;
        case 14:
            return 3;
        case 24:
            return -3;
        case 15:
            return 9;
        case 25:
            return -9;
        default:
            return 0;
    };
};

int simpleEval(char* board) {
    int res = 0;
    for (int i = 0; i < 64; i++) {
        res += *(board + i);
    };
    return res;
};

void aaa() {
    cout << "Hello";
};

char printPiece(char piece) {
    switch (piece)
    {
    case 0:
        return ' ';
    case 11:
        return 'p';
    case 21:
        return 'P';
    case 12:
        return 'r';
    case 13:
        return 'n';
    case 14:
        return 'b';
    case 15:
        return 'q';
    case 16:
        return 'k';
    case 22:
        return 'R';
    case 23:
        return 'N';
    case 24:
        return 'B';
    case 25:
        return 'Q';
    case 26:
        return 'K';
    default:
        return '!';
    };
};

void printBoard(char* board) {
    cout << "+-+-+-+-+-+-+-+-+\n";
    for (int i = 0; i < 8; i++) {
        cout << '|';
        for (int j = 0; j < 8; j++) {
            cout << printPiece(*(board + i*8 + j)) << '|';
        };
        cout << "\n+-+-+-+-+-+-+-+-+\n";
    };
};

int main() {    
    // thread identifier (aaa);
    // cout << "       gh        ";
    // char d[2] = {3, 0};
    // fourMoveListToHeap dummyres;
    // diagonalM(d, &dummyres);
    // cout << "       gh        ";
    // cout << dummyres.third;
    // cout << "       gh        ";
    // moveListToOnly* tmp = dummyres.second;
    // cout << "a  ";
    // while (tmp != NULL) {
    //     cout << "Ready";
    //     cout << int((*tmp).to[0]) << "  " << int((*tmp).to[1]) << "  ";
    //     tmp = (*tmp).next;
    // };
    // cout << "b  ";
    char moveTrace[30][3]; //From, to, piece taken? (special values: 31 is en passant, 32 is castle event)
    char moveIndex; //This is a number I just like chars bc they're small
    gameState exploringBoardstate;
    gameState actualBoardState;
    actualBoardState.blackCanCastle = true;
    actualBoardState.canEnPassant = false;
    actualBoardState.whiteCanCastle = true;
    actualBoardState.whiteToMove = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            actualBoardState.board[i][j] = initial_board[i][j];
        };
    };
    int initial_rng = rand();
    bool iAmWhite = initial_rng * 2 > RAND_MAX;
    printBoard(&(actualBoardState.board[0][0]));
    // char piece = 24;
    // int v = pieceValue(&piece);
    // cout << v;
    // identifier.join();
    return 0;
};