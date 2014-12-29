/*
*	main.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/

#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "tetris.h"
#include "guicntls.h"
#include "multi.h"


int
main()
{
	int c, seg, wide;

        m_initialize();
	g_initialize();

	do {
		c = gui_option("(S)tandard tetris or (M)odification?", "sSmM");
		if (c == 's' || c == 'S') {
			seg = 4;
			wide = 10;
		} else {
			c = gui_option("Brick size? (1..9)", "123456789");
			seg = c - '0';
			c = gui_option("(S)tandard grid or (D)ouble wide?", "sSdD");
			if (c == 's' || c == 'S') {
				wide = 10;
			} else {
				wide = 20;
			}
		}
		g_draw_mesh(20, wide);
		gui_message("Press any key to start ...");
		t_run(seg, 50);
		gui_message("GAME OVER");
		c = gui_option("(N)ew Game or (Q)uit", "nNqQ");
	} while ((c == 'n') || (c == 'N'));

	g_close();
	return 0;
}


