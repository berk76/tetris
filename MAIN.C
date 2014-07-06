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

static int wait_for_key(char *s);
static int wait_for_any_key();

int
main()
{
	int x, y, c;

	g_initialize();

	do {
		g_draw_mesh(20, 10, 15, BLACK);
		t_run();

		g_print_message("GAME OVER");
		wait_for_any_key();

		g_print_message("(N)ew Game, (Q)uit");
		c = wait_for_key("nNqQ");

	} while ((c == 'n') || (c == 'N'));

	g_close();
	return 0;
}

int
wait_for_key(char *s)
{
	int c;

	while (1) {
		while (kbhit()) {
			c = getch();
			if (strchr(s, c) != NULL)
				return c;
		}
		delay(100);
	}
}

int
wait_for_any_key()
{
	while (kbhit()) {
		getch();
	}

	while (!kbhit()) {
		delay(100);
	}
	getch();
}

