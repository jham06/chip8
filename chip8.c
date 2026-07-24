// Get necessary headers and libraries needed. 
#include <stdio.h>
#include "chip8.h" 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>


void chip8_init (Chip8 *chip) {  
    // zero out the struct using memset  
    memset(chip, 0, sizeof(*chip)); 

    // set PC to 0x200, i.e. starting point of program
    chip->pc = 0x200;
    
    // Hardcoding these font data. Copied from another open source code lul. Load font data into RAM at 0x050.
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

    memcpy(chip->ram + 0x050, sprites, sizeof(sprites)); // wanna be like cool kids, store at 0x050

   
}

void chip8_load (Chip8 *chip, const char *filename) { // Open the ROM file and load it into Ram starting at 0x200

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
            uint16_t temp0 = instr & 0x00FF;
            switch (temp0)  {
                case 0xE0: // Clear the display, 
                    memset(chip->display, 0, sizeof(chip->display));
                    break;
                case 0xEE: // pop last address from the stack and set PC to it. Use sp, don't overcomplicate
        
                    chip->sp--;
                    uint16_t popped_temp = chip->stack[chip->sp];

                    chip->pc = popped_temp;
                    
                    break;
            }   
            break;
        case 0x1: // only consider 1NNN, simply set PC to NNN
            uint16_t location = instr & 0x0FFF; // Need to acquire NNN

            chip->pc = location;
            break;
        case 0x2: // 2NNN, Set PC = NNN. Push current PC to stack. 

            uint16_t addr = instr & 0x0FFF;

            // First push current PC to stack. 

            chip->stack[chip->sp] = chip->pc;

            chip->sp++;

            chip->pc = addr;

            break;
        case 0x3: // 3XNN, increment PC by 2 if value in VX == NN
            uint8_t reg_num3 = (instr >> 8) & 0x000F;
            uint8_t value3 = instr & 0x00FF;
            if (chip->registers[reg_num3] == value3) {
                chip->pc += 2;
            }

            // otherwise do nun

            break;
        case 0x4: //  4XNN, increment by 2 if value in VX != NN
            uint8_t reg_num4 = (instr >> 8) & 0x000F;
            uint8_t value4 = instr & 0x00FF;
            if (chip->registers[reg_num4] != value4) {
                chip->pc += 2;
            }

            // otherwise do nun

            break;
        case 0x5: // 5XY0, skip if values VX == VY 
            uint8_t x_num5 = (instr >> 8) & 0x000F;
            uint8_t y_num5 = (instr >> 4) & 0x000F;
            if (chip->registers[x_num5] == chip->registers[y_num5]) {
                chip->pc += 2;
            }

            // otherwise do nun
            break;
        case 0x6: // 6XNN, simply set register VX to value NN
            uint8_t set_temp = instr & 0x00FF;
            uint8_t set_reg = (instr & 0x0F00) >> 8; // Need to acquie the decimal value
            
            chip->registers[set_reg] = set_temp; // set the register to the value NN
            break;
        case 0x7: // simply add value NN to VX
            uint16_t add_temp = instr & 0x00FF;

            uint16_t add_reg = (instr & 0x0F00) >> 8; // Need to acquie the decimal value
            
            chip->registers[add_reg] += add_temp; // okay mb, i thought VF would be affected
            
            break;
        case 0x8: // 8XYN
            uint8_t temp8 = instr & 0x000F;
            switch (temp8) {
                case 0x00: // 8XY0 VX is set to the value of VY.
                    uint8_t x_num80 = (instr >> 8) & 0x000F;
                    uint8_t y_num80 = (instr >> 4) & 0x000F;

                    chip->registers[x_num80] = chip->registers[y_num80];

                    break;

                case 0x01: // 8XY1 VX = VX OR VY
                    uint8_t x_num81 = (instr >> 8) & 0x000F;
                    uint8_t y_num81 = (instr >> 4) & 0x000F;

                    chip->registers[x_num81] = chip->registers[y_num81] | chip->registers[x_num81];
                    break;

                case 0x02: // 8XY2 VX = VX AND VY
                    uint8_t x_num82 = (instr >> 8) & 0x000F;
                    uint8_t y_num82 = (instr >> 4) & 0x000F;

                    chip->registers[x_num82] = chip->registers[y_num82] & chip->registers[x_num82];
            
                    break;

                case 0x03: // 8XY3 VX = VX XOR VY
                    uint8_t x_num83 = (instr >> 8) & 0x000F;
                    uint8_t y_num83 = (instr >> 4) & 0x000F;

                    chip->registers[x_num83] = chip->registers[y_num83] ^ chip->registers[x_num83];
            
                    break;

                case 0x04: // 8XY4 VX = VX + VY, set VF = 0/1 depending on overflow/not
                    uint8_t x_num84 = (instr >> 8) & 0x000F;
                    uint8_t y_num84 = (instr >> 4) & 0x000F;


                    // Need to check for overflow 
                    if (chip->registers[x_num84] > (UINT8_MAX - chip->registers[y_num84])) { // overflow
                        chip->registers[15] = 1;
                    } else { // fine
                        chip->registers[15] = 0;
                    }
                    chip->registers[x_num84] = chip->registers[x_num84] + chip->registers[y_num84]; // if overflow, does this happen either way?
            
                    break;
                
                case 0x05: // 8XY5 VX = VX - VY
                        
                    uint8_t x_num85 = (instr >> 8) & 0x000F;
                    uint8_t y_num85 = (instr >> 4) & 0x000F;

                    chip->registers[x_num85] = chip->registers[x_num85] - chip->registers[y_num85];

                    break;

                case 0x06: // 8XY6 Set VX to the value of VY, Shift the value of VX one bit to the right (8XY6), Set VF to 1 if lsb that was shifted out was 1, or 0 if it was 0
                    
                    uint8_t x_num86 = (instr >> 8) & 0x000F;
                    uint8_t result = 0x01 & chip->registers[x_num86];

                    chip->registers[x_num86] = chip->registers[x_num86] >> 1;

                    if (result) {
                        chip->registers[15] = 1;
                    } else {
                        chip->registers[15] = 0;
                    }
                    
                    
                    break;
                
                case 0x07: // 8XY7 VX = VX - VY
                    
                    uint8_t x_num87 = (instr >> 8) & 0x000F;
                    uint8_t y_num87 = (instr >> 4) & 0x000F;

                    chip->registers[x_num87] = chip->registers[y_num87] - chip->registers[x_num87];   
            
                    break;

                case 0x0E: // 8XYE  Set VX to the value of VY, Shift the value of VX one bit to the left (8XYE), Set VF to 1 if msb that was shifted out was 1, or 0 if it was 0
                    
                    uint8_t x_num8E = (instr >> 8) & 0x000F;
                    uint8_t result8E = 0x80 & chip->registers[x_num8E];

                    chip->registers[x_num8E] = chip->registers[x_num8E] << 1;

                    if (result8E) { // Even in this case, if there is successful AND, the value is >1, otherwise null. 
                        chip->registers[15] = 1;
                    } else {
                        chip->registers[15] = 0;
                    }
                    
                    
                    break;

            }
            

            break;
        case 0x9: // 9XY0, skip if values VX != VY
            uint8_t x_num9 = (instr >> 8) & 0x000F;
            uint8_t y_num9 = (instr >> 4) & 0x000F;

            uint8_t x_value9 = chip->registers[x_num9];
            uint8_t y_value9 = chip->registers[y_num9];

            if (x_value9 != y_value9) {
                chip->pc += 2;
            }

            // otherwise do nun

            break;
        case 0xA: // ANNN, set the index register to value NNN
            uint16_t index_temp = instr & 0x0FFF;

            chip->idx = index_temp;
            break;

        case 0xB: // BNNN, update PC = V0 + NNN

            uint16_t jump = 0x0FFF & instr;
            
            chip->pc = jump + chip->registers[0]; // Hopefully it does int promo?

            break;

        case 0xC: // CXNN, VX = rand() & NN

            uint8_t x_numC = (instr & 0x0F00) >> 8;
            uint8_t value = (instr & 0x00FF); // no need to shift.
            
            srand((unsigned int)time(NULL));
            uint8_t random_8bit = rand() & 0xFF;

            chip->registers[x_numC] = random_8bit & value;
        
            break;
        case 0xD: // DXYN This is drawing the display. Need X, Y coords and needs to set other things as well. 
            // If you reach the right edge of the screen, stop drawing this row

            // Stop if you reach the bottom edge of the screen
            
            uint16_t vx = (instr & 0x0F00) >> 8;
            uint16_t vy = (instr & 0x00F0) >> 4;
            uint16_t N = (instr & 0x000F); // Get N


            int xcord = (chip->registers[vx]) % 64; //
            int ycord = (chip->registers[vy]) % 32;
 
            chip->registers[15] = 0; // set VF to zero
            
            int ytemp = ycord;
            for (int i = 0; i < N; i++) { // access each of N rows
                // To access each of the 8 bits in the sprite row, use bitwise operations. 
                // have a inner forloop son im crine
                
                uint8_t n_temp = chip->ram[chip->idx+i]; 
                
                int xtemp = xcord;

                for (int j = 0; j < 8; j++) { // count from most to least significant bit 0000 0000, to be convinient shift left bc it is unsigned
                    // Shift the msb to the right
                    //if (n_temp)  {
                        // need to additionally check if the msb is set.  Additionally check if coord xy on the screen is also on. Then turn of pixel and set VF to 1

                        uint8_t msb = 0x80;
                        if (n_temp & msb) {
                            uint8_t value = chip->display[ytemp * 64 + xtemp];
                            // Apply two cases, if the value at the display is on or off
                            if (value) {
                                // use the 1d formula to access 2d
                                chip->registers[15] = 1;
                                chip->display[ytemp * 64 + xtemp] = 0; // turn off
                                //n_temp = n_temp << 1;
                            } else {
                                chip->display[ytemp * 64 + xtemp] = 1; // turn on 
                                //n_temp = n_temp << 1;
                            }
                        }
                        n_temp = n_temp << 1;
                        xtemp++; // Increment X (VX is not incremented)
                        if (xtemp >= 64) break; // Stop drawing on row. 
                            
                    }
                   
                
                
        
                ytemp++; //Increment Y (VY is not incremented)
                if (ytemp >= 32) break; // stop drawing.
            }



            // ASSUME FOR NOW, lets debug later. 
            break;

        /*
        These skip based on whether the player is currently pressing a key or not.
        */
        case 0xE: 
            uint8_t temp = (instr & 0x000F);

            switch(temp) {

                case 0x01: // EXA1, skips if the key corresponding to the value in VX is not pressed i.e. chip->keypad[key] = 1
                    printf("Checking key: %d value: %d\n", value, chip->keypad[value]);
                    uint8_t reg_num1 = (instr & 0x0F00) >> 8;

                    uint8_t value1 = chip->registers[reg_num1];

                    if (chip->keypad[value1] == 0) {
                        chip->pc += 2;
                    } 

                    // Otherwise do nun
                    break;

                case 0x0E: // EX9E, skip one instruction (increment PC by 2) if the key corresponding to the value in VX is pressed.
                   
                    uint8_t reg_num = (instr & 0x0F00) >> 8;

                    uint8_t value = chip->registers[reg_num];
                    printf("Pong checking keypad[%d]\n", value);
                    if (chip->keypad[value] == 1) {
                        chip->pc += 2;
                    }

                    break;
            }

            break;
        case 0xF:
            uint8_t tempF = (instr & 0x00FF);

            switch (tempF) {
                
                case 0x07: // FX07
                    uint8_t reg_num7 = (instr & 0x0F00) >> 8;

                    chip->registers[reg_num7] = chip->delay_timer;

                    break;

                case 0x0A: // FX0A, a key is pressed while this instruction is waiting for input, its hexadecimal value will be put in VX and execution continues.

                    uint8_t reg_numA = (instr & 0x0F00) >> 8;

                    bool pressed = false;
                    for (int i = 0; i < 16; i ++) {
                        if (chip->keypad[i] == 1) {
                            chip->registers[reg_numA] = i;
                            pressed = true;
                            break;
                        }
                    }
                    if (pressed == false) {
                        chip->pc -= 2;
                    }


                    break;

                case 0x15: // FX15
                    uint8_t reg_num15 = (instr & 0x0F00) >> 8;

                    chip->delay_timer = chip->registers[reg_num15];
                    
                    break;

                case 0x18: // FX18
                    uint8_t reg_num18 = (instr & 0x0F00) >> 8;

                    chip->sound_timer = chip->registers[reg_num18];

                    break;

                case 0x1E: // FX1E

                    uint8_t reg_num1E = (instr & 0x0F00) >> 8;
                    
                    chip->idx += chip->registers[reg_num1E];
                    
                    break;

                case 0x29: // FX29
                    uint8_t reg_num29 = (instr & 0x0F00) >> 8;

                    chip->idx = 0x050 + (chip->registers[reg_num29]) * 5; // SImply add to 0x050 to point to addr of the font. 

                    break;

                case 0x33: // FX33
                    uint8_t reg_num33 = (instr & 0x0F00) >> 8;

                    uint8_t value33 = chip->registers[reg_num33]; // This is a 8 bit number
                    
                    int hundreds = value33 / 100;
                    
                    int tens = (value33 / 10) % 10;

                    int ones = value33 % 10;

                    // now store into respective. 

                    chip->ram[chip->idx] = hundreds;
                    chip->ram[chip->idx + 1] = tens;
                    chip->ram[chip->idx + 2] = ones;
                    
                    break;

                case 0x55: // FX55, this is store memory

                    uint8_t reg_num55 = (instr & 0x0F00) >> 8;

                    for (int i = 0; i <= reg_num55; i++) {
                        chip->ram[chip->idx + i] = chip->registers[i];
                    }
                    
                    break;

                case 0x65: // FX65, this is load memory, simple mistake it should be from V0 TO VX

                    uint8_t reg_num65 = (instr & 0x0F00) >> 8;
                    
                    for (int i = 0; i <= reg_num65; i++) {
                        chip->registers[i] = chip->ram[chip->idx + i];
                    }
                    
                    break;
            
           
            }

            break;
    }   
}
