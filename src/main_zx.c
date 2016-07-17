/*
*       main_zx.c
*       
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
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
#include <time.h>
#include <conio.h>
#include <bgi.h>
#include <malloc.h>
#include "tetris.h"
#include "main.h"


#define G_FALSE 0
#define G_TRUE  1


extern long heap(60000);
static int MESH_COLOR = 8;
static int MESH_BK_COLOR = 7;
static TETRIS_T tetris;
static int _delay;
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
static void process_user_input();

/* gui functions */
static void gui_message(char *msg);
static int gui_confirm(char *msg);
static int gui_option(char *msg, char *options);
static void gui_fill_rect(int x, int y, int width, int height);
static void gui_unfill_rect(int x, int y, int width, int height);

int main() {
	int c, seg, wide, ret;

        mallinit();              /* heap cleared to empty */
        sbrk(30000,2000);        /* add 2000 bytes from addresses 30000-31999 inclusive to the heap */
        sbrk(65000,536);         /* add 536 bytes from addresses 65000-65535 inclusive to the heap  */
        
        _delay = 75;
        init_graph();

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
                
                t_create_game(&tetris, wide, 20, seg);
                
		g_draw_mesh(8);
		gui_message("Press any key to start ...");
                
                do {
                        int i;
                        for (i = 0; i < 5; i++) {
		              process_user_input();
		              delay(_delay);
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
        initgraph(0,0,0);
	return 0;
}


void close_graph(void) {
	closegraph();
}


void g_draw_mesh(int grid_size) {
        cleardevice();

	tetris.element_size = grid_size;
        tetris.origin_x = getmaxx()/2 - tetris.grid_size_x/2 * grid_size + 40;
        tetris.origin_y = getmaxy()/2 - tetris.grid_size_y/2 * grid_size;

	setcolor(WHITE);
        setusercharsize(2,1,1,1);
	outtextxy(0, tetris.origin_y, "Tetris");
	setusercharsize(1,1,1,1);
        outtextxy(37, tetris.origin_y + 4, TETRIS_VERSION);

	setcolor(color_vec[MESH_COLOR]);
	setbkcolor(color_vec[MESH_BK_COLOR]);

	rectangle(tetris.origin_x - 1,
		tetris.origin_y - 1,
		tetris.origin_x + tetris.element_size * tetris.grid_size_x + 1,
		tetris.origin_y + tetris.element_size * tetris.grid_size_y + 1);

	g_print_controls();
	g_update_score(0);
}


void g_print_controls() {
	int x, y;

	x = 1;
	y = tetris.origin_y + 5 * 8;
	setcolor(LIGHTGRAY);
	outtextxy(x, y, "Controls:");
	y += 10;
	outtextxy(x, y, " ");
	y += 10;
	outtextxy(x, y, "Left   ... 7");
	y += 10;
	outtextxy(x, y, "Right  ... 9");
	y += 10;
	outtextxy(x, y, "Rotate ... 8");
	y += 10;
	outtextxy(x, y, "Drop   ... 4");
	y += 10;
	outtextxy(x, y, " ");
	y += 10;
	outtextxy(x, y, "Pause  ... P");
	y += 10;
	outtextxy(x, y, "Quit   ... Q");
}


void g_update_score() {
	int x, y;
	char str[100];

	x = 1;
	y = tetris.origin_y + 3 * 8;

	gui_unfill_rect(x + 60, y, 20, 10);
	itoa(tetris.score, str, 10);
	outtextxy(x, y, "Score: ");
	outtextxy(x + 60, y, str);
}


void process_user_input() {
        int c;
        
	while ((c = getk()) != 0) {

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
	int i, j;

	setcolor(color_vec[color]);

	x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;

	for (i = x + 1; i < x + tetris->element_size - 1; i++) 
		plot(i, y);
	
	for (j = y + 1; j < y + tetris->element_size - 1; j++) {
		plot(x, j);
		plot(x + tetris->element_size - 1, j);
	}

	for (i = x + 1; i < x + tetris->element_size - 1; i++) 
		plot(i, y + tetris->element_size - 1);

}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
	int i, j;

	setcolor(color_vec[MESH_BK_COLOR]);

	x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;

	for (i = x + 1; i < x + tetris->element_size - 1; i++)
		unplot(i, y);

	for (j = y + 1; j < y + tetris->element_size - 1; j++) {
		unplot(x, j);
		unplot(x + tetris->element_size - 1, j);
	}

	for (i = x + 1; i < x + tetris->element_size - 1; i++)
		unplot(i, y + tetris->element_size - 1);
}


/*** GUI ***/

typedef struct {
	int x;
	int y;
	int width;
	int height;
	void *buff;
} GUI_CNTL;


static GUI_CNTL *gui_draw_message(char *msg);
static void gui_delete_message(GUI_CNTL *cntl);
static int gui_wait_for_key(char *s);
static int gui_wait_for_any_key();


void gui_message(char *msg) {
	GUI_CNTL *cntl;

	cntl = gui_draw_message(msg);
	gui_wait_for_any_key();
	gui_delete_message(cntl);
}


int gui_confirm(char *msg) {
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key("yYnN");
	gui_delete_message(cntl);

	return ((c == 'y') || (c == 'Y')) ? G_TRUE : G_FALSE;
}


int gui_option(char *msg, char *options) {
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key(options);
	gui_delete_message(cntl);

	return c;
}


GUI_CNTL *gui_draw_message(char *msg) {
	GUI_CNTL *result;
	result = (GUI_CNTL *) malloc(sizeof(GUI_CNTL));

	result->x = 1;
	result->y = 180;
	result->width = 200;
	result->height = 10;
	
	outtextxy(result->x, result->y, msg);

	return result;
}


void gui_delete_message(GUI_CNTL *cntl) {
	gui_unfill_rect(cntl->x, cntl->y, cntl->width, cntl->height);
	free((void *) cntl);
}


int gui_wait_for_key(char *s) {
	int c;

	while (1) {
		while ((c = getk()) != 0) {
			if (strchr(s, c) != NULL)
				return c;
		}
		delay(100);
	}
}


int gui_wait_for_any_key() {
	while (getk()) {
	}

	while (!getk()) {
		delay(100);
	}
}


void gui_fill_rect(int x, int y, int width, int height) {
	int i, j;

	for (i = x; i < x + width; i++)
		for (j = y; j < y + height; j++)
			plot(i, j);
}

void gui_unfill_rect(int x, int y, int width, int height) {
	int i, j;

	for (i = x; i < x + width; i++)
		for (j = y; j < y + height; j++)
			unplot(i, j);
}
