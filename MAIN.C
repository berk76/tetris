/*
*	main.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include "graph.h"
#include "tetris.h"
#include "guicntls.h"



int
main()
{
	int c;

	g_initialize();

	do {
		g_draw_mesh(20, 10, 15, BLACK);
		gui_message("Press any key to start ...");
		t_run(4, 50);
		gui_message("GAME OVER");
		c = gui_option("(N)ew Game, (Q)uit", "nNqQ");
	} while ((c == 'n') || (c == 'N'));

	g_close();
	return 0;
}



