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


long heap;
static unsigned char heap_space[1000];
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
                          
#define BRICK_OFFSET 10
char brick[] = {8,8,0x7e,0x81,0x81,0x89,0x99,0x81,0x81,0x7e,
                8,8,0x7e,0xab,0xd5,0xab,0xd5,0xab,0xd5,0x7e,
                8,8,0x7e,0xff,0xff,0xf7,0xe7,0xff,0xff,0x7e};
char blank[] = {8,8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

static void g_draw_menu();
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
        GAME_T game;

        mallinit();
        sbrk(heap_space, sizeof(heap_space));
        initgraph(0,0,0);
        srand(time(NULL));

	do {
                g_draw_menu();
                c = gui_option("CHOOSE YOUR OPTION...", "123");
                
                switch (c) {
                        case '1' :
                                game = ADDTRIS;
                                seg = 1;
                                wide = 10;
                                _delay = 140;
                                break;
                        case '2' :
                                game = TETRIS;
                                seg = 4;
        			wide = 10;
                                _delay = 75;
                                break
                        case '3' :
                                clg();
                                game = TETRIS;
        			c = gui_option("BRICK SIZE? (1..9)", "123456789");
        			seg = c - '0';
        			c = gui_option("(S)TANDARD GRID OR (D)OUBLE WIDE?", "sSdD");
        			if (c == 's' || c == 'S') {
        				wide = 10;
        			} else {
        				wide = 20;
        			}
                                _delay = 75;
                }
                
                t_create_game(&tetris, game, wide, 20, seg);
                
		g_draw_mesh(8);
		gui_message("PRESS ANY KEY TO START ...");
                
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
		c = gui_option("(N)EW GAME OR (Q)UIT", "nNqQ");
	} while ((c == 'n') || (c == 'N'));
        
        closegraph();

	return 0;
}


void g_draw_menu() {
        clg();
        
        setusercharsize(2,1,1,1);
	outtextxy(80, 30, "Tetris");
        setusercharsize(1,1,1,1);
        outtextxy(80, 70, "1) ADDTRIS");
        outtextxy(80, 80, "2) TETRIS");
        outtextxy(80, 90, "3) CRAZY MODIFICATION");
}


void g_draw_mesh(int grid_size) {
        clg();

	tetris.element_size = grid_size;
        tetris.origin_x = getmaxx()/2 - tetris.grid_size_x/2 * grid_size + 40;
        tetris.origin_y = getmaxy()/2 - tetris.grid_size_y/2 * grid_size;

        setusercharsize(2,1,1,1);
	outtextxy(0, tetris.origin_y, "Tetris");
	setusercharsize(1,1,1,1);
        outtextxy(37, tetris.origin_y + 4, TETRIS_VERSION);
        
	rectangle(tetris.origin_x - 1,
		tetris.origin_y - 1,
		tetris.origin_x + tetris.element_size * tetris.grid_size_x + 1,
		tetris.origin_y + tetris.element_size * tetris.grid_size_y + 1);
        
	g_print_controls();
	g_update_score();
}


void g_print_controls() {
        int x, y;

	x = 1;
	y = tetris.origin_y + 5 * 8;
	setcolor(LIGHTGRAY);
	outtextxy(x, y, "CONTROLS:");
	y += 10;
	outtextxy(x, y, " ");
	y += 10;
	outtextxy(x, y, "LEFT   ... 7");
	y += 10;
	outtextxy(x, y, "RIGHT  ... 9");
	y += 10;
	outtextxy(x, y, "ROTATE ... 8");
	y += 10;
	outtextxy(x, y, "DROP   ... 4");
	y += 10;
	outtextxy(x, y, " ");
	y += 10;
	outtextxy(x, y, "PAUSE  ... P");
	y += 10;
	outtextxy(x, y, "QUIT   ... Q");
}


void g_update_score() {
        static int last_score = -1;
	int x, y;
	char str[100];
        
        if (last_score == tetris.score)
                return;
                
        last_score = tetris.score;

	x = 1;
	y = tetris.origin_y + 3 * 8;

	gui_unfill_rect(x + 60, y, 20, 10);
	itoa(tetris.score, str, 10);
	outtextxy(x, y, "SCORE: ");
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
				gui_message("PAUSED");
				break;
			case 'q':
				if (gui_confirm("DO YOU WANT QUIT GAME? (Y/N)") == G_TRUE)
					exit(0);
		}
	}
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        char str[5];
        
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;
        if (tetris->game == TETRIS) {
                color %=  3; 
                putsprite(spr_or, x, y, brick + BRICK_OFFSET * color);
        } else {
                putsprite(spr_and, x, y, blank);
                itoa(tetris->brick.value, str, 10);
                outtextxy(x + 2, y + 1, str);
        }
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;
        putsprite(spr_and, x, y, blank);
}

/*** GUI ***/

static void gui_draw_message(char *msg);
static void gui_delete_message();
static int gui_wait_for_key(char *s);
static int gui_wait_for_any_key();


void gui_message(char *msg) {
	gui_draw_message(msg);
	gui_wait_for_any_key();
	gui_delete_message();
}


int gui_confirm(char *msg) {
	int c;

	gui_draw_message(msg);
	c = gui_wait_for_key("yYnN");
	gui_delete_message();

	return ((c == 'y') || (c == 'Y')) ? G_TRUE : G_FALSE;
}


int gui_option(char *msg, char *options) {
	int c;

	gui_draw_message(msg);
	c = gui_wait_for_key(options);
	gui_delete_message();

	return c;
}


void gui_draw_message(char *msg) {	
        outtextxy(1, 182, msg);
}


void gui_delete_message() {
	gui_unfill_rect(1, 180, 200, 10);
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
