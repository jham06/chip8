#ifndef CHIP8_H
#define CHIP8_H // I am now back in C, in order to avoid repeated defines of struct, need to implement the ifndef and define. 

#include <stdint.h> // Need to include this in order to work with numbers in C

// Need to initialize the components of the emulator
typedef struct {
    uint8_t ram[4096]; // Stores 4096 bytes
    uint8_t registers[16]; // store 16 registers from v0-vf
    uint8_t display[64*32]; // 1d array for simplciity, tis a display buffer. Width = 64, height =32. 


    uint16_t pc; 
    uint16_t idx; // specfically, these are 16 bits where 12 are used to access the 4096 bytes. Both pc and index
    uint16_t stack[16]; // because the chip8 had limited space, can only have 16 levels. In the future I'll make it bigger??
    uint16_t sp; // apprently i cant declare ptr in struct :(


    uint8_t keypad[16]; // checks to see if any of the 16 keys is pressed or not
    uint8_t delay_timer;
    uint8_t sound_timer; // these are both 8 bits

} Chip8;

#endif 

