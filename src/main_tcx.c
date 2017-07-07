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
#include <conio.h>
#include "tetris.h"
#include "tui_tc.h"
#include "tui_gfx.h"
#include "wait_tc.h"
#include "main.h"


#define TUI_COL LIGHTGRAY
#define TUI_BKCOL BLACK

static int MESH_BK_COLOR = 7;
static TETRIS_T tetris;
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
static char floating_text[] = "Ptakovina game was created in year 2017 " \
        "as part of developers competition published at www.high-voltage.cz. " \
        "I would like thank to Sledge for making such challenges and for " \
        "pushing us to create crazy games. Also I would like thank to web " \
        "www.chris.com and all ascii art creators such as jgs, mrf, as, lc " \
        "and many others for their wonderful ascii creatures. " \
        "This software consists of three games: 1) ADDTRIS is game invented " \
        "in 2016 by Vasek Petourka. Game was published at www.8bity.cz. "\
        "2) TETRIS is well known game invented in 1984 by Russian game " \
        "designer Alexey Pajitnov. 3) X-Tris is my crazy modification of " \
        "Tetris game where you can cook by yourself some parameters and " \
        "create some crazy challenge to manage. - berk -   * * *   ";


static void g_draw_mesh(int grid_size);
static void g_print_controls();
static void g_update_score();
static int process_user_input();
static void draw_star();
static void draw_floating_text();


int main() {
        int c, seg, wide, ret;
        unsigned _delay;
        GAME_T game;
        JOB_T *j1, *j2;

	mainw = tui_create_win(1, 1, TUI_SCR_X_SIZE, TUI_SCR_Y_SIZE, TUI_COL, TUI_BKCOL, ' ');
        srand(time(NULL) % 37);
        j1 = w_register_job(300, &draw_star);
        j2 = w_register_job(250, &draw_floating_text); 

        do {
                game = TETRIS;
                
                tui_cls_win(mainw, FALSE);
                tui_draw_box(15, 1, TUI_COL, TUI_BKCOL, gfx_ptakovina, FALSE);
                tui_draw_box(5, 9, TUI_COL, TUI_BKCOL, gfx_bird_05, FALSE);
                
		c = tui_option("\n1) Addtris\n\n2) Tetris\n\n3) X-Tris\n\n4) Quit\n", "1234", TUI_COL, TUI_BKCOL);
                switch (c) {
                        case '1':
                                game = ADDTRIS;
                                seg = 1;
                                wide = 10;
                                _delay = 170;
                                break;
                        case '2':
                                game = TETRIS;
                                seg = 4;
                                wide = 10;
                                _delay = 100;
                                break;
                        case '3':
                                game = XTRIS;
                                c = tui_option("\nBrick size? (1..9)\n", "123456789", TUI_COL, TUI_BKCOL);
                                seg = c - '0';
                                c = tui_option("\n(S)tandard grid or (D)ouble wide?\n", "sSdD", TUI_COL, TUI_BKCOL);
                                if (c == 's' || c == 'S') {
                                        wide = 10;
                                } else {
                                        wide = 20;
                                }
                                _delay = 100;
                                break;
                        case '4':
                                w_unregister_job(j1);
                                w_unregister_job(j2);
                                tui_delete_win(mainw);
                                return 0;
                }

                t_create_game(&tetris, game, wide, 20, seg);

                g_draw_mesh(1);
                tui_message("\nPress any key to start ...\n", TUI_COL, TUI_BKCOL);

                do {
                        int i;
                        for (i = 0; i < 5; i++) {
                                c = process_user_input();
                                if (c == -1)
                                        break;
                                w_wait(_delay);
                        }
                        ret = t_go(&tetris);
                        g_update_score();  
                } while ((ret != -1) && (c != -1));

                t_delete_game(&tetris);
                if (c != -1)
                        tui_message("\nGAME OVER\n", TUI_COL, TUI_BKCOL);
                
        } while (1);
}


void g_draw_mesh(int grid_size) {
        int x, y;

        tui_cls_win(mainw, FALSE);
	tetris.element_size = grid_size;
        tetris.origin_y = 3;

        switch (tetris.game) {
                case TETRIS:
                        tetris.origin_x = 39;
                        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_tetris, FALSE);
                        break;
                case XTRIS:
                        tetris.origin_x = 40;
                        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_xtris, FALSE);
                        break;
                case ADDTRIS:
                        tetris.origin_x = 47;
                        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_addtris, FALSE);
                        break; 
        }
        
        tui_set_attr(0, TUI_COL, TUI_BKCOL);

        for (y = 0; y < tetris.element_size * tetris.grid_size_y; y++) {
                gotoxy(tetris.origin_x - 1, tetris.origin_y + y);
                putch('|');
                for (x = 0; x < tetris.element_size * 2 * tetris.grid_size_x; x++) {
                        putch(' ');
                }
                putch('|');
        }

        gotoxy(tetris.origin_x - 1, tetris.origin_y + tetris.element_size * tetris.grid_size_y);
        putch('+');
        for (x = 0; x < tetris.element_size * 2 * tetris.grid_size_x; x++) {
                putch('-');
        }
        putch('+');
        
        if ((tetris.game == TETRIS) && (tetris.grid_size_x <= 10)) {
                /*
                tui_draw_box(tetris.origin_x + tetris.grid_size_x * 2, \
                        tetris.origin_y + 4, TUI_COL, TUI_BKCOL, gfx_bird_02, FALSE);
                */        
                tui_draw_box(tetris.origin_x + 23, \
                        1, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                        
                tui_draw_box(tetris.origin_x + 23, \
                        7, TUI_COL, TUI_BKCOL, \
                        "Jaroslav Beran\n  GNU/GPL v3\n   (c) 2017\n" \
                        " Creatures by:\n www.chris.com", \
                        TRUE);
                        
                tui_draw_box(tetris.origin_x + 23, \
                        15, TUI_COL, TUI_BKCOL, gfx_bird_04, FALSE);
        }
        
        if (tetris.game == ADDTRIS) {
                tui_draw_box(69, 19, TUI_COL, TUI_BKCOL, gfx_mush_01, FALSE);
        }

        g_print_controls();
        g_update_score();
}


void g_print_controls() {

        tui_draw_box(2, 14, TUI_COL, TUI_BKCOL, \
                "Controls:\n\n" \
                "Left   ---------- arrow\n" \
                "Right  ---------- arrow\n" \
                "Rotate ---------- arrow\n" \
                "Drop   ---------- arrow\n\n\n" \
                "Pause  ---------- P\n" \
                "Quit   ---------- Q\n", \
                FALSE);
                
        tui_draw_box(10, 12, TUI_COL, TUI_BKCOL, gfx_bird_01, FALSE);
        
}


void g_update_score() {
        static int last_score = -1;
        char s[20];

        if (last_score == tetris.score)
                return;

        last_score = tetris.score;
        
        sprintf(s, "Score: %4d", tetris.score);
        tui_draw_box(2, 9, TUI_COL, TUI_BKCOL, s, TRUE);
}


int process_user_input() {
        int c, result;

        result = 0;
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
                                tui_message("\nPaused\n", TUI_COL, TUI_BKCOL);
                                break;
                        case 'q':
                                if (tui_confirm("\nDo you want to quit game? (Y/N)\n", TUI_COL, TUI_BKCOL) == TRUE) {
                                        result = -1;
                                }
                }
        }
        
        return result;
}


void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color) {
        x = tetris->origin_x + x * tetris->element_size * 2;
        y = tetris->origin_y + y * tetris->element_size;
        gotoxy(x, y);
        if ((tetris->game == TETRIS) || (tetris->game == XTRIS)) {
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

void draw_star() {
        static char s[] = " .+***+. ";
        static char *pc;
        
        if ((*pc == '\0') || (*pc == NULL))
                pc = s;
        
        gotoxy(79,25);
        tui_set_attr(0, TUI_COL, TUI_BKCOL);
        putch(*pc);
        tui_flush();
        pc++;
        
}

void draw_floating_text() {
        #define FT_X 5
        #define FT_Y 25
        #define FT_LEN 70
        
        static char b[(FT_LEN - 1) * 2];
        static char *p = NULL;
        
        gotoxy(FT_X,FT_Y);
        tui_set_attr(0, TUI_COL, TUI_BKCOL);
        
        if ((p == NULL) || (*p == '\0')) {
                p = floating_text;
        }
        
        gettext(FT_X + 1 , FT_Y, FT_X + 1 + (FT_LEN - 2), FT_Y, b);
        puttext(FT_X, FT_Y, FT_X + (FT_LEN - 2), FT_Y, b);
        
        gotoxy(FT_X + FT_LEN - 1, FT_Y);
        putch(*p);
        p++;
        tui_flush();
}

