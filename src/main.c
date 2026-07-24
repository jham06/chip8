#include <stdio.h>
#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    
    Chip8 chip;

    if (argc < 2) {
        printf("Usage: ./chip8 <rom>\n");
        return 1;
    }  
    chip8_init(&chip); // initialize the window, load the binary file. 
    chip8_load(&chip, argv[1]);
    display_init();

    int retVal;

    int cycle_count = 0;
    while (67) { // DUring infinite loop, get the input and apply as is. 
        retVal = display_handle_input(&chip);

        if (retVal == 1) {
            break;
        }
        chip8_cycle(&chip);
        display_render(&chip);
        SDL_Delay(2);

        cycle_count++; // CREDIT: Claude.ai. THis allows me to run this emulator run at the correct speed. NOt the modern cpu speed. 
        if (cycle_count % 8 == 0) {
            if (chip.delay_timer > 0) chip.delay_timer--;
            if (chip.sound_timer > 0) chip.sound_timer--;
        }

     
    }

    display_clean();

    return 0; // simple loop, just calling in order...
}