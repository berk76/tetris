/*
*       tui_tc.c
*
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 14.6.2017
*/


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "tui_tc.h"


static WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor);
static int tui_wait_for_key(char *s);
static void tui_wait_for_any_key();


/* External functions */

WINDOW_T *tui_create_win(int x, int y, int size_x, int size_y, int color, int bkcolor, char bkchar) {
        WINDOW_T *w;
        int i;

        w = (WINDOW_T *) malloc(sizeof(WINDOW_T));
        assert(w != NULL);

        w->x = x;
        w->y = y;
        w->size_x = size_x;
        w->size_y = size_y;
        w->color = color;
        w->bkcolor = bkcolor;
        w->bkchar = bkchar;
        w->old_content = (char *) malloc(size_x * size_y * 2 * sizeof(char));
        assert(w->old_content != NULL);
        i = gettext(w->x, w->y, w->x + w->size_x - 1, w->y + w->size_y - 1, w->old_content);
        assert(i != 0);

        tui_cls_win(w);

        return w;
}


void tui_delete_win(WINDOW_T *w) {
        int i;

        if (w->old_content != NULL) {
                i = puttext(w->x, w->y, w->x + w->size_x - 1, w->y + w->size_y - 1, w->old_content);
                assert(i != 0);
                free((void *) w->old_content);
                w->old_content = NULL;
        }

        if (w != NULL) {
                free((void *) w);
                w = NULL;
        }
}


void tui_cls_win(WINDOW_T *w) {
        int a, b;

        tui_set_attr(0, w->color, w->bkcolor);
        for (a = 0; a < w->size_y; a++) {
                gotoxy(w->x, w->y + a);
                for (b = 0; b < w->size_x; b++) {
                        putch(w->bkchar);
                }
        }
        tui_flush();
}


void tui_flush() {
	gotoxy(1,25);
}


void tui_message(char *msg, int color, int bkcolor) {
        WINDOW_T *w;

        w = tui_draw_message(msg, color, bkcolor);
        tui_wait_for_any_key();
        tui_delete_win(w);
}


int tui_confirm(char *msg, int color, int bkcolor) {
        WINDOW_T *w;
        int c;

        w = tui_draw_message(msg, color, bkcolor);
        c = tui_wait_for_key("yYnN");
        tui_delete_win(w);

        return ((c == 'y') || (c == 'Y')) ? G_TRUE : G_FALSE;
}


int tui_option(char *msg, char *options, int color, int bkcolor) {
        WINDOW_T *w;
        int c;

        w = tui_draw_message(msg, color, bkcolor);
        c = tui_wait_for_key(options);
        tui_delete_win(w);

        return c;
}


void tui_set_attr(int blink, int color, int bkcolor) {
        textattr(color + (bkcolor << 4) + blink);
}


/* Static functions */

WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor) {
        WINDOW_T *w;
        int x, y, size_x, size_y;
        int i, len;
        char *p;

        assert(msg != NULL);

        size_x = 0;
        size_y = 1;
        i = 0;
        p = msg;
        while (*p != '\0') {
                if (*p == '\n') {
                        size_y++;
                        i = 0;
                } else {
                        i++;
                        if (i > size_x) {
                                size_x = i;
                        }
                }
                p++;
        }


        size_x += 4;
        size_y += 2;

        x = (TUI_SCR_X_SIZE - size_x) / 2;
        y = (TUI_SCR_Y_SIZE - size_y) / 2;
        w = tui_create_win(x, y, size_x, size_y, color, bkcolor, ' ');

        /* 1st line */
        gotoxy(x, y);
        putch(0xc9);
        for (i = 0; i < (size_x - 2); i++) {
                putch(0xcd);
        }
        putch(0xbb);

        /* n-th line */
        p = msg;
        for (i = 0; i < (size_y - 2); i++) {
                gotoxy(x, y + i + 1);
                cprintf("%c ", 0xba);
                len = size_x - 4; 
		while ((*p != '\n') && (*p != '\0')) {
                        putch(*p);
                        p++;
                        len--;
                }
                while (len != 0) {
                        putch(' ');
                        len--;
                }
                cprintf(" %c", 0xba);
                p++;
        }

        /* last line */
	gotoxy(x, y + size_y - 1);
        putch(0xc8);
        for (i = 0; i < (size_x - 2); i++) {
                putch(0xcd);
        }
        putch(0xbc);

        tui_flush();
        return w;
}


int tui_wait_for_key(char *s) {
        int c;

        while (1) {
                while (kbhit()) {
                        c = getch();
                        if (strchr(s, c) != NULL)
                                return c;
                }
                /* delay(100); */
        }
}


void tui_wait_for_any_key() {
        while (kbhit()) {
                getch();
        }

        while (!kbhit()) {
                delay(100);
        }
        getch();
}
