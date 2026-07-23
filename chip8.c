#include <stdio.h>
#include "chip8.h" // forgor to include this lol
#include <stdlib.h>

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

    uint16_t opcode = instr & 0xF000; // Extract that specific 4 upper bits, the most significant nibble

    opcode = opcode >> 12; // Forgot to shift 12 to get what to do, 0000 0000 0000 0000 

    /*NOTE! I STILL HAVE ACCESS TO THE ORIGINAL INSTRUCTION*/
    
    switch (opcode)  { 
        case 0x0: // Two cases, 00E0 and 00EE
            uint16_t temp = instr & 0x00FF;
            switch (temp)  {
                case 0xE0: // Clear the display, 
                    memset(chip->display, 0, sizeof(chip->display));
                    break;
                case 0xEE: // pop last address from the stack and set PC to it
                    
                     // cant just pop in C, so I need to shift it
                    int stack_size = sizeof(chip->stack) / sizeof(chip->stack[0]);

                    uint16_t popped_temp = chip->stack[stack_size];
                
                    for (int i = 0; i < stack_size; i++) {
                        chip->stack[i] = chip->stack[i + 1];
                    } // shift elements to the left

                    chip->pc = popped_temp;
                    
                    break;
            }   
            break;
        case 0x1: // only consider 1NNN, simply set PC to NNN
            uint16_t location = instr & 0x0FFF; // Need to acquire NNN

            chip->pc = location;
            break;
        case 0x2:

            break;
        case 0x3:

            break;
        case 0x4:

            break;
        case 0x5:

            break;
        case 0x6: // simply set register VX to value NN, 6XNN
            uint16_t set_temp = instr & 0x00FF;

            uint16_t set_reg = (instr & 0x0F00) >> 8; // Need to acquie the decimal value
            
            chip->registers[set_reg] = set_temp; // set the register to the value NN
            break;
        case 0x7: // simply add value NN to VX
            uint16_t add_temp = instr & 0x00FF;

            uint16_t add_reg = (instr & 0x0F00) >> 8; // Need to acquie the decimal value
            
            chip->registers[add_reg] += add_temp; // okay mb, i thought VF would be affected
            
            break;
        case 0x8:

            break;
        case 0x9:

            break;
        case 0xA: // set the index register to value NNN
            uint16_t index_temp = instr & 0x0FFF;

            chip->idx = index_temp;
            break;
        case 0xB:

            break;
        case 0xC: 

            break;
        case 0xD: // DXYN This is drawing the display. Need X, Y coords and needs to set other things as well. 


            /* Now work for N rows.  Need to implement:
                - Get the Nth byte of sprite data, counting from the memory address in the I register (I is not incremented)
                - For each of the 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
                    - If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on, turn off the pixel and set VF to 1
                    - Or if the current pixel in the sprite row is on and the screen pixel is not, draw the pixel at the X and Y coordinates
                    - If you reach the right edge of the screen, stop drawing this row
                    - Increment X (VX is not incremented)
                - Increment Y (VY is not incremented)
                - Stop if you reach the bottom edge of the screen
            */


            uint16_t vx = (instr & 0x0F00) >> 8;
            uint16_t vy = (instr & 0x00F0) >> 4;
            uint16_t N = (instr & 0x000F); // Get N


            int xcord = (chip->registers[vx]) % 64; //
            int ycord = (chip->registers[vy]) % 32;
 
            chip->registers[15] = 0; // set VF to zero
            
            
            for (int i = 0; i < N; i++) { // access each row in the N rows
                // SO in this case, in order to access each of the 8 bits in the sprite row, i need to use bitwise operations. 
                // Should prolly have a inner forloop son im crine
                
                uint16_t n_temp = chip->ram[chip->idx+i]; 
                
                int xtemp = xcord;
                int ytemp = ycord;

                while (n_temp > 0) { // count from most to least significant bit 0000 0000, to be convinient shift left bc it is unsigned
                    // Shift the msb to the right
                    if (n_temp)  {
                        
                    } else {
                        xtemp++;
                        continue;
                    }
                }
                
        
                ytemp++;
                
                // IN order to access the coordinates X and Y in the display, since it is a 1d, I need to add some 
            }
            
            break;
        case 0xE:

            break;
        case 0xF:

            break;
    }   
}

int main () {

   
}