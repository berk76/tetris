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
#include "wait_tc.h"
#include "tui_tc.h"


static WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor);
static void calc_box_size(int *size_x, int *size_y, char *content);
static void draw_box(int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border);
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

        tui_cls_win(w, TRUE);

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


void tui_cls_win(WINDOW_T *w, G_BOOL_T incl_status_line) {
        int a, b, c;
        
        if (incl_status_line == FALSE) {
                c = 1;
        } else {
                c = 0;
        }

        tui_set_attr(0, w->color, w->bkcolor);
        for (a = 0; a < (w->size_y - c); a++) {
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


void tui_draw_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border) {
        int size_x, size_y;

        assert(msg != NULL);
        tui_set_attr(0, color, bkcolor);
        calc_box_size(&size_x, &size_y, msg);
        
        if (add_border == TRUE) {
                size_x += 4;
                size_y += 2;
        }
        
        draw_box(x, y, size_x, size_y, msg, add_border);
}


void tui_message(char *msg, int color, int bkcolor) {
        WINDOW_T *w;

        w = tui_draw_message(msg, color, bkcolor);
        tui_wait_for_any_key();
        tui_delete_win(w);
}


G_BOOL_T tui_confirm(char *msg, int color, int bkcolor) {
        WINDOW_T *w;
        int c;

        w = tui_draw_message(msg, color, bkcolor);
        c = tui_wait_for_key("yYnN");
        tui_delete_win(w);

        return ((c == 'y') || (c == 'Y')) ? TRUE : FALSE;
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
        textattr(calcattr(blink, color, bkcolor));
}


/* Static functions */

WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor) {
        WINDOW_T *w;
        int x, y, size_x, size_y;

        assert(msg != NULL);

        calc_box_size(&size_x, &size_y, msg);

        size_x += 4;
        size_y += 2;

        x = (TUI_SCR_X_SIZE - size_x) / 2;
        y = (TUI_SCR_Y_SIZE - size_y) / 2;
        w = tui_create_win(x, y, size_x, size_y, color, bkcolor, ' ');

        draw_box(x, y, size_x, size_y, msg, TRUE);
        return w;
}


void calc_box_size(int *size_x, int *size_y, char *content) {
        char *p;
        int i;
        
        assert(content != NULL);
        
        *size_x = 0;
        *size_y = 1;
        i = 0;
        p = content;
        while (*p != '\0') {
                if (*p == TUI_ATTR_LEADING) {
                        p += 2;
                        continue;
                }
                
                if (*p == '\n') {
                        (*size_y)++;
                        i = 0;
                } else {
                        i++;
                        if (i > *size_x) {
                                *size_x = i;
                        }
                }
                p++;
        }
}


void draw_box(int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border) {
        int i, len, offx, offy;
        char *p;

        /* 1st line */
        if (add_border == TRUE) {
                gotoxy(x, y);
                putch('+');
                for (i = 0; i < (size_x - 2); i++) {
                        putch('-');
                }
                putch('+');
                offx = 2;
                offy = 1;
        } else {
                offx = 0;
                offy = 0;
        }

        /* n-th line */
        p = content;
        for (i = 0; i < (size_y - 2*offy); i++) {
                gotoxy(x, y + i + offy);
                if (add_border == TRUE) { 
                        cprintf("%c ", '|');
                }
                len = size_x - 2*offx;
                 
		while ((*p != '\n') && (*p != '\0')) {
                        if (*p == TUI_ATTR_LEADING) {
                                p++;
                                textattr(*p);
                                p++;
                                continue;
                        }
                        putch(*p);
                        p++;
                        len--;
                }
                while (len != 0) {
                        putch(' ');
                        len--;
                }
                if (add_border == TRUE) {
                        cprintf(" %c", '|');
                }
                p++;
        }
        
        /* last line */
        if (add_border == TRUE) {
                gotoxy(x, y + size_y - 1);
                putch('+');
                for (i = 0; i < (size_x - 2); i++) {
                        putch('-');
                }
                putch('+');
        }

        tui_flush();
}


int tui_wait_for_key(char *s) {
        int c;

        while (1) {
                while (kbhit()) {
                        c = getch();
                        if (strchr(s, c) != NULL)
                                return c;
                }
                w_wait(100);
        }
}


void tui_wait_for_any_key() {
        while (kbhit()) {
                getch();
        }

        while (!kbhit()) {
                w_wait(100);
        }
        getch();
}
