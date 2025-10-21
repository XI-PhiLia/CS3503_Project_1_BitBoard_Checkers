# CS 3503 Project 1 - Bitboard Checkers Game

## Author
Darren

## Description
My implementation of Owl Tech ’s BitBoard Game Engine.

## Build Instructions
‘‘‘bash
gcc main.c BitManipulation.c Board.c -o main.exe
./main.exe
‘‘‘

## Directions
1. Have a friend<br>
2. Player 1 are the white pieces, player 2 are the black pieces. King promotion is through moving a piece to opposite end of the board for player 1 or 2. <br>
3. Moving Pieces <br>
Select 1 in the menu. Then type in the right column letter and row number as input for the square on the board. Then select R, L, BL, or BR. Note: BL and BR for player 2 moves up the board (for kings), R and L moves down the board.<br>
4. Capturing Pieces <br>
Select 2 in the menu. Then type in the column, row of the starting square and destination square (If there is an opponent's piece in between).<br>
5. Win condition: Eliminating all opponent pieces.

## Test Results
This Checkers Bitboard game engine has the basic functions of capturing, moving, quitting, and simple win condition of a player having 0 checkers pieces left. With the capturing function allowing for multiple captures in one turn.

## Notes
The hardest part of this project was finding ways to implement successful bit manipulations into functions of the game (capturing, moving, etc.). 
<br>Overall, working on this project has given me a great understanding of bitboards and the applications of bit manipulations on these bitboards.<br>