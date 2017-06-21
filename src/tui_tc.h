/*
*       tui_tc.h
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


#ifndef _TUI_TC_
#define _TUI_TC_


#define G_FALSE 0
#define G_TRUE  1


#define TUI_SCR_X_SIZE 80
#define TUI_SCR_Y_SIZE 25

typedef struct {
        int x;
        int y;
        int size_x;
        int size_y;
        int color;
        int bkcolor;
        char bkchar;
        char *old_content;
} WINDOW_T;


extern WINDOW_T *tui_create_win(int x, int y, int size_x, int size_y, int color, int bkcolor, char bkchar);
extern void tui_delete_win(WINDOW_T *w);
extern void tui_cls_win(WINDOW_T *w);
extern void tui_flush();

extern void tui_message(char *msg, int color, int bkcolor);
extern int tui_confirm(char *msg, int color, int bkcolor);
extern int tui_option(char *msg, char *options, int color, int bkcolor);
extern void tui_set_attr(int blink, int color, int bkcolor);

#endif
