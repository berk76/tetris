/*
*       tui_con.h
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


#ifndef _TUI_CON_
#define _TUI_CON_


#define ESC 27
#define TUI_SCR_X_SIZE 80
#define TUI_SCR_Y_SIZE 25
#define TUI_ATTR_LEADING 0x01


typedef enum {
        BLACK,			/* dark colors */
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,		/* light colors */
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE
} COLOR_T;


typedef enum {
        FALSE,
        TRUE
} G_BOOL_T;


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


extern void tui_init();
extern void tui_cleanup();

extern WINDOW_T *tui_create_win(int x, int y, int size_x, int size_y, int color, int bkcolor, char bkchar);
extern void tui_delete_win(WINDOW_T *w);
extern void tui_cls_win(WINDOW_T *w, G_BOOL_T incl_status_line);
extern void tui_flush(void);
#define tui_gotoxy(x, y) printf("%c[%u;%uH",27,(y),(x))

extern void tui_draw_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border);
extern void tui_del_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border);

extern void tui_message(char *msg, int color, int bkcolor);
extern G_BOOL_T tui_confirm(char *msg, int color, int bkcolor);
extern int tui_option(char *msg, char *options, int color, int bkcolor);
extern void tui_input(char *msg, char *buff, size_t len, int color, int bkcolor);

extern void tui_set_attr(int blink, int color, int bkcolor);
extern int tui_getk();

#endif

