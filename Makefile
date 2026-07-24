# Ensure convenience for the user. 

all: 
	gcc main.c chip8.c display.c -o chip8 -I"C:\SDL2\x86_64-w64-mingw32\include" -L"C:\SDL2\x86_64-w64-mingw32\lib" -lmingw32 -lSDL2main -lSDL2

clean:
	del chip8.exe