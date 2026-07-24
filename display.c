#include "chip8.h"
#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

const int scale = 15;
const int SCREEN_WIDTH = 64 * scale;
const int SCREEN_HEIGHT = 32 * scale;

static SDL_Window *window = NULL; // This means file-scoped
static SDL_Renderer *renderer = NULL;

int display_init (void) {


    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) { 
        printf("SDL could not be initialized. SDL_ERROR: %s\n", SDL_GetError()); // the GetError lets me know if any errors happened inside of any SDL function.
        return 1;
    }
    window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
    if (window == NULL) {
        printf("Window could not be created, SDL ERROR: %s\n", SDL_GetError());
        return 1;
    }  
    

    renderer = SDL_CreateRenderer(window, -1, 0);
  
    if (renderer == NULL) {
        printf("Renderer could not be created, SDL ERROR: %s\n", SDL_GetError());
        return 1;
    }
    
    return 0; // This is success.

};

void display_render (Chip8 *chip) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set to black first

        SDL_RenderClear(renderer); // This should clear the screen

       
        SDL_Rect rect;
        rect.w = scale;
        rect.h = scale; // initialize members of struct. 


        int len = sizeof(chip->display) / sizeof(chip->display[0]);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // now white for drawing. 

        for (int i = 0; i < len; i++) {
            int x = i % 64;
            int y = i / 64; // apply modulo based on dim
            if (chip->display[i] == 1) {
                rect.x = x * scale;
                rect.y = y * scale;
                SDL_RenderFillRect(renderer, &rect);
            } else if (chip->display[i] == 0) {
                continue;
            }
        }

        SDL_RenderPresent(renderer);
}; 

int display_handle_input (Chip8 *chip) {
    SDL_Event event;
    
    // Handle the events. 

    while(SDL_PollEvent(&event) != 0) {
        // NESTED SWITCH

        switch (event.type) {

            case SDL_QUIT:
                return 1;
                break;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                
                    case SDLK_1: 
                        chip->keypad[1] = 1;
                        break;
                    case SDLK_2:
                        chip->keypad[2] = 1; 
                        break;
                    case SDLK_3: 
                        chip->keypad[3] = 1;
                        break;
                    case SDLK_4: 
                        chip->keypad[0x0C] = 1;
                        break;
                    case SDLK_q: 
                        chip->keypad[4] = 1;
                        break;
                    case SDLK_w: 
                        chip->keypad[5] = 1;
                        break;
                    case SDLK_e:
                        chip->keypad[6] = 1;
                        break;
                    case SDLK_r: 
                        chip->keypad[0x0D] = 1;
                        break;
                    case SDLK_a:
                        chip->keypad[7] = 1;
                        break;
                    case SDLK_s:
                        chip->keypad[8] = 1;
                        break; 
                    case SDLK_d: 
                        chip->keypad[9] = 1;
                        break;
                    case SDLK_f: 
                        chip->keypad[0x0E] = 1;
                        break;
                    case SDLK_z: 
                        chip->keypad[0x0A] = 1;
                        break;
                    case SDLK_x: 
                        chip->keypad[0] = 1;
                        break;
                    case SDLK_c: 
                        chip->keypad[0x0B] = 1;
                        break;
                    
                    case SDLK_v: 
                        chip->keypad[0x0F] = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                
                    case SDLK_1: 
                        chip->keypad[1] = 0;
                        break;
                    case SDLK_2:
                        chip->keypad[2] = 0; 
                        break;
                    case SDLK_3: 
                        chip->keypad[3] = 0;
                        break;
                    case SDLK_4: 
                        chip->keypad[0x0C] = 0;
                        break;
                    case SDLK_q: 
                        chip->keypad[4] = 0;
                        break;
                    case SDLK_w: 
                        chip->keypad[5] = 0;
                        break;
                    case SDLK_e:
                        chip->keypad[6] = 0;
                        break;
                    case SDLK_r: 
                        chip->keypad[0x0D] = 0;
                        break;
                    case SDLK_a:
                        chip->keypad[7] = 0;
                        break;
                    case SDLK_s:
                        chip->keypad[8] = 0;
                        break; 
                    case SDLK_d: 
                        chip->keypad[9] = 0;
                        break;
                    case SDLK_f: 
                        chip->keypad[0x0E] = 0;
                        break;
                    case SDLK_z: 
                        chip->keypad[0x0A] = 0;
                        break;
                    case SDLK_x: 
                        chip->keypad[0] = 0;
                        break;
                    case SDLK_c: 
                        chip->keypad[0x0B] = 0;
                        break;
                    
                    case SDLK_v: 
                        chip->keypad[0x0F] = 0;
                        break;
                }

                break;
        }
    }

    return 0; // if no quit event. 
};  

void display_clean (void) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    // Nice and simple, simply exit and clean the display. 
};


