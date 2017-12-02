/*
*       tui_con.c
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 2.12.2017
*/


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tui_con.h"


static const char FG_COLORS[][5] = {"30","34","32","36","31","35","33","37","1;30","1;34","1;32","1;36","1;31","1;35","1;33","1;37"};
static const char BG_COLORS[][5] = {"40","44","42","46","41","45","43","47","100", "104", "102", "106", "101" ,"105" ,"103", "107"};


/* External functions */

WINDOW_T *tui_create_win(int x, int y, int size_x, int size_y, int color, int bkcolor, char bkchar) {
        WINDOW_T *w;

        w = (WINDOW_T *) malloc(sizeof(WINDOW_T));
        assert(w != NULL);

        w->x = x;
        w->y = y;
        w->size_x = size_x;
        w->size_y = size_y;
        w->color = color;
        w->bkcolor = bkcolor;
        w->bkchar = bkchar;

        tui_cls_win(w, TRUE);

        return w;
}


void tui_delete_win(WINDOW_T *w) {

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
                tui_gotoxy(w->x, w->y + a);
                for (b = 0; b < w->size_x; b++) {
                        putchar(w->bkchar);
                }
        }
        tui_flush();

}


void tui_flush(void) {
        tui_gotoxy(1,22);
        printf("\n");
}


void tui_draw_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border) {

}


void tui_del_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border) {

}


void tui_message(char *msg, int color, int bkcolor) {

}


G_BOOL_T tui_confirm(char *msg, int color, int bkcolor) {
        return FALSE;
}


int tui_option(char *msg, char *options, int color, int bkcolor) {
        return 0;
}


void tui_input(char *msg, char *buff, size_t len, int color, int bkcolor) {

}


void tui_set_attr(int blink, int color, int bkcolor) {
        printf("%c[%s;%sm", ESC, FG_COLORS[color], BG_COLORS[bkcolor]);
}


/* Static functions */


