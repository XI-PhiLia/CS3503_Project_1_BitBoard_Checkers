#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "BitManipulation.h"

// declaration of helper functions
int SquareIndex(int row, int col);
int IsKingThere(unsigned int pieces, unsigned int kings, int position);
int IsOccupied(unsigned int board, int position);
bool DiagonalPiece1(int from, int to);
bool DiagonalPiece2(int from, int to);
bool DiagonalKing(int from, int to);
bool IsEmpty(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces);
bool PossibleCapture(int from, int over, int to);
bool IsOpponentAt(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);
bool HasCapture(int from, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);

void SetUpBoard(unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings) {
    *p1_pieces = *p2_pieces = *p1_kings = *p2_kings = 0; // sets all to 0

    for (int row = 0; row < 3; row++) { // sets pieces for player 2
        for (int col = 0; col < 8; col++) {
            int bitIndex = SquareIndex(row, col);
            if (bitIndex != -1) {
                *p1_pieces |= 1 << bitIndex;
            }
        }
    }
    for (int row = 5; row < 8; row++) { // sets pieces for player 1
        for (int col = 0; col < 8; col++) {
            int bitIndex = SquareIndex(row, col);
            if (bitIndex != -1) {
                *p2_pieces |= 1 << bitIndex;
            }
        }
    }
}

void PrintBoard(unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings) {
    // checkers pieces
    wchar_t w_piece = L'⛂'; 
    wchar_t w_king = L'⛃';
    wchar_t b_piece = L'⛀';
    wchar_t b_king = L'⛁';
    wchar_t w_square = L'▯';
    wchar_t b_square = L'▮';
    wprintf(L"   A B C D E F G H\n   ---------------\n");
    for(int row = 7; row >= 0; row--) { // goes through each of the bitboards and prints the pieces
        wprintf(L"%d |", row + 1);  // Row label
        for(int col = 0; col < 8; col++) {
        int bitIndex = SquareIndex(row, col);
        if(bitIndex != -1) {
            if((*p1_pieces >> bitIndex) & 1) { // checks position of player 1 pieces and prints it
                wprintf(L"%lc ", w_piece);
            }
            else if((*p1_kings >> bitIndex) & 1) { // checks for and prints king piece
                    wprintf(L"%lc ", w_king);
                }
            else if((*p2_pieces >> bitIndex ) & 1) {// checks player 2...
                wprintf(L"%lc ", b_piece);
            }
            else if((*p2_kings >> bitIndex) & 1) {
                    wprintf(L"%lc ", b_king);
                }
            else
                wprintf(L"%lc ", w_square); // prints white squares
        }
        else 
            wprintf(L"%lc ", b_square); // prints black (playable) squares
    }
    wprintf(L"| %d", row + 1);
    wprintf(L"\n"); // 8 squares new row
    }
    wprintf(L"   ---------------\n   A B C D E F G H\n");
}

// move piece, returns -1 if capture failed, returns the end position if capture is successful
int MovePiece(unsigned int from, int to, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn) {
    if(p1_turn) {
        if(GetBit(*p2_pieces, from) || GetBit(*p2_kings, from)) { // p1 tries to move p2 pieces
            wprintf(L"This is not your piece!\n");
            return -1;
        }
        else if (GetBit(*p1_pieces, to) || GetBit(*p2_pieces, to)) { // checks if the new spot is occupied
            wprintf(L"Destination square is already occupied.\n");
            return -1;
        }
        else if(GetBit(*p1_kings, from)){ // checks if its a king
            if(DiagonalKing(from, to)) {
                *p1_kings = ClearBit(*p1_kings, from); 
                *p1_kings = SetBit(*p1_kings, to); // "moves" the king
                return to;
            }
            else {
                wprintf(L"Your king cannot move there!\n");
                return -1;
            }
        }
        else if(!GetBit(*p1_pieces, from)) { // no pieces here
            wprintf(L"There are no pieces here to move.\n");
            return -1;
        }
        else if(GetBit(*p1_pieces, from)){ // checks if the piece is there
            if(DiagonalPiece1(from, to)) {
                if (p1_turn && to >= 28) { // piece is moved to top, can promote to king
                    *p1_pieces = ClearBit(*p1_pieces, from);
                    *p1_kings = SetBit(*p1_kings, to); // sets p1 piece to king
                    return to;
                }
                *p1_pieces = ClearBit(*p1_pieces, from); 
                *p1_pieces = SetBit(*p1_pieces, to); // "moves" the piece
                return to;
            }
            else {
                wprintf(L"Your piece cannot move there!\n"); 
                return -1;
            }
        }
        else {
            wprintf(L"Unknown error.\n");
            return -1;
        }
    }
    else {
        if(GetBit(*p1_pieces, from) || GetBit(*p1_kings, from)) { // p1 tries to move p2 pieces
            wprintf(L"This is not your piece!\n");
            return -1;
        }
        else if (GetBit(*p1_pieces, to) || GetBit(*p2_pieces, to)) {
            wprintf(L"Destination square is already occupied.\n");
            return -1;
        }
        else if(GetBit(*p2_kings, from)){ // checks if its a king
            if(DiagonalKing(from, to)) {
                *p2_kings = ClearBit(*p2_kings, from);
                *p2_kings = SetBit(*p2_kings, to); // "moves" the king
                return to;
            }
            else {
                wprintf(L"Your king cannot move there!\n");
                return -1;
            }
        }
        else if(!GetBit(*p2_pieces, from)) { // no pieces here
            wprintf(L"There are no pieces here to move.\n");
            return -1;
        }
        else if(GetBit(*p2_pieces, from)){ // checks if the piece is there
            if(DiagonalPiece2(from, to)) {
                if (!p1_turn && to <= 3) { // piece is moved to the bottom, can promote to king
                    *p2_pieces = ClearBit(*p2_pieces, from);
                    *p2_kings = SetBit(*p2_kings, to); // sets p2 piece to king
                    return to;
                }
                *p2_pieces = ClearBit(*p2_pieces, from); 
                *p2_pieces = SetBit(*p2_pieces, to); // "moves" the piece
                return to;
            }
            else {
                wprintf(L"Your piece cannot move there!\n");
                return -1;
            }
        }
        else { // any other unknown error
            wprintf(L"Unknown error.\n");
            return -1;
        }
    }
}

int CapturePiece(unsigned int from, int to, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn) {
    int mid;
    int row = from/4;
    bool even_row = (row % 2 == 0);
    if(even_row)
        mid = (from + to + 1) / 2;
    else
        mid = (from + to) / 2;
    if(!IsEmpty(to, p1_pieces, p2_pieces)) { // checks if the destination square is empty 
        wprintf(L"Destination square is occupied.\n");
        return -1;
    }
    if(!PossibleCapture(from, mid, to)) { // checks if capture path is possible
        wprintf(L"Invalid capture path.\n");
        return -1;
    }
    if(!IsOpponentAt(mid, p1_pieces, p2_pieces, p1_kings, p2_kings, p1_turn)) { // checks if there is an opponent piece in between
        wprintf(L"No opponent piece found to capture.\n");
        return -1;
    }

    if(p1_turn) { 
        *p2_pieces = ClearBit(*p2_pieces, mid); // removes opponent piece (or king) here 
        *p2_kings = ClearBit(*p2_kings, mid);
        if(GetBit(*p1_kings, from)) { // for king
            *p1_kings = ClearBit(*p1_kings, from);
            *p1_kings = SetBit(*p1_kings, to);
        }
        else { // for normal pieces
            *p1_pieces = ClearBit(*p1_pieces, from);
            if(to >= 28) // promotes to king if capture takes the piece to the end positions
                *p1_kings = SetBit(*p1_kings, to);
            else
                *p1_pieces = SetBit(*p1_pieces, to);
        }
    }
    else {
        *p1_pieces = ClearBit(*p1_pieces, mid); // removes opponent piece (or king) here
        *p1_kings = ClearBit(*p1_kings, mid);
        if(GetBit(*p2_kings, from)) { // for king
            *p2_kings = ClearBit(*p2_kings, from);
            *p2_kings = SetBit(*p2_kings, to);
        }
        else { // for normal pieces
            *p2_pieces = ClearBit(*p2_pieces, from);
            if(to <= 3) // promotes to king if capture takes the piece to the end positions
                *p2_kings = SetBit(*p2_kings, to);
            else
                *p2_pieces = SetBit(*p2_pieces, to);
        }
    }
    return to;
}

// helper functions here:
// finds if there are any more possible captures
bool HasCapture(int from, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn) {
    int dir_to[4] = {7, 9, -7, -9};  // possible captures -> differences in bit index of to and from are +-7 and +-9 
    int row = from/4;
    bool even_row = (row % 2 == 0);
    int mid;
    bool is_king = p1_turn ? GetBit(*p1_kings, from) : GetBit(*p2_kings, from); // checks if the piece at position 'from' is a king
    for (int i = 0; i < 4; i++) { // loops through and checks if there are possible captures
        // Skip backward directions for non-kings
        if (!is_king) {
            if ((p1_turn && dir_to[i] < 0) || (!p1_turn && dir_to[i] > 0))
                continue;
        } 
        int to = from + dir_to[i];
        if(even_row)
            mid = (from + to + 1) / 2;
        else
            mid = (from + to) / 2;
        if (to < 0 || to >= 32 || mid < 0 || mid >= 32) // makes sure it's within the board
            continue;
        // checks if the position is empty, the capture path is valid, and if there's an opponent piece in between
        if (IsEmpty(to, p1_pieces, p2_pieces) && PossibleCapture(from, mid, to) && IsOpponentAt(mid, p1_pieces, p2_pieces, p1_kings, p2_kings, p1_turn)) {
            return true;
        }
    }
    return false; // no more possible captures
}

// converts board coordinate to bit index
int SquareIndex(int row, int col) { 
    if ((row + col) % 2 == 1) {
        return (row * 4) + (col / 2); // black squares
    }
    return -1; // white square
}

bool IsEmpty(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces) { // checks if the spot is empty
    return !GetBit(*p1_pieces, pos) && !GetBit(*p2_pieces, pos);
}

// checks if there's an opponent piece at the position
bool IsOpponentAt(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn) {
    if (p1_turn)
        return GetBit(*p2_pieces, pos) || GetBit(*p2_kings, pos); 
    else
        return GetBit(*p1_pieces, pos) || GetBit(*p1_kings, pos);
}

// checks if the capture is valid
bool PossibleCapture(int from, int mid, int to) {
    int row = mid/4;
    bool even_row = (row%2==0);
    if(even_row) {
       return   (to >= 0 && to < 32) &&
                (abs(to - from) == 7 || abs(to - from) == 9) && 
                (mid - from == 4 || mid - from == 3 || mid - from == -4 || mid - from == -5);  // for even rows
    }
    else {
        return  (to >= 0 && to < 32) &&
                (abs(to - from) == 7 || abs(to - from) == 9) &&
                (mid - from == 4 || mid - from == 5 || mid - from == -3 || mid - from == -4); // for odd rows
    }
}

// checks if a move diagonal is possible for player 1
bool DiagonalPiece1(int from, int to) { 
    int diff = to-from;
    return diff == 3 || diff == 4 || diff == 5;
}

// checks if a move diagonal is possible for player 2
bool DiagonalPiece2(int from, int to) { 
    int diff = to-from; 
    return diff == -3 || diff == -4 || diff == -5;
}

// checks if a move diagonal is possible for kings
bool DiagonalKing(int from, int to) { 
    int diff = to-from; 
    return diff == 4 || diff == 5|| diff == -4 || diff == -5 || diff == 3 || diff == -3;
}