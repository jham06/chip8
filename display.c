#include "chip8.h"
#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

const int scale = 10;
const int SCREEN_WIDTH = 64 * scale;
const int SCREEN_HEIGHT = 32 * scale;

int display_init (void) {

};

void display_render (Chip8 *chip) {

}; 

int display_handle_input (Chip8 *chip) {

};  

void display_clean (void) {

};


/*
    Code adapted from: Lazy Foo' Productions, Hello SDL: Your First Graphics Window
    Author: Lazy foo
    URL: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php
*/

int main(int argc, char* args[]) { // Use an int followed by char* array as any other type of main function will cause undefined reference to main. 

        SDL_Window *window = NULL; // Window I'll be rendering to

        SDL_Surface *screenSurface = NULL; // SUrface contained by the window

        // Init SDL, when error occurs SDL_Init returns -1
        if (SDL_Init(SDL_INIT_VIDEO) < 0 ) { 
            printf("SDL could not be initialized. SDL_ERROR: %s\n", SDL_GetError()); // the GetError lets me know if any errors happened inside of any SDL function. 
        } else {
            // Need to create window. 
            window = SDL_CreateWindow("Creating a Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
            if (window == NULL) {
                printf("Window could not be created, SDL ERROR: %s\n", SDL_GetError());
            } else {
                
                // Get Window surface
                screenSurface = SDL_GetWindowSurface(window);

                // FIll the surface White, but in our case should do somehting diff. 
                SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
                //Update the surface
                SDL_UpdateWindowSurface(window);

                 //Hack to get window to stay up
                SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
            }
        }

        // OKay done with tutorial, now implement actual. 

        // Now, destroy the window and quit SDL subsystems. 
        SDL_DestroyWindow(window);

        SDL_Quit();

        return 0;


}