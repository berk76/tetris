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
	int x, y, c;
	GUI_CNTL *cntl;

	g_initialize();

	do {
		g_draw_mesh(20, 10, 15, BLACK);
		t_run();

		cntl = gui_show_message("GAME OVER");
		gui_wait_for_any_key();
		gui_delete_message(cntl);

		cntl = gui_show_message("(N)ew Game, (Q)uit");
		c = gui_wait_for_key("nNqQ");
		gui_delete_message(cntl);

	} while ((c == 'n') || (c == 'N'));

	g_close();
	return 0;
}



