/*
*       main_tcx.c
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
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
#include <assert.h>
#include "sound_tc.h"
#include "tetris.h"
#include "tui_tc.h"
#include "wait_tc.h"
#include "res_tc.h"
#include "main.h"


#define BUFF_LEN 512
#define SC_NAME_LEN 10
#define SC_TABLE_LEN 5

typedef struct {
        char name[SC_NAME_LEN];
        int score;
} T_SCORE;

T_SCORE score_table[SC_TABLE_LEN];

static char fn_addtris[] = "addtris.dat";
static char fn_tetris[] = "tetris.dat";
static char fn_xtris[] = "xtris%d%s.dat";


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
        
static JOB_T *j2;       /* floating text */
#define j2p -1
static JOB_T *j3;       /* sound */
#define j3p 1
static JOB_T *j4;       /* animations */
#define j4p -1

static SND_SONG song;
static int play_sound;
static int _delay;
static int score_x;
static int score_y;


static int draw_mainscreen(TETRIS_T *t);
static void draw_goodbye(void);
static void draw_addtris(void);
static void draw_tetris(void);
static void draw_xtris(void);
static int update_score(int reset);
static int process_user_input(void);
/* Jobs */
static long draw_floating_text(enum W_ACTION a);
static long animate_scr_main(enum W_ACTION a);
static long animate_scr_add(enum W_ACTION a);
static long animate_scr_tet(enum W_ACTION a);
static long animate_scr_xte(enum W_ACTION a);
/* Score */
static void show_score(int my_score);


int main() {
        int c, ret;
        int d;
        

        mainw = tui_create_win(1, 1, TUI_SCR_X_SIZE, TUI_SCR_Y_SIZE, TUI_COL, TUI_BKCOL, ' ');
        srand(time(NULL) % 37);
        j2 = w_register_job(4, j2p, &draw_floating_text);
        j3 = NULL;
        j4 = NULL;
        play_sound = 1;

        c = 0;
        do {
                ret = draw_mainscreen(&tetris);
                
                /* Quit game */
                if (ret == 0) {
                        if (j3 != NULL)
                                w_unregister_job(j3);
                        if (j4 != NULL)
                                w_unregister_job(j4);
                        snd_speaker(0);
                        draw_goodbye();
                        w_unregister_job(j2);
                        tui_delete_win(mainw);
                        return 0;
                }
                         
                switch (tetris.game) {
                        case TETRIS:
                                draw_tetris();
                                break;
                        case XTRIS:
                                draw_xtris();
                                break;
                        case ADDTRIS:
                                draw_addtris();
                                break; 
                }
                
                if (j4 != NULL) j4->run(PAUSE);        
                tui_message("\n\x01\x0f Press any key to start \n", LIGHTCYAN, TUI_BKCOL);
                if (j4 != NULL) j4->run(UNPAUSE);
                        
                d = _delay;
                if (play_sound == 1)
                        j3 = w_register_job(6, j3p, &snd_play_sound);
        
                do {
                        int i;                             

                        for (i = 0; i < d; i++) {
                                c = process_user_input();
                                if (c == -1)
                                        break;
                                w_wait(1);
                        }
                        
                        ret = t_go(&tetris);
                        
                        if (update_score(0) == 1) {
                                d = _delay - (tetris.score / 20);
                                if (d < 3)
                                        d = 3;
                        }  
                } while ((ret != -1) && (c != -1));

                t_delete_game(&tetris);
                if (j3 != NULL) {
                        w_unregister_job(j3);
                        j3 = NULL;
                        snd_speaker(0);
                }
                
                if (j4 != NULL) {
                        w_unregister_job(j4);
                        j4 = NULL;
                }
                
                if (c != -1) {
                        tui_message("\n\x01\x0fGAME OVER\n", LIGHTCYAN, TUI_BKCOL);
                        show_score(tetris.score);
                }
                
        } while (1);
}


int draw_mainscreen(TETRIS_T *t) {
        int i, c, seg, wide, ret;
        char buff[BUFF_LEN];
        GAME_T game;
        int menu_only;
        
        ret = -1;
        menu_only = 0;
        do {
                if (menu_only == 0) {
                        tui_cls_win(mainw, FALSE);
                        tui_draw_box(15, 1, TUI_COL, TUI_BKCOL, gfx_ptakovina, FALSE);
                        
                        gotoxy(23,14);
                        tui_set_attr(0, BROWN, TUI_BKCOL);
                        for (i = 0; i < 34; i++)
                                putch('=');
                        tui_draw_box(5, 12, TUI_COL, TUI_BKCOL, gfx_bird_08, FALSE);
                        tui_draw_box(13, 9, TUI_COL, TUI_BKCOL, gfx_bird_10, FALSE);
                        tui_draw_box(57, 9, TUI_COL, TUI_BKCOL, gfx_bird_11, FALSE);
                        tui_draw_box(67, 12, TUI_COL, TUI_BKCOL, gfx_bird_09, FALSE);
                        tui_draw_box(54, 15, BROWN, TUI_BKCOL, "hjw", FALSE);
                        
                        tui_draw_box(17, 22, LIGHTGREEN, TUI_BKCOL, "- - - = = = (c) 2017 Jaroslav Beran = = = - - -", FALSE);
                        menu_only = 1;
                }
                
                if ((play_sound == 1) && (j3 == NULL)) {
                        song.duration = SD2;
                        song.rest = SR2;
                        song.song = s2;
                        snd_setsong(&song);
                        j3 = w_register_job(6, j3p, &snd_play_sound);
                }
                
                if ((play_sound == 0) && (j3 != NULL)) {
                        w_unregister_job(j3);
                        j3 = NULL;
                        snd_speaker(0);
                }
                
                sprintf(buff, "\x01\x0f\n"
                               " 1) Addtris \n\n" \
                               " 2) Tetris \n\n" \
                               " 3) X-Tris \n\n" \
                               " S) Sound: %s \n\n" \
                               " Q) Quit \n", \
                               (play_sound == 0) ? "off" : "on");
                assert(strlen(buff) < BUFF_LEN);
                               
                j4 = w_register_job(18, j4p, &animate_scr_main);
                
                c = tui_option(buff, "123SsQq", LIGHTCYAN, TUI_BKCOL);
                
                w_unregister_job(j4);
                j4 = NULL;
                        
                switch (c) {
                        case '1':
                                game = ADDTRIS;
                                seg = 1;
                                wide = 10;
                                _delay = 15;
                                ret = 1;
                                break;
                        case '2':
                                game = TETRIS;
                                seg = 4;
                                wide = 10;
                                _delay = 10;
                                ret = 1;
                                break;
                        case '3':
                                game = XTRIS;
                                c = tui_option("\n\x01\x0f Brick size? (1..9) \n", "123456789", LIGHTCYAN, TUI_BKCOL);
                                seg = c - '0';
                                c = tui_option("\n\x01\x0f (S)tandard grid or (D)ouble wide? \n", "sSdD", LIGHTCYAN, TUI_BKCOL);
                                if (c == 's' || c == 'S') {
                                        wide = 10;
                                } else {
                                        wide = 20;
                                }
                                if (seg > 4) {
                                        _delay = 15;
                                } else {
                                        _delay = 10;
                                }
                                ret = 1;
                                break;
                        case 'S':
                        case 's':
                                if (play_sound == 0) {
                                        play_sound = 1;
                                } else {
                                        play_sound = 0;
                                }
                                break;
                        case 'Q':
                        case 'q':
                                ret = 0;
                                break;
                }
        } while (ret == -1);
        
        if (j3 != NULL) {
                w_unregister_job(j3);
                j3 = NULL;
                snd_speaker(0);
        }
        
        if (ret == 1)
                t_create_game(t, game, wide, 20, seg);
        
        /* 0=quit game; 1=game created */
        return ret;
}


void draw_goodbye(void) {
        tui_cls_win(mainw, FALSE);
        tui_draw_box(5, 3, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
        tui_draw_box(5, 7, BROWN, TUI_BKCOL, "mrf", FALSE);
        
        tui_draw_box(22, 3, TUI_COL, TUI_BKCOL, gfx_owl_03, FALSE);
        tui_draw_box(20, 7, BROWN, TUI_BKCOL, "Joachim Kromm", FALSE);
        
        tui_draw_box(37, 3, TUI_COL, TUI_BKCOL, gfx_owl_02, FALSE);
        tui_draw_box(44, 7, BROWN, TUI_BKCOL, "as", FALSE);
        
        tui_draw_box(55, 1, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
        tui_draw_box(55, 5, BROWN, TUI_BKCOL, "jgs", FALSE);
        
        tui_draw_box(55, 9, TUI_COL, TUI_BKCOL, gfx_butter_02, FALSE);
        tui_draw_box(63, 13, WHITE, TUI_BKCOL, "S-v", FALSE);
        
        tui_draw_box(68, 12, TUI_COL, TUI_BKCOL, gfx_butter_03, FALSE);
        tui_draw_box(77, 16, BLUE, TUI_BKCOL, "S-v", FALSE);
        
        tui_draw_box(58, 17, TUI_COL, TUI_BKCOL, gfx_rabbit_05, FALSE);
        tui_draw_box(58, 23, RED, TUI_BKCOL, "Keely", FALSE);
        
        tui_draw_box(42, 18, TUI_COL, TUI_BKCOL, gfx_mush_01, FALSE);
        tui_draw_box(52, 22, GREEN, TUI_BKCOL, "dwb", FALSE);
        
        tui_draw_box(30, 17, TUI_COL, TUI_BKCOL, gfx_bee_02, FALSE);
        tui_draw_box(35, 21, DARKGRAY, TUI_BKCOL, "sjw", FALSE);
        
        tui_draw_box(16, 18, TUI_COL, TUI_BKCOL, gfx_bee_04, FALSE);
        tui_draw_box(28, 23, DARKGRAY, TUI_BKCOL, "jgs", FALSE);
        
        tui_draw_box(4, 11, TUI_COL, TUI_BKCOL, gfx_spray_01, FALSE);
        tui_draw_box(13, 9, TUI_COL, TUI_BKCOL, gfx_spray_02, FALSE);
        tui_draw_box(1, 21, LIGHTGRAY, TUI_BKCOL, "jgs", FALSE);
        
        tui_draw_box(80 - strlen(TETRIS_VERSION), 24, DARKGRAY, TUI_BKCOL, TETRIS_VERSION, FALSE);
        
        tui_message("\n\x01\x0fGood Bye\n", LIGHTCYAN, TUI_BKCOL);
}


void draw_addtris(void) {
        int x, y;

        tui_cls_win(mainw, FALSE);
	tetris.element_size = 1;
        tetris.origin_y = 3;

        tetris.origin_x = 47;
        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_addtris, FALSE);
        
        tui_set_attr(0, LIGHTMAGENTA, TUI_BKCOL);

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
        
        tui_set_attr(0, TUI_COL, TUI_BKCOL);
        tui_draw_box(69, 19, TUI_COL, TUI_BKCOL, gfx_mush_01, FALSE);
        tui_draw_box(6, 8, TUI_COL, TUI_BKCOL, gfx_witch_01, FALSE);
        tui_draw_box(5, 23, DARKGRAY, TUI_BKCOL, "snd", FALSE);
        
        #define ADD_OWL_X 35
        #define ADD_OWL_Y 7
        tui_draw_box(ADD_OWL_X, ADD_OWL_Y, TUI_COL, TUI_BKCOL, gfx_owl_03, FALSE);
        tui_draw_box(70, 3, TUI_COL, TUI_BKCOL, gfx_owl_02, FALSE);

        score_x = 70;
        score_y = 11;        
        update_score(1);
        
        if ((play_sound == 1) && (j3 == NULL)) {
                song.duration = SD4;
                song.rest = SR4;
                song.song = s4;
                snd_setsong(&song);
        }
        
        j4 = w_register_job(18, j4p, &animate_scr_add);
}


void draw_tetris(void) {
        int i, x, y;

        tui_cls_win(mainw, FALSE);
	tetris.element_size = 1;
        tetris.origin_y = 3;
        tetris.origin_x = 39;
        
        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_tetris, FALSE);
        
        tui_set_attr(0, LIGHTMAGENTA, TUI_BKCOL);

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
        
        gotoxy(12,11);
        tui_set_attr(0, BROWN, TUI_BKCOL);
        for (i = 0; i < 15; i++)
                putch('=');
        tui_draw_box(4, 9, TUI_COL, TUI_BKCOL, gfx_bird_08, FALSE);
        tui_draw_box(27, 9, TUI_COL, TUI_BKCOL, gfx_bird_09, FALSE);
        tui_draw_box(17, 8, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
        
        gotoxy(2, 23);
        tui_set_attr(0, LIGHTGREEN, TUI_BKCOL);
        for (x = 2; x < 24; x++)
                putch(',');
        tui_draw_box(2, 18, TUI_COL, TUI_BKCOL, gfx_rabbit_01, FALSE);
        tui_draw_box(26, 18, TUI_COL, TUI_BKCOL, gfx_rabbit_02, FALSE);        
        tui_draw_box(12, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_03, FALSE);
        tui_draw_box(22, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_04, FALSE);
        tui_draw_box(2, 24, BROWN, TUI_BKCOL, "b'ger", FALSE);
        
        tui_draw_box(62, 1, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);

        score_x = 66;
        score_y = 10;        
        update_score(1);
        
        tui_draw_box(62, 18, TUI_COL, TUI_BKCOL, gfx_rabbit_05, FALSE);
        
        if ((play_sound == 1) && (j3 == NULL)) {
                song.duration = SD6;
                song.rest = SR6;
                song.song = s6;
                snd_setsong(&song);
        }
        
        j4 = w_register_job(18, j4p, &animate_scr_tet);
}


void draw_xtris(void) {
        int x, y;

        tui_cls_win(mainw, FALSE);
	tetris.element_size = 1;
        tetris.origin_y = 3;

        tetris.origin_x = 40;
        tui_draw_box(1, 1, TUI_COL, TUI_BKCOL, gfx_xtris, FALSE);
        
        tui_set_attr(0, LIGHTMAGENTA, TUI_BKCOL);

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
        
        tui_set_attr(0, TUI_COL, TUI_BKCOL);
        tui_draw_box(2, 16, TUI_COL, TUI_BKCOL, gfx_plant_03, FALSE);
        
        if (tetris.grid_size_x == 10) {
                tui_draw_box(63, 3, TUI_COL, TUI_BKCOL, gfx_butter_02, FALSE);
                tui_draw_box(68, 8, TUI_COL, TUI_BKCOL, gfx_butter_03, FALSE);
                tui_draw_box(62, 17, TUI_COL, TUI_BKCOL, gfx_plant_04, FALSE);        
        }

        score_x = 3;
        score_y = 10;
        update_score(1);
        
        if ((play_sound == 1) && (j3 == NULL)) {
                song.duration = SD5;
                song.rest = SR5;
                song.song = s5;
                snd_setsong(&song);
        }
        
        j4 = w_register_job(18, j4p, &animate_scr_xte);
}


int update_score(int reset) {
        static int last_score;
        static int hidden = 0;
        int ret = 0;
        int redraw = 0;
        int clear = 0;
        /*
          reset:
          0 = draw only in case of value changes
          1 = reset value to 0
          2 = redraw all
          3 = hide all
        */
        
        switch(reset) {
                case 0:
                        if (last_score != tetris.score) {
                                ret = 1;
                                if (hidden == 0)
                                        redraw = 1;
                        }
                        break;
                case 1:
                        last_score = 0;
                        hidden = 0;
                        ret = 1;
                        redraw = 1;
                        break;
                case 2:
                        redraw = 1;
                        hidden = 0;
                        break;
                case 3:
                        clear = 1;
                        hidden = 1;
                        break;
        }

        last_score = tetris.score;
        
        if (redraw == 1) {
                gotoxy(score_x, score_y);
                tui_set_attr(0, WHITE, TUI_BKCOL);
                cprintf("Score: %3d", tetris.score);
                gotoxy(score_x, score_y + 2);
                cprintf("Pause .. P");
                gotoxy(score_x, score_y + 3);
                cprintf("Quit  .. Q");
                gotoxy(score_x, score_y + 4);
                cprintf("Help  .. H");
        }
        
        if (clear == 1) {
                gotoxy(score_x, score_y);
                tui_set_attr(0, WHITE, TUI_BKCOL);
                cprintf("          ");       
                gotoxy(score_x, score_y + 2);
                cprintf("          ");
                gotoxy(score_x, score_y + 3);
                cprintf("          ");
                gotoxy(score_x, score_y + 4);
                cprintf("          ");
        }
        
        return ret;
}


int process_user_input(void) {
        int c, result;
        G_BOOL_T r;

        result = 0;
        while (kbhit()) {

                int c;
                c = getch();

                switch (c) {
                        /* Cursor keys */
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
                                                        w_wait(0);
                                                break;
                                }
                                break;
                        /* Normal keys */
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
					w_wait(0);
				break;
                        case 'h':
                                if (j4 != NULL) j4->run(PAUSE);
                                switch (tetris.game) {
                                        case ADDTRIS:
                                                tui_message(
                                                        addtris_help, 
                                                        LIGHTCYAN, TUI_BKCOL);
                                                break;
                                        case TETRIS:
                                                tui_message(
                                                        tetris_help, 
                                                        LIGHTCYAN, TUI_BKCOL);
                                                break;
                                        case XTRIS:
                                                tui_message(
                                                        xtris_help, 
                                                        LIGHTCYAN, TUI_BKCOL);
                                                break; 
                                }
                                if (j4 != NULL) j4->run(UNPAUSE);
                                break;
                        case 'p':
                                if (j3 != NULL) j3->run(PAUSE);
                                if (j4 != NULL) j4->run(PAUSE);
                                tui_message("\n\x01\x0fPaused\n", LIGHTCYAN, TUI_BKCOL);
                                if (j3 != NULL) j3->run(UNPAUSE);
                                if (j4 != NULL) j4->run(UNPAUSE);
                                break;
                        case 'q':
                                if (j3 != NULL) j3->run(PAUSE);
                                if (j4 != NULL) j4->run(PAUSE);
                                r = tui_confirm("\n\x01\x0fQuit game? (Y/N)\n", LIGHTCYAN, TUI_BKCOL);
                                if (r == TRUE) {
                                        show_score(tetris.score);
                                        result = -1;
                                }
                                if (j3 != NULL) j3->run(UNPAUSE);
                                if (j4 != NULL) j4->run(UNPAUSE);
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
        } else {
                textcolor(WHITE);
                cprintf("%d ", tetris->brick.value);
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


void m_line_destroyed(void) {
        w_wait(0);
}


long draw_floating_text(enum W_ACTION a) {
        #define FT_X 5
        #define FT_Y 25
        #define FT_LEN 70
        
        static char b[(FT_LEN - 1) * 2];
        static char *p = NULL;
        static i;
        
        if ((a == RESET) || (p == NULL)) {
                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                gotoxy(FT_X, FT_Y);
                for(i = 0; i < FT_LEN; i++)
                        putch(' ');
                p = floating_text;
                i = 0;
                
                return 0;
        }
        
        if (a == RUN) {
                gotoxy(FT_X,FT_Y);
                tui_set_attr(0, DARKGRAY, TUI_BKCOL);
                
                gettext(FT_X + 1 , FT_Y, FT_X + 1 + (FT_LEN - 2), FT_Y, b);
                puttext(FT_X, FT_Y, FT_X + (FT_LEN - 2), FT_Y, b);
                
                gotoxy(FT_X + FT_LEN - 1, FT_Y);
                
                if (*p == '\0') {
                        putch(' ');
                        i++;
                        if (i == FT_LEN) {
                                p = NULL;
                                i = 0;
                        } 
                } else {
                        putch(*p);
                        p++;
                }
                
                tui_flush();
        }
        return 0;
}


long animate_scr_main(enum W_ACTION a) {
        static int step = -1;
        static int paused;

        if ((a == RESET) || (step == -1)) {
                step = 0;
                paused = 0;
                
                return 0;        
        }
        
        if (a == PAUSE) {
                paused = 1;
        }
        
        if (a == UNPAUSE) {
                paused = 0;
        }            
        
        if (a == RUN) {
                if (paused != 0)
                        return 0;
                        
                switch (step) {
                        case 0:
                                switch (rand() % 10) {
                                        case 1:
                                                step = 1;
                                                break;
                                        case 2:
                                                step = 3;
                                                break;
                                }
                                break;
                        case 1:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(14, 10);
                                putch('-');
                                gotoxy(16, 10);
                                putch('-');
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(15, 11);
                                putch('v');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 16;
                        case 2:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(14, 10);
                                putch('.');
                                gotoxy(16, 10);
                                putch('.');
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(15, 11);
                                putch('V');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        case 3:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(63, 10);
                                putch('-');
                                gotoxy(65, 10);
                                putch('-');
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(64, 11);
                                putch('v');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 16;
                        case 4:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(63, 10);
                                putch('.');
                                gotoxy(65, 10);
                                putch('.');
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(64, 11);
                                putch('V');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                }
        }
        
        return 18;
}


long animate_scr_add(enum W_ACTION a) {
        static int step = -1;
        static int paused;

        if ((a == RESET) || (step == -1)) {
                step = 0;
                paused = 0;
                
                return 0;
        }
        
        if (a == PAUSE) {
                paused = 1;
        }
        
        if (a == UNPAUSE) {
                paused = 0;
        }
        
        if (a == RUN) {
                if (paused != 0)
                        return 0;
                        
                switch (step) {
                        case 0:
                                switch (rand() % 20) {
                                        case 1:
                                                step = 1;
                                                break;
                                        case 2:
                                                step = 3;
                                                break;
                                        case 3:
                                                step = 5;
                                                break;
                                        case 4:
                                                step = 27;
                                                break;
                                        default:
                                                step = 7;
                                                break;
                                }
                                break;
                        /* owl */
                        case 1:
                                tui_draw_box(ADD_OWL_X, ADD_OWL_Y, TUI_COL, TUI_BKCOL, gfx_owl_04, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 2:
                                tui_draw_box(ADD_OWL_X, ADD_OWL_Y, TUI_COL, TUI_BKCOL, gfx_owl_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 8;
                        case 3:
                                tui_draw_box(ADD_OWL_X, ADD_OWL_Y, TUI_COL, TUI_BKCOL, gfx_owl_04, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 4:
                                tui_draw_box(ADD_OWL_X, ADD_OWL_Y, TUI_COL, TUI_BKCOL, gfx_owl_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        /* owl eyes */
                        case 5:
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                gotoxy(73, 4);
                                putch('.');
                                gotoxy(75, 4);
                                putch('.');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 20;
                        case 6:
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                gotoxy(73, 4);
                                putch('*');
                                gotoxy(75, 4);
                                putch('*');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        /* cooking */
                        case 7:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(14, 22);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 8:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(17, 22);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 9:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(15, 23);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 10:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(17, 23);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 11:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(14, 24);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 12:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(13, 24);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                                
                        case 13:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(14, 22);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 14:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(17, 22);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 15:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(15, 23);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 16:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(17, 23);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 17:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(14, 24);
                                putch(')');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 2;
                        case 18:
                                tui_set_attr(0, LIGHTRED, TUI_BKCOL);
                                gotoxy(13, 24);
                                putch('(');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step ++;
                                return 2;
                                
                        case 19:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(13, 14);
                                putch('.');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;
                        case 20:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(13, 14);
                                putch('*');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;
                        case 21:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(16, 12);
                                putch('.');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;
                        case 22:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(16, 12);
                                putch('*');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;        
                        case 23:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(10, 10);
                                putch('.');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;
                        case 24:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(10, 10);
                                putch('*');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;        
                        case 25:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(12, 9);
                                putch('.');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 3;
                        case 26:
                                tui_set_attr(0, WHITE, TUI_BKCOL);
                                gotoxy(12, 9);
                                putch('*');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step=0;
                                return 3;
                        /* eye */
                        case 27:
                                tui_set_attr(0, MAGENTA, TUI_BKCOL);
                                gotoxy(26, 13);
                                putch('-');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 10;
                        case 28:
                                tui_set_attr(0, LIGHTCYAN, TUI_BKCOL);
                                gotoxy(26, 13);
                                putch('o');
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step=0;
                                return 18;
                }
        }
        
        return 18;
}


long animate_scr_tet(enum W_ACTION a) {
        static int step = -1;
        static int paused;
        

        if ((a == RESET) || (step == -1)) {
                step = 0;
                paused = 0;
                
                return 0;        
        }
        
        if (a == PAUSE) {
                paused = 1;
        }
        
        if (a == UNPAUSE) {
                paused = 0;
        }            
        
        if (a == RUN) {
                if (paused != 0)
                        return 0;
                        
                switch (step) {
                        case 0:
                                switch (rand() % 10) {
                                        case 1:
                                                step = 1;
                                                break;
                                        case 2:
                                                step = 7;
                                                break;
                                        case 3:
                                                step = 13;
                                                break;
                                        case 4:
                                                step = 17;
                                                break;
                                }
                                break;
                        /* rabbit */
                        case 1:
                                tui_draw_box(13, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_031, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 2:
                                tui_draw_box(14, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_03, FALSE);
                                tui_set_attr(0, LIGHTGRAY, TUI_BKCOL);
                                gotoxy(67, 20);
                                cprintf("oo");
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 16;
                        case 3:
                                tui_draw_box(14, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_04, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 4:
                                tui_draw_box(13, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_041, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 5:
                                tui_draw_box(12, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_04, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 6:
                                tui_draw_box(12, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_03, FALSE);
                                tui_set_attr(0, LIGHTGRAY, TUI_BKCOL);
                                gotoxy(67, 20);
                                cprintf("..");
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        /* rabbit */
                        case 7:
                                tui_draw_box(21, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_041, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 8:
                                tui_draw_box(20, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_04, FALSE);
                                tui_set_attr(0, LIGHTGRAY, TUI_BKCOL);
                                gotoxy(67, 20);
                                cprintf("oo");
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 16;
                        case 9:
                                tui_draw_box(20, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_031, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 10:
                                tui_draw_box(21, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 11:
                                tui_draw_box(22, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_031, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 12:
                                tui_draw_box(22, 19, TUI_COL, TUI_BKCOL, gfx_rabbit_04, FALSE);
                                tui_set_attr(0, LIGHTGRAY, TUI_BKCOL);
                                gotoxy(67, 20);
                                cprintf("..");
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        /* bird */
                        case 13:
                                tui_draw_box(16, 8, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 14:
                                tui_draw_box(15, 8, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 36;
                        case 15:
                                tui_draw_box(16, 8, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 5;
                        case 16:
                                tui_draw_box(17, 8, TUI_COL, TUI_BKCOL, gfx_bird_06, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                        /* bird */        
                        case 17:
                                tui_del_box(62, 1, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_draw_box(61, 2, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 9;
                        case 18:
                                tui_del_box(61, 2, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_draw_box(62, 3, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 9;
                        case 19:
                                tui_del_box(62, 3, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_draw_box(62, 2, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step++;
                                return 9;
                        case 20:
                                tui_del_box(62, 2, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_draw_box(62, 1, TUI_COL, TUI_BKCOL, gfx_bird_03, FALSE);
                                tui_set_attr(0, TUI_COL, TUI_BKCOL);
                                tui_flush();
                                step = 0;
                                return 18;
                }
        }
        
        return 18;
}

long animate_scr_xte(enum W_ACTION a) {
        static int step = -1;
        static int paused = 0;
        
        static int x2;
        static int y2;
        static int d2;
        static char *ps2;
        
        if ((a == RESET) || (step == -1)) {
                step = 0;
                paused = 0;
                
                x2 = 26;
                y2 = 10;
                d2 = 0;
                
                return 0;        
        }
        
        if (a == PAUSE) {
                paused = 1;
        }
        
        if (a == UNPAUSE) {
                paused = 0;
        }            
        
        if (a == RUN) {
                if (paused != 0)
                        return 0;
                        
                switch (step) {
                        case 0:
                                switch (rand() % 10) {
                                        case 1: 
                                                if (x2 == 26) {
                                                        step = 1;
                                                        x2 = 26;
                                                        y2 = 8;
                                                        d2 = -1;
                                                        ps2 = gfx_bee_04;
                                                }
                                                break; 
                                }
                                
                                if (step != 0) break;
                                
                                /* bee 2 */
                                switch (x2) {
                                        case 26:
                                                y2 = 10;
                                                d2 = -1;
                                                ps2 = gfx_bee_02;
                                                break;
                                        case 15:
                                                y2 = 10;
                                                d2 = 1;
                                                ps2 = gfx_bee_01;
                                                break;
                                }
                                x2 += d2;
                                tui_draw_box(x2, y2, TUI_COL, TUI_BKCOL, ps2, FALSE);
                                tui_flush();
                                break;
                        case 1:
                                /* hornet */
                                switch (x2) {
                                        case 20:
                                                update_score(3); /* hide score */
                                                tui_draw_box(2, 10, TUI_COL, TUI_BKCOL, gfx_spray_01, FALSE);
                                                tui_flush();
                                                break;
                                        case 15:
                                                tui_draw_box(11, 8, TUI_COL, TUI_BKCOL, gfx_spray_02, FALSE);
                                                gotoxy(9,11);
                                                tui_set_attr(0, LIGHTGRAY, TUI_BKCOL);
                                                putch('-');
                                                tui_flush();
                                                x2 += d2;
                                                return 36;
                                        case 14:
                                                step = 0;
                                                tui_del_box(x2, y2, TUI_COL, TUI_BKCOL, ps2, FALSE);
                                                tui_del_box(2, 10, TUI_COL, TUI_BKCOL, gfx_spray_01, FALSE);
                                                tui_del_box(11, 8, TUI_COL, TUI_BKCOL, gfx_spray_02, FALSE);
                                                tui_draw_box(2, 16, TUI_COL, TUI_BKCOL, gfx_plant_03, FALSE);
                                                update_score(2); /* redraw score */
                                                tui_flush();
                                                x2 = 26;
                                                y2 = 10;
                                                d2 = -1;
                                                ps2 = gfx_bee_02;
                                                break;
                                }
                                if (step != 1) break;
                                x2 += d2;
                                tui_draw_box(x2, y2, TUI_COL, TUI_BKCOL, ps2, FALSE);
                                tui_flush();
                                break;
                }        
        }
        return 5;
}


int cmp_score(const void *a, const void *b) {
        return (((T_SCORE *)b)->score - ((T_SCORE *)a)->score);
}


void show_score(int my_score) {
        int i;
        char *filename;
        char report[BUFF_LEN];
        #define FNBUFFLEN 13
        char fnbuff[FNBUFFLEN];
        FILE *f;
        
        /* choose filename */
        switch (tetris.game) {
                case ADDTRIS:
                        filename = fn_addtris; 
                        break;
                case TETRIS:
                       filename = fn_tetris;
                        break;
                case XTRIS:
                        sprintf(fnbuff, fn_xtris, tetris.brick.shape_size, (tetris.grid_size_x == 10) ? "s" : "d");
                        assert(strlen(fnbuff) < FNBUFFLEN);
                        filename = fnbuff;
                        break; 
        }
        
        /* load from file */
        f = fopen(filename, "rb");
        if (f != NULL) {
                fread(score_table, sizeof(T_SCORE), SC_TABLE_LEN, f);
                fclose(f);
        } else {
                for (i = 0; i < SC_TABLE_LEN; i++) {
                        strcpy(score_table[i].name, "empty");
                        score_table[i].score = 0;
                }
        }
        
        /* update table */
        if (my_score > score_table[4].score) {
                tui_input("\x01\x0fPlease enter your name:", 
                        score_table[4].name, SC_NAME_LEN, 
                        LIGHTCYAN, TUI_BKCOL);
                score_table[4].score = my_score;         
        }
        qsort(score_table, SC_TABLE_LEN, sizeof(T_SCORE), cmp_score);
        
        /* show hall of fame */
        sprintf(report, "\n\x01\x0f - - - = = = Hall Of Fame = = = - - - \n\n"
                        "\x01\x0f 1.\x01\x0e %-10s . . . . . . . . . %4d \n"
                        "\x01\x0f 2.\x01\x0d %-10s . . . . . . . . . %4d \n"
                        "\x01\x0f 3.\x01\x0c %-10s . . . . . . . . . %4d \n"
                        "\x01\x0f 4.\x01\x0b %-10s . . . . . . . . . %4d \n"
                        "\x01\x0f 5.\x01\x0a %-10s . . . . . . . . . %4d \n"
                        "\n\x01\x0f - - - = = = Best Of Best = = = - - - \n", 
                        score_table[0].name, score_table[0].score,
                        score_table[1].name, score_table[1].score,
                        score_table[2].name, score_table[2].score,
                        score_table[3].name, score_table[3].score,
                        score_table[4].name, score_table[4].score
                        );
        assert(strlen(report) < BUFF_LEN);
        tui_message(report, LIGHTCYAN, TUI_BKCOL);
        
        /* write to file file */
        f = fopen(filename, "wb");
        if (f != NULL) {
                fwrite(score_table, sizeof(T_SCORE), SC_TABLE_LEN, f);
                fclose(f);
        } else {
                perror(filename);
                exit(-1);
        } 
}


