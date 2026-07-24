#ifndef DISPLAY8_H
#define DISPLAY8_H

#include <SDL2/SDL.h>
#include "chip8.h"

int display_init (void); // Initialize sdl2 window + renderer
void display_render (Chip8 *chip); 
int display_handle_input (Chip8 *chip) ; // simply handle display tasks, nothng is returned. 
void display_clean (void) ;

#endif