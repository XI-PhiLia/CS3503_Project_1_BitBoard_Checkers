#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// Basic bit operations

// Set bit to 1
unsigned int SetBit(unsigned int value, int position) { 
    if (position < 0 || position >= 32) return value;
        return value | (1 << position);
}

// Set bit to 0
unsigned int ClearBit(unsigned int value, int position) { 
    if (position < 0 || position >= 32) return value;
        return value & ~(1 << position);
}

// Flip bit
unsigned int FlipBit(unsigned int value, int position) {  
    if (position < 0 || position >= 32) return value;
        return value ^ (1 << position);
}

// Get bit value
int GetBit(unsigned int value, int position) { 
    return (value >> position) & 1; 
}

// Creates mask
unsigned int CreateMask(int position) {
    return 1 << position;
}

// Counting and finding
// Count 1s
int CountBits(unsigned int value) {
    int count = 0;
    while(value) {
        count += value & 1; // count++ if rightmost bit is 1
        value >>= 1; // shift right to check next bit
    }
    return count;
}

// Shift operations
unsigned int ShiftLeft(unsigned int value, int positions) {
    return value << positions;
}
unsigned int ShiftRight(unsigned int value, int positions) {
    return value >> positions;
}

// Display functions
// Show in binary
void PrintBinary(unsigned int value) {
    int i = 31;
    while(i>=0) {
        printf("%d", GetBit(value, i)); // grabs & prints bits
        if(i%4 == 0 && i != 0) { // spaces between 4 bits
            printf(" ");
        }
        i--;
    }
}

// Show in hex
void PrintHex(unsigned int value) {
    printf("0x%08X\n", value);
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