/*
*       main_tcx.c
*
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 12.6.2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "tetris.h"
#include "tui_tc.h"
#include "main.h"


#define TUI_COL LIGHTGRAY
#define TUI_BKCOL BLACK

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
static WINDOW_T *mainw = NULL;


static void g_draw_mesh(int grid_size);
static void g_print_controls();
static void g_update_score();
static void process_user_input();
static void wait(int ms);


int main() {
        int c, seg, wide, ret;
        GAME_T game;

	mainw = tui_create_win(1, 1, TUI_SCR_X_SIZE, TUI_SCR_Y_SIZE, TUI_COL, TUI_BKCOL, 0xb0);
        srand(time(NULL) % 37);

        do {
                game = TETRIS;
		c = tui_option("(A)ddtrix or (T)etris?\nTest, test\n", "tTaA", TUI_COL, TUI_BKCOL);
                if (c == 'a' || c == 'A') {
                        game = ADDTRIS;
                        seg = 1;
                        wide = 10;
                        _delay = 170;
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
                        _delay = 90;
                }

                t_create_game(&tetris, game, wide, 20, seg);

                g_draw_mesh(1);
                tui_message("Press any key to start ...", TUI_COL, TUI_BKCOL);

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
                tui_message("GAME OVER", TUI_COL, TUI_BKCOL);
                c = tui_option("(N)ew Game or (Q)uit", "nNqQ", TUI_COL, TUI_BKCOL);
        } while ((c == 'n') || (c == 'N'));

        tui_delete_win(mainw);
        return 0;
}


void g_draw_mesh(int grid_size) {
        int x, y;

        tui_cls_win(mainw);
	tetris.element_size = grid_size;
        tetris.origin_x = 30;
        tetris.origin_y = 3;

	gotoxy(2,2);
	tui_set_attr(BLINK, YELLOW, BLUE);
        cprintf("Tetris %s", TETRIS_VERSION);
        tui_set_attr(0, TUI_COL, TUI_BKCOL);

        for (y = 0; y < tetris.element_size * tetris.grid_size_y; y++) {
                gotoxy(tetris.origin_x - 1, tetris.origin_y + y);
                putch(0xb3);
                for (x = 0; x < tetris.element_size * 2 * tetris.grid_size_x; x++) {
                        putch(' ');
                }
                putch(0xb3);
        }

        gotoxy(tetris.origin_x - 1, tetris.origin_y + tetris.element_size * tetris.grid_size_y);
        putch(0xc0);
        for (x = 0; x < tetris.element_size * 2 * tetris.grid_size_x; x++) {
                putch(0xc4);
        }
        putch(0xd9);

        g_print_controls();
        g_update_score();
}


void g_print_controls() {

        tui_draw_box(2, 6, TUI_COL, TUI_BKCOL, \
                "Controls:\n\n" \
                "Left   ... 7\n" \
                "Right  ... 9\n" \
                "Rotate ... 8\n" \
                "Drop   ... 4\n\n" \
                "Pause  ... P\n" \
                "Quit   ... Q\n" \
                );
}


void g_update_score() {
        static int last_score = -1;

        if (last_score == tetris.score)
                return;

        last_score = tetris.score;
        gotoxy(2, 4);
        printf("Score: %4d", tetris.score);
}


void process_user_input() {
        int c;

        while (kbhit()) {

                int c;
                c = getch();

                switch (c) {
                        case 0 :
                                c = getch();
                                switch (c) {
                                        case 75:
                                                t_move_left(&tetris);
                                                break;
                                        case 72:
                                                t_rotate(&tetris, 1);
                                                break;
                                        case 77:
                                                t_move_right(&tetris);
                                                break;
                                        case 80:
                                                while(t_move_down(&tetris) != -1)
                                                        ;
                                                break;
                                }
                                break;
                        case 'p':
                                tui_message("Paused", TUI_COL, TUI_BKCOL);
                                break;
                        case 'q':
                                if (tui_confirm("Do you want to quit game? (Y/N)", TUI_COL, TUI_BKCOL) == G_TRUE) {
                                        tui_delete_win(mainw);
                                        exit(0);
                                }
                }
        }
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        x = tetris->origin_x + x * tetris->element_size * 2;
        y = tetris->origin_y + y * tetris->element_size;
        gotoxy(x, y);
        if (tetris->game == TETRIS) {
                textcolor(color_vec[color]);
                cprintf("%c%c", 0xdb, 0xdb);
                textcolor(color_vec[MESH_BK_COLOR]);
        } else {
                printf("%d ", tetris->brick.value);
        }
        tui_flush();
}


void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y) {
        x = tetris->origin_x + x * tetris->element_size * 2;
        y = tetris->origin_y + y * tetris->element_size;
        gotoxy(x,y);
        printf("  ");
        tui_flush();
}


void wait(int ms) {
        clock_t endwait;

        endwait = clock() + ms * CLK_TCK / 1000;
        while (clock() < endwait)
                ;
}

