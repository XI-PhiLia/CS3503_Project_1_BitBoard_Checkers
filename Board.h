#ifndef BOARD_H
#define BOARD_H
void SetUpBoard(unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings);
void PrintBoard(unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings);
int MovePiece(unsigned int from, int to, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);
int CapturePiece(unsigned int from, int to, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);

// helper functions
int SquareIndex(int row, int col);
int IsKingThere(unsigned int pieces, unsigned int kings, int position);
int IsOccupied(unsigned int *board, int position);
bool DiagonalPiece1(int from, int to);
bool DiagonalPiece2(int from, int to);
bool DiagonalKing(int from, int to);
bool IsEmpty(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces);
bool PossibleCapture(int from, int over, int to);
bool HasCapture(int from, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);
bool IsOpponentAt(int pos, unsigned int *p1_pieces, unsigned int *p2_pieces, unsigned int *p1_kings, unsigned int *p2_kings, bool p1_turn);
#endif