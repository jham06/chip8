#ifndef CHIP8_H
#define CHIP8_H // Apply ifndef and define to avoid repeated defines of struct

#include <stdint.h> 

// Initialize the components of the emulator
typedef struct {
    uint8_t ram[4096]; // Stores 4096 bytes
    uint8_t registers[16]; // store 16 registers from v0-vf, registers[15] always reserved. 
    uint8_t display[64*32]; // 1d array for simplciity, tis a display buffer. Width = 64, height =32. 


    uint16_t pc; 
    uint16_t idx; // 16 bits, but only 12 are used to access the 4096 bytes
    uint16_t stack[16]; // 16 levels of stack, 16 bits to store addresses to subroutines. 
    uint16_t sp; // apprently i cant declare ptr in struct :(. It will initially be 0, by memset. 


    uint8_t keypad[16]; // 
    uint8_t delay_timer;
    uint8_t sound_timer; //

} Chip8;

#endif 

