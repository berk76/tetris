/*
*       main_81.c
*       
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 31.7.2016        
*/

//#pragma output STACKPTR=49151
//#pragma output hrgpage = 36096

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <malloc.h>
#include "tetris.h"
#include "main.h"


#define G_FALSE 0
#define G_TRUE  1

long heap;
static unsigned char heap_space[1000];
static int MESH_BK_COLOR = 7;
static TETRIS_T tetris;
static int _delay;
static int color_vec[] = {44,42,46,41,45,43,47,40,47};


static void g_draw_mesh(int grid_size);
static void g_print_controls();
static void g_update_score();
static void process_user_input();

/* gui functions */
static void gui_message(char *msg);
static int gui_confirm(char *msg);
static int gui_option(char *msg, char *options);
static void gui_gotoxy(int x, int y);
static void gui_flush();
static void gui_cls();


int main() {
	int c, seg, wide, ret;

        mallinit();
        sbrk(heap_space, sizeof(heap_space));
	gui_cls();
        srand(time(NULL));
        _delay = 75;

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
                
		g_draw_mesh(1);
		gui_message("Press any key to start ...");
                
                do {
                        int i;
                        for (i = 0; i < 5; i++) {
		              process_user_input();
		              delay(_delay);
                              gui_flush();
                        }
                        ret = t_go(&tetris);
                        g_update_score();
                        gui_flush();
                } while (ret != -1);
                
                t_delete_game(&tetris);
		gui_message("GAME OVER");
		c = gui_option("(N)ew Game or (Q)uit", "nNqQ");
	} while ((c == 'n') || (c == 'N'));
        
	return 0;
}


void g_draw_mesh(int grid_size) {
        int i;

	gui_cls();
	tetris.element_size = grid_size;
        tetris.origin_x = 19;
        tetris.origin_y = 1;

	gui_gotoxy(1,1);
	printf("Tetris %s", TETRIS_VERSION);
        
	printf("%c[%um", 27, 33);
	for (i = 0; i < tetris.element_size * tetris.grid_size_y; i++) {
		gui_gotoxy(tetris.origin_x - 1, i + 1);
		printf("*");
		gui_gotoxy(tetris.origin_x + tetris.element_size * tetris.grid_size_x, i + 1);
		printf("*");
	}

	for (i = 0; i < tetris.element_size * tetris.grid_size_x; i++) {
		gui_gotoxy( tetris.origin_x + i, tetris.element_size * tetris.grid_size_y + 1);
		printf("*");
	}
	printf("%c[%um", 27, 37);
        
	g_print_controls();
	g_update_score();
        gui_flush();
}


void g_print_controls() {

	gui_gotoxy(1,5);
	printf("Controls:");

	gui_gotoxy(1,7);
	printf("Left   ... 7");
	gui_gotoxy(1,8);
	printf("Right  ... 9");
	gui_gotoxy(1,9);
	printf("Rotate ... 8");
	gui_gotoxy(1,10);
	printf("Drop   ... 4");

	gui_gotoxy(1,12);
	printf("Pause  ... P");
	gui_gotoxy(1,13);
	printf("Quit   ... Q");
}


void g_update_score() {
        static int last_score = -1;
        
        if (last_score == tetris.score)
                return;
                
        last_score = tetris.score;
	gui_gotoxy(1,3);
	printf("Score: %4d", tetris.score);
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
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size; 
	gui_gotoxy(x, y);
	printf("%c[%um", 27, color_vec[color]);
        printf("O");
	printf("%c[%um", 27, color_vec[MESH_BK_COLOR]);
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;
	gui_gotoxy(x,y);
        printf(" ");
}


/*** GUI ***/

static void gui_draw_message(char *msg);
static void gui_delete_message();
static int gui_wait_for_key(char *s);
static void gui_wait_for_any_key();



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
	gui_gotoxy(1,23);
        printf("%s", msg);
        gui_flush();
}


void gui_delete_message() {
	gui_gotoxy(1,23);
	printf("                                                  ");
        gui_flush();
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


void gui_wait_for_any_key() {
	while (getk()) {
	}

	while (!getk()) {
		delay(100);
	}
}


void gui_gotoxy(int x, int y) {
        printf("%c[%u;%uH",27,y,x);
}


void gui_flush() {
	gui_gotoxy(1,22);
        printf("\n");
}


void gui_cls() {
        /* Set default text attributes (white on black) */
        printf("%c[m",27);

        /* Clear screen in non-ANSI mode*/
        printf("%c",12);
}

