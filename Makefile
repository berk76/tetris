# TETRIS (C) 2014 Jaroslav Beran

CFLAGS = +zx -c -o $@
CC = zcc
objects = main.o tetris.o graph.o shape.o guicntls.o multi.o

tetris: $(objects)
	$(CC) +zx -lndos -lmalloc -lx11 -o$@ -create-app $(objects)

main.o: main.c graph.h tetris.h guicntls.h multi.h

tetris.o: tetris.c tetris.h graph.h shape.h multi.h guicntls.h

graph.o: graph.c graph.h multi.h

shape.o: shape.c shape.h graph.h

guicntls.o: guicntls.c guicntls.h multi.h

multi.o: multi.c multi.h mu_tc.c mu_z88.c

clean:
	rm -f $(objects) *.bak tetris *.exe *.tap zcc_opt.def

