all:
	clear
	rm -rf build/
	mkdir build/
	ccache gcc -c src/main.c -o build/main.o
	ccache gcc -c src/input.c -o build/input.o
	ccache gcc -c src/draw.c -o build/draw.o
	ccache gcc -c src/map.c -o build/map.o
	ccache gcc -c src/object.c -o build/object.o
	ccache gcc -c src/player.c -o build/player.o
	ccache gcc build/*.o -lm -lSDL2 -lSDL2_image
	./a.out

windows:
	clear
	rm -rf build/
	mkdir build/
	ccache x86_64-w64-mingw32-gcc -c src/main.c -o build/main.o
	ccache x86_64-w64-mingw32-gcc -c src/input.c -o build/input.o
	ccache x86_64-w64-mingw32-gcc -c src/draw.c -o build/draw.o
	ccache x86_64-w64-mingw32-gcc -c src/map.c -o build/map.o
	ccache x86_64-w64-mingw32-gcc -c src/object.c -o build/object.o
	ccache x86_64-w64-mingw32-gcc -c src/player.c -o build/player.o
	ccache x86_64-w64-mingw32-gcc build/*.o -o GAME.EXE -static-libgcc -Wl,-Bstatic -lpthread -lcomctl32 -lpsapi -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -Wl,-Bdynamic