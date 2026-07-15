#include <stdio.h>
#include "chip8.h" // forgor to include this lol

// Need to include the necessary helper functions. 



void chip8_init (Chip8 *chip) {  
    // zero out the struct, as memset helps us do so 
    memset(chip, 0, sizeof(*chip)); 

    // set PC to 0x200, the starting point of the program
    chip->pc = 0x200;
    
    // Just hardcoding these font data. Copied from another open source code lul
    // load font data into RAM at 0x050. Just assign to hexadecimal
    const uint8_t sprites []= { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    memcpy(chip->ram + 0x050, sprites, sizeof(sprites)); // i wanna be like the cool kids, so storing font at 0x050
}

void chip8_load (Chip8 *chip, const char *filename) { // This should open the ROM file and load it into Ram starting at 0x200

    FILE *pf = fopen(filename, "rb"); // should use rb in this case as im reading binary files. 
    if (pf == NULL) {
        printf("NO file\n"); // if it doesnt exist, exit with code 1. 
        exit(1);
    }

    fseek(pf, 0, SEEK_END); // Need to determine 

    long file_size = ftell(pf);
    rewind(pf);
    if (file_size > 3584) {
        printf("File is too big");
        exit(1);
    }

    // otherwise it should read ROM into RAM

    fread(chip->ram+0x200, sizeof(uint8_t), file_size, pf);

    // now done with file pointer, so close it. 

    fclose(pf);
}


/*
Fetch — read 2 bytes from RAM at PC, combine into one 16-bit opcode, increment PC by 2
Decode — look at the first nibble to figure out what instruction it is
Execute — carry out the instruction
*/

void chip8_cycle (Chip8 *chip) {  // fetch/decode/execute code 
    uint16_t temp1 = chip->ram[chip->pc];
    uint16_t temp2 = chip->ram[chip->pc + 1];

    // C is a little endian. However, due to nautre of chip8, first byte read from memory is most sig byte. Store temp1 byte at uper 

    temp1 = temp1 << 8; 

    uint16_t instr = temp1 | temp2; 

    chip->pc += 2; // Incrememnt the PC by 2, not 4 because we are only working with 16 bits not 32 bits

    uint16_t opcode = instr & 0xFF00;
    
    switch (opcode)  {
        case 0x0:
            break;
        case 0x1:

            break;
        case 0x2:

            break;
        case 0x3:

            break;
        case 0x4:

            break;
        case 0x5:

            break;
        case 0x6:

            break;
        case 0x7:

            break;
        case 0x8:

            break;
        case 0x9:

            break;
        case 0xA:

            break;
        case 0xB:

            break;
        case 0xC: 

            break;
        case 0xD:

            break;
        case 0xE:

            break;
        case 0xF:

            break;
    }   
}

int main () {

   
}