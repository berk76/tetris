/*
*       main_81.c
*       
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 31.7.2016        
*/


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

#ifdef USE_SPRITES
#define SPRITE_OFFSET 10
char brick[] = {8,8,0x7e,0x81,0x81,0x89,0x99,0x81,0x81,0x7e,
                8,8,0x7e,0xab,0xd5,0xab,0xd5,0xab,0xd5,0x7e,
                8,8,0x7e,0xff,0xff,0xf7,0xe7,0xff,0xff,0x7e};
char blank[] = {8,8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#endif


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
        GAME_T game;

        mallinit();
        sbrk(heap_space, sizeof(heap_space));
	gui_cls();
        srand(time(NULL));
        _delay = 75;

	do {
                c = gui_option("(T)etris or (A)ddtrix?", "tTaA");
                if (c == 'a' || c == 'A') {
                        game = ADDTRIS;
                        seg = 1;
                        wide = 10;
                } else {
                        game = TETRIS;
                        c = gui_option("(S)tandard or (M)odification?", "sSmM");
                        if (c == 's' || c == 'S') {
                                seg = 4;
                                wide = 10;
                        } else {
                                c = gui_option("Brick size? (1..9)", "123456789");
                                seg = c - '0';
                                c = gui_option("(S)tandard or (D)ouble wide?", "sSdD");
                                if (c == 's' || c == 'S') {
                                        wide = 10;
                                } else {
                                        wide = 20;
                                }
                        }
                }
                
                t_create_game(&tetris, game, wide, 20, seg);
                
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
        if (tetris.grid_size_x > 10) {
                tetris.origin_x = 12;
        } else {
                tetris.origin_x = 19;
        }
        tetris.origin_y = 1;

	gui_gotoxy(1,1);
	printf("Tetris\n%s", TETRIS_VERSION);
        
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

	gui_gotoxy(1,6);
	printf("Controls:");

	gui_gotoxy(1,8);
	printf("Left    7");
	gui_gotoxy(1,9);
	printf("Right   9");
	gui_gotoxy(1,10);
	printf("Rotate  8");
	gui_gotoxy(1,11);
	printf("Drop    4");

	gui_gotoxy(1,13);
	printf("Pause   P");
	gui_gotoxy(1,14);
	printf("Quit    Q");
}


void g_update_score() {
        static int last_score = -1;
        
        if (last_score == tetris.score)
                return;
                
        last_score = tetris.score;
	gui_gotoxy(1,4);
	printf("Score:%3d", tetris.score);
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

#ifdef USE_SPRITES
void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        x = tetris->origin_x * 8 + x * tetris->element_size * 8 - 8;
	y = tetris->origin_y * 8 + y * tetris->element_size * 8 - 8;
        color %=  3; 
        putsprite(spr_or, x, y, brick + SPRITE_OFFSET * color);
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x * 8 + x * tetris->element_size * 8 - 8;
	y = tetris->origin_y * 8 + y * tetris->element_size * 8 - 8;
        putsprite(spr_and, x, y, blank);
}

#else

void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size; 
	gui_gotoxy(x, y);
        if (tetris->game == TETRIS) {
                printf("%c[%um", 27, color_vec[color]);
                printf("O");
                printf("%c[%um", 27, color_vec[MESH_BK_COLOR]);
        } else {
                printf("%d", tetris->brick.value);
        }
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x + x * tetris->element_size;
	y = tetris->origin_y + y * tetris->element_size;
	gui_gotoxy(x,y);
        printf(" ");
}
#endif


void m_line_destroyed() {
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

