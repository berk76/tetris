/*
*       main_tc.c
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 15.7.2016
*/


#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <dos.h>
#include <time.h>
#include "gui_tc.h"
#include "tetris.h"
#include "main.h"


static int graph_driver;
static int graph_mode;
static int error_code;
static int MESH_COLOR = 8;
static int MESH_BK_COLOR = 7;
static TETRIS_T tetris;
static unsigned _delay;
static int color_vec[] = {LIGHTBLUE,
                          LIGHTGREEN,
                          LIGHTCYAN,
                          LIGHTRED,
                          LIGHTMAGENTA,
                          YELLOW,
                          WHITE,
                          BLACK,
                          BLUE};

static int init_graph(void);
static void close_graph(void);
static void g_draw_mesh(int grid_size);
static void g_print_controls();
static void g_update_score();
static int g_printf(int *xloc, int *yloc, char *fmt, ...);
static void process_user_input();
static void wait(int ms);


int main() {
	int c, seg, wide, ret;
        GAME_T game;

        init_graph();
        srand(time(NULL) % 37);

	do {
                game = TETRIS;
		c = gui_option("(A)ddtrix or (T)etris?", "tTaA");
                if (c == 'a' || c == 'A') {
                        game = ADDTRIS;
                        seg = 1;
			wide = 10;
			_delay = 170;
                } else {
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
			_delay = 90;
                }
                t_create_game(&tetris, game, wide, 20, seg);

		g_draw_mesh(15);
		gui_message("Press any key to start ...");

                do {
                        int i;
                        for (i = 0; i < 5; i++) {
		              process_user_input();
			      wait(_delay);
                        }
                        ret = t_go(&tetris);
                        g_update_score();
                } while (ret != -1);

                t_delete_game(&tetris);
		gui_message("GAME OVER");
		c = gui_option("(N)ew Game or (Q)uit", "nNqQ");
	} while ((c == 'n') || (c == 'N'));

	close_graph();
	return 0;
}


int init_graph(void) {
	graph_driver = DETECT;
	initgraph(&graph_driver, &graph_mode, "");
	error_code = graphresult();
	if (error_code != grOk) {
		printf("Graphics system error: %s\n", grapherrormsg(error_code));
		exit(1);
	}
}


void close_graph(void) {
	closegraph();
}


void g_draw_mesh(int grid_size) {
        int x, y;

	cleardevice();

        tetris.element_size = grid_size;
        tetris.origin_x = getmaxx()/2 - tetris.grid_size_x/2 * grid_size;
        tetris.origin_y = getmaxy()/2 - tetris.grid_size_y/2 * grid_size;

	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	setcolor(WHITE);
        x = tetris.origin_x;
        y = tetris.origin_y - 32;
        g_printf(&x, &y, "Tetris %s", TETRIS_VERSION);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

	setcolor(color_vec[MESH_COLOR]);
	setbkcolor(color_vec[MESH_BK_COLOR]);

	rectangle(tetris.origin_x - 1,
		tetris.origin_y - 1,
		tetris.origin_x + tetris.element_size * tetris.grid_size_x + 1,
		tetris.origin_y + tetris.element_size * tetris.grid_size_y + 1);

	g_print_controls();
	g_update_score();
}


void g_print_controls() {
	int x, y;

	x = tetris.origin_x - 150;
	y = tetris.origin_y + 4 * 8;
	setcolor(LIGHTGRAY);
	g_printf(&x, &y, "Controls:");
	g_printf(&x, &y, " ");
	g_printf(&x, &y, "Left   ... 7");
	g_printf(&x, &y, "Right  ... 9");
	g_printf(&x, &y, "Rotate ... 8");
	g_printf(&x, &y, "Drop   ... 4");
	g_printf(&x, &y, " ");
	g_printf(&x, &y, "Pause  ... P");
	g_printf(&x, &y, "Quit   ... Q");
}


void g_update_score() {
	int x, y;
	static int last = -1;

	if (last != tetris.score) {
		x = tetris.origin_x - 150;
		y = tetris.origin_y + 8;
		setcolor(BLACK);
		gui_fill_rect(x, y, 100, 8, BLACK, SOLID_FILL);
		setcolor(WHITE);
		g_printf(&x, &y, "Score: %d", tetris.score);
		last = tetris.score;
	}
}


int g_printf(int *xloc, int *yloc, char *fmt, ...) {
	va_list argptr;
	char str[140];
	int cnt;

	va_start(argptr, format);
	cnt = vsprintf(str, fmt, argptr);
	outtextxy(*xloc, *yloc, str);
	*yloc += textheight("H") + 2;
	va_end(argptr);

	return cnt;
}


void process_user_input() {
	while (kbhit()) {

		int c;
		c = getch();

		switch (c) {
			case '7':
				t_move_left(&tetris);
				break;
			case '8':
				t_rotate(&tetris, 1);
				break;
			case '9':
				t_move_right(&tetris);
				break;
			case '4':
				while(t_move_down(&tetris) != -1)
					;
				break;
			case 'p':
				gui_message("Paused");
				break;
			case 'q':
				if (gui_confirm("Do you want to quit game? (Y/N)") == G_TRUE)
					exit(0);
		}
	}
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
	int tx, ty;

	if (tetris->game == TETRIS) {
		setcolor(color_vec[MESH_COLOR]);
        	gui_fill_rect(
                        tetris->origin_x + x * tetris->element_size,
                        tetris->origin_y + y * tetris->element_size,
        		tetris->element_size,
                        tetris->element_size,
                        color_vec[color], SOLID_FILL);
	} else {
		m_empty_mesh_pixel(tetris, x, y);
		setcolor(WHITE);
		tx = tetris->origin_x + x * tetris->element_size + 4;
		ty = tetris->origin_y + y * tetris->element_size + 4;
		g_printf(&tx, &ty, "%d", tetris->brick.value);
        }
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        setcolor(color_vec[MESH_BK_COLOR]);
	gui_fill_rect(
                tetris->origin_x + x * tetris->element_size,
                tetris->origin_y + y * tetris->element_size,
		tetris->element_size,
                tetris->element_size + 1,
                color_vec[MESH_BK_COLOR], SOLID_FILL);
}


void m_line_destroyed() {
}


void wait(int ms) {
	clock_t endwait;

	endwait = clock() + ms * CLK_TCK / 1000;
	while (clock() < endwait)
		;
}

