# CHIP-8 Emulator/Interpreter
This is a simple-approach chip8-emulator fully built on C. Featuring a scaled 64 x 32 pixel screen that ensures a fully visible window for the user. Due to the small size of RAM (4kb/4096 bytes), it makes this interpreter flexible such that any modern computer should be able to run it.

Combining my knowledge of low level concepts that I learnt in my practical programming class + computer architecture class (plus my summertime boredom), I decided making a chip-8 emulator was a great idea in continuing my low level journey. 

## Images
<img width="800" height="400" alt="Screenshot 2026-07-24 144517" src="https://github.com/user-attachments/assets/f9450158-d99f-4f61-ac49-87f3cf054909" />
<img width="1100" height="500" alt="Screenshot 2026-07-24 144445" src="https://github.com/user-attachments/assets/dd056ad4-9def-49d2-9161-1fb83ccaec08" />
<img width="950"  height="450" alt="Screenshot 2026-07-24 143705" src="https://github.com/user-attachments/assets/9b5a1dbc-1f64-476e-85ca-7af4c27d45cf" />

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

## Future Improvements 
- Make own chip-8 game
- Make Add SUPER-CHIP support
- Adding Color
- Emulator to run faster

## Sources
- [Chip-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8#Further_reading)
- [Chip-8 High Level Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#display)
- [More Chip-8 Games](github.com/kripod/chip8-roms)

  
