#       Makefile
#       
#       This file is part of Tetris game.
#       https://bitbucket.org/berk76/tetris
#       
#       Tetris is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version. <http://www.gnu.org/licenses/>
#       
#       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 28.6.2014


CC = gcc
CFLAGS = -D WINVER=0x0603 -D _WIN32_WINNT=0x0603 -O2 -Wall -c -o $@


all: Tetris

Tetris: src/main_w32.o src/tetris.o src/shape.o src/resource.res
	$(CC) -Wall -o $@ src/main_w32.o src/tetris.o src/shape.o src/resource.res -lgdi32 -lcomctl32 -mwindows


src/main_w32.o: src/main_w32.c src/tetris.h src/resource.h

src/tetris.o: src/tetris.h src/shape.h

src/shape.o: src/shape.h

src/resource.res: src/resource.rc src/resource.h
	windres src/resource.rc -O coff -o src/resource.res

clean:
	rm -f src/*.o *.exe src/*.res 

