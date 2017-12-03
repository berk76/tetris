/*
*       main_con.c
*       
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 25.7.2016
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "tetris.h"
#include "tui_con.h"
#include "main.h"


static int MESH_BK_COLOR = 7;
static TETRIS_T tetris;
static useconds_t _delay;
static int color_vec[] = {44,42,46,41,45,43,47,40,47};


#define TUI_COL LIGHTGRAY
#define TUI_BKCOL BLACK
static WINDOW_T *mainw = NULL;


static void g_draw_mesh(int grid_size);
static void g_print_controls();
static void g_update_score();
static void process_user_input();


int main() {
        int c, seg, wide, ret;
        GAME_T game;

        tui_init();
        atexit(tui_cleanup);
        mainw = tui_create_win(1, 1, TUI_SCR_X_SIZE, TUI_SCR_Y_SIZE, TUI_COL, TUI_BKCOL, ' ');
        srand(time(NULL));

        do {
                c = tui_option("(A)ddtrix or (T)etris?", "tTaA", TUI_COL, TUI_BKCOL);
                if (c == 'a' || c == 'A') {
                        game = ADDTRIS;
                        seg = 1;
                        wide = 10;
                        _delay = 100000;
                } else {
                        game = TETRIS;
                        c = tui_option("(S)tandard tetris or (M)odification?", "sSmM", TUI_COL, TUI_BKCOL);
                        if (c == 's' || c == 'S') {
                                seg = 4;
                                wide = 10;
                        } else {
                                c = tui_option("Brick size? (1..9)", "123456789", TUI_COL, TUI_BKCOL);
                                seg = c - '0';
                                c = tui_option("(S)tandard grid or (D)ouble wide?", "sSdD", TUI_COL, TUI_BKCOL);
                                if (c == 's' || c == 'S') {
                                        wide = 10;
                                } else {
                                        wide = 20;
                                }
                        }
                        _delay = 60000;
                }

                t_create_game(&tetris, game, wide, 20, seg);
                
                g_draw_mesh(1);
                tui_message("Press any key to start ...", TUI_COL, TUI_BKCOL);

                do {
                        int i;
                        for (i = 0; i < 5; i++) {
                              process_user_input();
                              usleep(_delay);
                              tui_flush();
                        }
                        ret = t_go(&tetris);
                        g_update_score();
                        tui_flush();
                } while (ret != -1);

                t_delete_game(&tetris);
                tui_message("GAME OVER", TUI_COL, TUI_BKCOL);
                c = tui_option("(N)ew Game or (Q)uit", "nNqQ", TUI_COL, TUI_BKCOL);
        } while ((c == 'n') || (c == 'N'));
        

        return 0;
}


void g_draw_mesh(int grid_size) {
        int i;

        tui_cls_win(mainw, TRUE);
        tetris.element_size = grid_size;
        tetris.origin_x = 30;
        tetris.origin_y = 1;

        tui_gotoxy(1,1);
        printf("Tetris %s", TETRIS_VERSION);
        
        printf("%c[%um", 27, 33);
        for (i = 0; i < tetris.element_size * tetris.grid_size_y; i++) {
                tui_gotoxy(tetris.origin_x - 1, i + 1);
                printf("*");
                tui_gotoxy(tetris.origin_x + tetris.element_size * 2 * tetris.grid_size_x, i + 1);
                printf("*");
        }

        for (i = 0; i < tetris.element_size * 2 * tetris.grid_size_x; i++) {
                tui_gotoxy( tetris.origin_x + i, tetris.element_size * tetris.grid_size_y + 1);
                printf("*");
        }
        printf("%c[%um", 27, 37);

        g_print_controls();
        g_update_score();
        tui_flush();
}


void g_print_controls() {

        tui_gotoxy(1,5);
        printf("Controls:");

        tui_gotoxy(1,7);
        printf("Left   ... 7");
        tui_gotoxy(1,8);
        printf("Right  ... 9");
        tui_gotoxy(1,9);
        printf("Rotate ... 8");
        tui_gotoxy(1,10);
        printf("Drop   ... 4");

        tui_gotoxy(1,12);
        printf("Pause  ... P");
        tui_gotoxy(1,13);
        printf("Quit   ... Q");
}


void g_update_score() {
        static int last_score = -1;
        
        if (last_score == tetris.score)
                return;

        last_score = tetris.score;
        tui_gotoxy(1,3);
        printf("Score: %4d", tetris.score);
}


void process_user_input() {
        int c;

        while ((c = tui_getk()) != 0) {

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
                                tui_message("Paused", TUI_COL, TUI_BKCOL);
                                break;
                        case 'q':
                                if (tui_confirm("Do you want to quit game? (Y/N)", TUI_COL, TUI_BKCOL) == TRUE)
                                exit(0);
                }
        }
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        x = tetris->origin_x + x * tetris->element_size * 2;
        y = tetris->origin_y + y * tetris->element_size; 
        tui_gotoxy(x, y);
        if (tetris->game == TETRIS) {
                printf("%c[%um", 27, color_vec[color]);
                printf("OO");
                printf("%c[%um", 27, color_vec[MESH_BK_COLOR]);
        } else {
                printf("%d ", tetris->brick.value);
        }
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x + x * tetris->element_size * 2;
        y = tetris->origin_y + y * tetris->element_size;
        tui_gotoxy(x,y);
        printf("  ");
}


void m_line_destroyed() {
}

