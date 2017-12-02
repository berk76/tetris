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
#include <termios.h>
#include <unistd.h>
#include "tetris.h"
#include "tui_con.h"
#include "main.h"


#define G_FALSE 0
#define G_TRUE  1

#define NB_ENABLE 1
#define NB_DISABLE 0


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

/* gui functions */
static void gui_message(char *msg);
static int gui_confirm(char *msg);
static int gui_option(char *msg, char *options);
static int gui_getk();
static void gui_nonblock(int state);
static void gui_cleanup();


int main() {
        int c, seg, wide, ret;
        GAME_T game;

        gui_nonblock(NB_ENABLE);
        atexit(gui_cleanup);
        mainw = tui_create_win(1, 1, TUI_SCR_X_SIZE, TUI_SCR_Y_SIZE, TUI_COL, TUI_BKCOL, ' ');
        srand(time(NULL));

        do {
                c = gui_option("(A)ddtrix or (T)etris?", "tTaA");
                if (c == 'a' || c == 'A') {
                        game = ADDTRIS;
                        seg = 1;
                        wide = 10;
                        _delay = 100000;
                } else {
                        game = TETRIS;
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
                        _delay = 60000;
                }

                t_create_game(&tetris, game, wide, 20, seg);
                
                g_draw_mesh(1);
                gui_message("Press any key to start ...");

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
                gui_message("GAME OVER");
                c = gui_option("(N)ew Game or (Q)uit", "nNqQ");
        } while ((c == 'n') || (c == 'N'));
        
        gui_nonblock(NB_DISABLE);
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

        while ((c = gui_getk()) != 0) {

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

/*** GUI ***/

static void gui_draw_message(char *msg);
static void gui_delete_message();
static int gui_wait_for_key(char *s);
static void gui_wait_for_any_key();
static int gui_kbhit();


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
        tui_gotoxy(1,22);
        printf("%s", msg);
        tui_flush();
}


void gui_delete_message() {
        tui_gotoxy(1,22);
        printf("                                                  ");
        tui_flush();
}


int gui_wait_for_key(char *s) {
        int c;

        while (1) {
                while ((c = gui_getk()) != 0) {
                        if (strchr(s, c) != NULL)
                                return c;
                }
                usleep(10000);
        }
}


void gui_wait_for_any_key() {
        while (gui_getk()) {
        }

        while (!gui_getk()) {
                usleep(10000);
        }
}


int gui_getk() {
        int result;

        result = 0;

        if (gui_kbhit()) {
                result = getchar();
        }

        return result;
}


int gui_kbhit() {
        struct timeval tv;
        fd_set fds;

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
        select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &fds);
}


void gui_nonblock(int state) {
        struct termios ttystate;

        //get the terminal state
        tcgetattr(STDIN_FILENO, &ttystate);

        if (state==NB_ENABLE) {
                //turn off canonical mode
                ttystate.c_lflag &= ~ICANON;
                ttystate.c_lflag &= ~ECHO;
                //minimum of number input read.
                ttystate.c_cc[VTIME] = 0;
                ttystate.c_cc[VMIN] = 0;
        }
        else if (state==NB_DISABLE) {
                //turn on canonical mode
                ttystate.c_lflag |= ICANON;
                ttystate.c_lflag |= ECHO;
        }
        //set the terminal attributes.
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}


void gui_cleanup() {
        printf("%c[m",27); //set default text attributes
        printf("%c[2J",27); // erases the screen with the background colour and moves the cursor to home.
        gui_nonblock(NB_DISABLE);
}
