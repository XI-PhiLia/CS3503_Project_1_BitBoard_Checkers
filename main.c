#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#include "BitManipulation.h"
#include "Board.h"

typedef struct { 
    unsigned int p1_pieces;
    unsigned int p2_pieces;
    unsigned int p1_kings;
    unsigned int p2_kings;
    int current_turn;
} DetailedGame;

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    _setmode(_fileno(stdout),_O_U8TEXT);
    DetailedGame game = {0};
    game.current_turn = 0;
    SetUpBoard(&game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings);
    bool gameover = false;
    while(!gameover) {
        char from_col, to_col;
        char input[4];
        char direction[3] = {0};
        int from_row, to_row, to;
        int player = (game.current_turn % 2) + 1;
        bool p1_turn = (player==1);
        PrintBoard(&game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings);
        wprintf(L"Current turn: Player %d ", (game.current_turn%2)+1); // prints the player's turn
        if(p1_turn) 
                wprintf(L"(White)\n");
            else
                wprintf(L"(Black)\n");
        wprintf(L"Menu:\n1. Move a piece\n2. Capture a piece\n3. Quit\n");
        wprintf(L"Choose an option:");
        char choice;
        scanf(" %c", &choice);
        if (choice == '1') {
            // gets the starting col,row and ending col,row for moving
            wprintf(L"\nEnter starting square (example: C3): ");
            scanf(" %3s", &input);
            if(strlen(input)<2) {
                wprintf(L"Invalid square. Try again.\n");
                continue;
            }
            from_row = input[1] - '0';
            from_col = toupper(input[0]);
            wprintf(L"Enter destination: Right (R), Left (L), Backwards-Right (BR), Backwards-Left (BL):");
            scanf(" %2s", &direction); 
            for (int i = 0; i < 2; i++) // converts to uppercase
                direction[i] = toupper(direction[i]);
            int from = SquareIndex(from_row - 1, from_col - 'A'); // converts letter-number coordinate to bit index
            int row = from/4;
            bool even_row = (row % 2 == 0);
            int true_col = (row % 2 == 0) ? ((from % 4) * 2 + 1) : ((from % 4) * 2); // the real column of the bit index
            int change = 0;
            if(strcmp(direction, "R")==0) { // moves piece to the right, changes based on the player and even/odd row
                if(from_row % 2 == 2 && true_col == 7) { // prevents pieces on the right side from moving off the board
                    wprintf(L"Invalid move. Try again.\n");
                    continue;
                }
                change = p1_turn ? (even_row ? 5 : 4) : (even_row ? -3 : -4);
            }
            else if(strcmp(direction, "L")==0) { // moves pieces to the left
                if(from_row % 2 == 0 && true_col == 0) { // prevents pieces on the left side from moving off the board
                    wprintf(L"Invalid move. Try again.\n");
                    continue;
                }
                change = p1_turn ? (even_row ? 4 : 3) : (even_row ? -4 : -5);
            }
            else if(strcmp(direction, "BR")==0) { // moves pieces backwards to the right
                if(from_row % 2 == 2 && true_col == 7) { // prevents pieces on the right side from moving off the board
                    wprintf(L"Invalid move. Try again.\n");
                    continue;
                }
                change = p1_turn ? (even_row ? -3 : -4) : (even_row ? 5 : 4);
            }
            else if(strcmp(direction, "BL")==0) { // moves pieces backwards to the left
                if(from_row % 2 == 0 && true_col == 0) { // prevents pieces on the left side from moving off the board
                    wprintf(L"Invalid move. Try again.\n");
                    continue;
                }
                change = p1_turn ? (even_row ? -4 : -5) : (even_row ? 4 : 3);
            }
            else {
                wprintf(L"Invalid option. Try again.\n");
                continue;
            }
            to = from + change; // calculates the end position based on the offset (change) and starting square
            // double checks that the position moved to/moved from is within the 32 bit board
            if (from < 0 || from > 31 || to > 31 || to < 0) { 
                wprintf(L"Invalid square(s). Try again.\n");
                continue;
            }
            if(MovePiece(from, to, &game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings, p1_turn)==-1) {
                continue;
            }
        } 
        else if (choice == '2') {
            // gets the starting col,row and ending col,row for capturing
            wprintf(L"Enter starting square (example: C3): ");
            scanf(" %c%d", &from_col, &from_row);
            from_col = toupper(from_col);
            wprintf(L"Enter destination square (example: E4): ");
            scanf(" %c%d", &to_col, &to_row);
            to_col = toupper(to_col);
            int from = SquareIndex(from_row - 1, from_col - 'A'); // converts letter-number coordinate to bit index
            int to = SquareIndex(to_row - 1, to_col - 'A');
            // checks that the position moved to/moved from is within the 32 bit board
            if (from < 0 || from > 31 || to > 31 || to < 0) { 
                wprintf(L"Invalid square(s). Try again.\n");
                continue;
            }
            int result = CapturePiece(from, to, &game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings, p1_turn);
            if(result == -1)
                continue;
            int current_pos = result;
            // checks if there are still available captures and loops until captures aren't available
            while (HasCapture(current_pos, &game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings, p1_turn)) {
                PrintBoard(&game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings); // prints current board
                wprintf(L"Another capture is available from here. Enter next destination: ");
                scanf(" %c%d", &to_col, &to_row);
                to_col = toupper(to_col);
                int next_to = SquareIndex(to_row - 1, to_col - 'A'); // next position to move to
                if (next_to < 0 || next_to >= 32) {
                    wprintf(L"Invalid square. Ending chain capture.\n"); // ends chain capture if user inputs an invalid square
                    break;
                }
                // tries to capture next piece
                int next_result = CapturePiece(current_pos, next_to, &game.p1_pieces, &game.p2_pieces, &game.p1_kings, &game.p2_kings, p1_turn);
                if (next_result == -1) {
                    wprintf(L"Ending chain capture.\n"); // ends chain capture if the capture isn't possible
                    break;
                }
                current_pos = next_result;
            }
        } 
        else if (choice == '3') { // ends the game
            wprintf(L"\nGame ended by Player %d.", player);
            break;
        }
        else {
            wprintf(L"\nInvalid option. Try again.\n");
            continue;
        }
        // Check win condition, loses if either player has no pieces remaining
        if (CountBits(game.p1_pieces | game.p1_kings) == 0) {
            wprintf(L"Player 2 wins!\n");
            gameover = true;
        } 
        else if (CountBits(game.p2_pieces | game.p2_kings) == 0) {
            wprintf(L"Player 1 wins!\n");
            gameover = true;
        } 
        else {
            game.current_turn++; // increases the game turn to determine which player's turn it is
        }
    }
    return 0;
}
