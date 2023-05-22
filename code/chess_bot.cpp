#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <cstdlib>
using namespace std;

class gameState {
    public:
        bool blackCanCastleShort;
        bool whiteCanCastleShort;
        bool blackCanCastleLong;
        bool whiteCanCastleLong;
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

//x -> up/down
//y -> left/right

char initial_board[8][8] = {{22, 23, 24, 25, 26, 24, 23, 22}, {21, 21, 21, 21, 21, 21, 21, 21}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {11, 11, 11, 11, 11, 11, 11, 11}, {12, 13, 14, 15, 16, 14, 13, 12}};

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

// void handleExit() { //Maybe dangerous if it hides error message while developing
//     cout << "Exiting program.\n";
//     abort();
// };

void printBoard(char* board, bool whiteToMove) {
    string header = whiteToMove ? "   A B C D E F G H\n  +-+-+-+-+-+-+-+-+\n" : "   H G F E D C B A\n  +-+-+-+-+-+-+-+-+\n";
    cout << header;
    for (int i = 0; i < 8; i++) {
        cout << char(49 + (whiteToMove ? 7 - i : i)) << " |";
        for (int j = 0; j < 8; j++) {
            char pointerOffset = whiteToMove ? i*8 + j : 63 - (i * 8) - j;
            cout << printPiece(*(board + pointerOffset)) << '|';
        };
        cout << "\n  +-+-+-+-+-+-+-+-+\n";
    };
};

bool gameOver = false;

bool kingAttackedCheck(char* board, bool white) {
    char kingX;
    char kingY;
    bool flag = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (*(board + i*8 + j) == 26 - (white * 10)) {
                flag = true;
                kingX = i;
                kingY = j;
                break;
            };
        };
        if (flag) {
            break;
        };
    };
    for (char x : {-1, 1}) { //Is checked by knight?
        for (char y : {-2, 2}) {
            if (kingX + x >= 0 && kingX + x < 8 && kingY + y >= 0 && kingY + y < 8 && *(board + 8 * (kingX + x) + kingY + y) == 13 + (white * 10)) {
                return true;
            };
            if (kingX + y >= 0 && kingX + y < 8 && kingY + x >= 0 && kingY + x < 8 && *(board + 8 * (kingX + y) + kingY + x) == 13 + (white * 10)) {
                return true;
            };
        };
    };
    char diagonal_direction[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (char i = 0; i < 4; i++) { //Is checked diagonally?
        char tmpx = kingX + diagonal_direction[i][0];
        char tmpy = kingY + diagonal_direction[i][1];
        while (tmpy >= 0 && tmpx >= 0 && tmpy < 8 && tmpx < 8 && board[8 * tmpx + tmpy] == 0) {
            tmpx += diagonal_direction[i][0];
            tmpy += diagonal_direction[i][1];
        };
        if (tmpy >= 0 && tmpx >= 0 && tmpy < 8 && tmpx < 8 && (board[8 * tmpx + tmpy] == 14 + (white * 10) || board[8 * tmpx + tmpy] == 15 + (white * 10))) {
            return true;
        };
    };
    char straight_direction[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
    for (char i = 0; i < 4; i++) { //Is checked straight?
        char tmpx = kingX + straight_direction[i][0];
        char tmpy = kingY + straight_direction[i][1];
        while (tmpy >= 0 && tmpx >= 0 && tmpy < 8 && tmpx < 8 && board[8 * tmpx + tmpy] == 0) {
            tmpx += straight_direction[i][0];
            tmpy += straight_direction[i][1];
        };
        if (tmpy >= 0 && tmpx >= 0 && tmpy < 8 && tmpx < 8 && (board[8 * tmpx + tmpy] == 12 + (white * 10) || board[8 * tmpx + tmpy] == 15 + (white * 10))) {
            return true;
        };
    };
    char radial_direction[8][2] = {{-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}};
    for (char i = 0; i < 8; i++) { //Is checked by king?
        char tmpx = kingX + radial_direction[i][0];
        char tmpy = kingY + radial_direction[i][1];
        if (tmpx >= 0 && tmpx < 8 && tmpy >= 0 && tmpy < 8 && board[8 * tmpx + tmpy] == 16 + (white * 10)) {
            return true;
        };
    };
    char pawn_take_direction[2][2] = {{(char)(1 - (2 * white)), 1}, {(char)(1 - (2 * white)), -1}};
    for (char i = 0; i < 2; i++) {
        char tmpx = kingX + pawn_take_direction[i][0];
        char tmpy = kingY + pawn_take_direction[i][1];
        if (tmpx >= 0 && tmpx < 8 && tmpy >= 0 && tmpy < 8 && board[8 * tmpx + tmpy] == 11 + (white * 10)) {
            return true;
        };
    };
    return false;
};

bool legalMove(gameState* board, char* moveNode, bool white, bool playIfLegal) {
    char fromX = moveNode[0] / 8;
    char fromY = moveNode[0] % 8;
    char toX = moveNode[1] / 8;
    char toY = moveNode[1] % 8;
    char preserve_old_piece = board->board[toX][toY];
    if (moveNode[2] != 0) {
        switch(moveNode[2]) { //Note: currently does NOT account for moving in to check
            case 31:
                if (!board->canEnPassant || board->enPassantTarget != moveNode[1] || board->board[fromX][fromY] != 21 - (10 * white) || (toY - fromY != 1 && fromY - toY != 1) || (white && fromX - toX != 1) || (!white && toX - fromX != 1) || board->board[toX][toY] != 0) {
                    return false;
                };
                char takenX = fromX;
                char takenY = toY;
                board->board[takenX][takenY], board->board[fromX][fromY] = 0;
                board->board[toX][toY] = 21 - (10 * white);
                bool flag = kingAttackedCheck(&(board->board[0][0]), white);
                if (flag || !playIfLegal) {
                    board->board[takenX][takenY] = 11 + (10 * white);
                    board->board[fromX][fromY] = 21 - (10 * white);
                    board->board[toX][toY] = 0;
                };
                return !flag;
            case 32:
                if ((white && !board->whiteCanCastleShort) || (!white && !board->blackCanCastleShort)) {
                    return false;
                };
                if (white) {
                    if (board->board[7][6] == 0 && board->board[7][5] == 0) {
                        if (playIfLegal) {
                            board->board[7][7], board->board[7][4] = 0;
                            board->board[7][6] = 16;
                            board->board[7][5] = 12;
                            board->whiteCanCastleLong = false;
                            board->whiteCanCastleShort = false;
                        };
                        return true;
                    } else {
                        return false;
                    };
                } else {
                    if (board->board[0][6] == 0 && board->board[0][5] == 0) {
                        if (playIfLegal) {
                            board->board[0][7], board->board[0][4] = 0;
                            board->board[0][6] = 26;
                            board->board[0][5] = 22;
                            board->blackCanCastleLong = false;
                            board->blackCanCastleShort = false;
                        };
                        return true;
                    } else {
                        return false;
                    };
                };
            case 33:
                if ((white && !board->whiteCanCastleLong) || (!white && !board->blackCanCastleLong)) {
                    return false;
                };
                if (white) {
                    if (board->board[7][1] == 0 && board->board[7][2] == 0 && board->board[7][3] == 0) {
                        if (playIfLegal) {
                            board->board[7][0], board->board[7][4] = 0;
                            board->board[7][2] = 16;
                            board->board[7][3] = 12;
                            board->whiteCanCastleLong = false;
                            board->whiteCanCastleShort = false;
                        };
                        return true;
                    } else {
                        return false;
                    };
                } else {
                    if (board->board[0][6] == 0 && board->board[0][5] == 0) {
                        if (playIfLegal) {
                            board->board[0][7], board->board[0][4] = 0;
                            board->board[0][6] = 26;
                            board->board[0][5] = 22;
                            board->blackCanCastleLong = false;
                            board->blackCanCastleShort = false;
                        };
                        return true;
                    } else {
                        return false;
                    };
                };                
        };
    };
};

int main() {
    srand((unsigned int)time(NULL));
    int initial_rng = rand();
    bool iAmWhite = initial_rng > (RAND_MAX / 2);
    string userColour = iAmWhite ? "black" : "white";
    cout << "Starting game\nBlack pieces are uppercase, white pieces are lowercase\nTo input moves, type in the square you want to move from and the square you want to move to, lowercase and separated by a space\nExample: 'e2 e4' (without the quotes)\nYou may also type 'resign' without quotes to resign\nYou are playing as " << userColour << "\nGood luck!\n\n";    
    // set_terminate(handleExit);
    char moveTrace[30][3]; //From, to, piece taken? (special values: 31 is en passant, 32 is short castle event, 33 is long castle event)
    char moveIndex; //This is a number I just like chars bc they're small
    gameState exploringBoardstate;
    gameState actualBoardState;
    actualBoardState.blackCanCastleShort = true;
    actualBoardState.blackCanCastleLong = true;
    actualBoardState.canEnPassant = false;
    actualBoardState.whiteCanCastleShort = true;
    actualBoardState.whiteCanCastleLong = true;
    actualBoardState.whiteToMove = true;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            actualBoardState.board[i][j] = initial_board[i][j];
        };
    };
    do {
        if (actualBoardState.whiteToMove == iAmWhite) {
            gameOver = true;
        } else {
            printBoard(&(actualBoardState.board[0][0]), actualBoardState.whiteToMove);
            cout << "Input your next move: ";
            string line;
            getline(cin, line);
            if (line == "resign") {
                gameOver = true;
                cout << "\nGame Over\n";
            } else if (line.size() != 5 || line[0] > 104 || line[3] > 104 || line[0] < 97 || line[3] < 97 || line[1] < 49 || line[4] < 49 || line[1] > 56 || line[4] > 56 || line[2] != 32) {
                cout << "\nInvalid Move Syntax\n";
            } else {
                char pieceToMove = actualBoardState.board[56 - line[1]][104 - line[0]];
                if ((actualBoardState.whiteToMove && (pieceToMove > 16 || pieceToMove < 11)) || (!actualBoardState.whiteToMove && (pieceToMove > 26 || pieceToMove < 21))) {
                    cout << "\nIllegal Move\n";
                } else {
                    cout << bool(kingAttackedCheck(&(actualBoardState.board[0][0]), actualBoardState.whiteToMove)) << "aaa";
                };
            };
        };
    } while (!gameOver);
    return 0;
};