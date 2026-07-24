# CHIP-8 Emulator/Interpreter
This is a simple-approach chip8-emulator fully built on C. Featuring a scaled 64 x 32 pixel screen that ensures a fully visible window for the user. Due to the small size of RAM (4kb/4096 bytes), it makes this interpreter flexible such that any modern computer should be able to run it.

Combining my knowledge of low level concepts that I learnt in my practical programming class + computer architecture class (plus my summertime boredom), I decided making a chip-8 emulator was a great idea in continuing my low level journey. 

## Features
- 64x32 pixel monochrome display scaled up 15x to 960x480
- XOR sprite rendering with collision detection
- Full 16-key hexadecimal keypad support
- Compatible with any standard Chip-8 ROM
- Maps standard keyboard input to the Chip-8 hexadecimal keypad

## Pre-requsities 
- GCC (MINGW)
- SDL2 (specifically at C:\SDL2\)

## How to Run
- Run `make` to compile and to run the Makefile using default settings
- Run `./chip8 roms/<GAME>` to run any desired game. Feel free to download other games as well!

## Sources:
- [Chip-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8#Further_reading)
- [Chip-8 High Level Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#display)

  
