#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// Basic bit operations- implement these or equivalent functions
unsigned int SetBit(unsigned int value, int position);

// Set bit to 1
unsigned int ClearBit(unsigned int value, int position); // Set bit to 0
unsigned int ToggleBit(unsigned int value, int position); // Flip bit
int GetBit(unsigned int value, int position); // Get bit value


// Counting and finding
int CountBits(unsigned int value); // Count 1s
 
// Shift operations
unsigned int ShiftLeft(unsigned int value, int positions);
unsigned int ShiftRight(unsigned int value, int positions);

// Display functions
void PrintBinary(unsigned int value); // Show in binary
void PrintHex(unsigned int value); // Show in hex

// Traditional approach
unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
        return value | (1 << position);
    }
// Alternative: Helper function approach
unsigned int CreateMask(int position) {
    return 1 << position;
}

unsigned int SetBitWithMask(unsigned int value, int position) {
    return value | CreateMask(position);
}

// Alternative: Combined function approach
unsigned int ModifyBit(unsigned int value, int position, int operation) {
    // operation: 0=clear, 1=set, 2=toggle
    switch(operation) {
        case 0: return value & ~(1 << position);
        case 1: return value | (1 << position);
        case 2: return value ^ (1 << position);
    }
    return value;
}