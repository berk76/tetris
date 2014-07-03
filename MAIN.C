/*
*	main.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/

#include <stdio.h>
#include <graphics.h>
#include "graph.h"
#include "tetris.h"


int
main()
{
	int x, y;

	g_initialize();
	g_draw_mesh(20, 10, 15, BLACK);

	t_run();

	x = 100;
	y = 100;
	g_printf(&x, &y, "GAME OVER");
	getchar();

	g_close();
	return 0;
}

