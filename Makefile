
CC = gcc
CFLAGS = -D WINVER=0x0603 -D _WIN32_WINNT=0x0603 -O2 -Wall -c -o $@


all: Tetris

Tetris: src/main.o src/tetris.o src/shape.o src/graph.o src/resource.res
	$(CC) -Wall -o $@ src/main.o src/tetris.o src/shape.o src/graph.o src/resource.res -lgdi32 -mwindows


src/main.o: src/main.c src/tetris.h src/graph.h src/resource.h

src/tetris.o: src/tetris.h src/shape.h src/graph.h

src/shape.o: src/shape.h

src/graph.o: src/graph.h src/tetris.h

src/resource.res: src/resource.rc src/resource.h
	windres src/resource.rc -O coff -o src/resource.res

clean:
	rm -f src/*.o *.exe src/*.res 

