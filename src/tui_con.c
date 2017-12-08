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
#include <termios.h>
#include <unistd.h>
#include "wait_con.h"
#include "tui_con.h"


static COLOR_T curr_attr = -1;


#define tui_gotoxy_w(w, x, y) wmove((w),(y-1),(x-1))
#define textattrw(w, color) tui_set_attr_w((w), 0, (color), 0)
static void tui_set_attr_w(WINDOW *w, int blink, int color, int bkcolor);
static WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor);
static void calc_box_size(int *size_x, int *size_y, char *content);
static void draw_box(int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border);
static void draw_box_w(WINDOW *w, int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border);
static void del_box(int x, int y, int size_x, int size_y);
static int tui_wait_for_key(char *s);
static void tui_wait_for_any_key(void);
static int get_attribute(void);
static int tui_kbhit();


/* External functions */


void tui_init() {
        initscr();
        keypad(stdscr, TRUE);
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        curs_set(0);

        start_color();

        /* Foreground colors */
        init_pair(0, COLOR_BLACK,   COLOR_BLACK);
        init_pair(1, COLOR_BLUE,    COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_CYAN,    COLOR_BLACK);
        init_pair(4, COLOR_RED,     COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);

        init_pair(8,  COLOR_WHITE,   COLOR_BLACK);
        init_pair(9,  COLOR_BLUE,    COLOR_BLACK);
        init_pair(10, COLOR_GREEN,   COLOR_BLACK);
        init_pair(11, COLOR_CYAN,    COLOR_BLACK);
        init_pair(12, COLOR_RED,     COLOR_BLACK);
        init_pair(13, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(14, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(15, COLOR_WHITE,   COLOR_BLACK);


        /* Background colors */
        init_pair(16, COLOR_BLACK, COLOR_BLACK);
        init_pair(17, COLOR_BLACK, COLOR_BLUE);
        init_pair(18, COLOR_BLACK, COLOR_GREEN);
        init_pair(19, COLOR_BLACK, COLOR_CYAN);
        init_pair(20, COLOR_BLACK, COLOR_RED);
        init_pair(21, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(22, COLOR_BLACK, COLOR_YELLOW);
        init_pair(23, COLOR_BLACK, COLOR_WHITE);

        init_pair(24, COLOR_BLACK, COLOR_WHITE);
        init_pair(25, COLOR_BLACK, COLOR_BLUE);
        init_pair(26, COLOR_BLACK, COLOR_GREEN);
        init_pair(27, COLOR_BLACK, COLOR_CYAN);
        init_pair(28, COLOR_BLACK, COLOR_RED);
        init_pair(29, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(30, COLOR_BLACK, COLOR_YELLOW);
        init_pair(31, COLOR_BLACK, COLOR_WHITE);
}


void tui_cleanup() {
        endwin();
}


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

        w->window = newwin(size_y, size_x, y, x);
        box(w->window, 0 , 0);
        wrefresh(w->window);

        tui_cls_win(w, TRUE);

        return w;
}


void tui_delete_win(WINDOW_T *w) {

        if (w->window != NULL) {
                wborder(w->window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
                wrefresh(w->window);
                delwin(w->window);
                w->window = NULL;
        }

        if (w != NULL) {
                free((void *) w);
                w = NULL;
        }

        redrawwin(stdscr);
        refresh();
}


void tui_cls_win(WINDOW_T *w, G_BOOL_T incl_status_line) {
        int a, b, c;
        
        if (incl_status_line == FALSE) {
                c = 1;
        } else {
                c = 0;
        }

        tui_set_attr_w(w->window, 0, w->color, w->bkcolor);
        for (a = 0; a < (w->size_y - c); a++) {
                wmove(w->window, w->x, w->y + a);
                for (b = 0; b < w->size_x; b++) {
                        waddch(w->window, w->bkchar);
                }
        }
        wrefresh(w->window);
        tui_flush();
}


void tui_flush(void) {
        tui_gotoxy(1,25);
        refresh();
}


void tui_draw_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border) {
        int size_x, size_y;

        assert(msg != NULL);
        assert(x > 0);
        assert(y > 0);
        
        tui_set_attr(0, color, bkcolor);
        calc_box_size(&size_x, &size_y, msg);
        
        if (add_border == TRUE) {
                size_x += 4;
                size_y += 2;
        }
        
        draw_box(x, y, size_x, size_y, msg, add_border);
}


void tui_del_box(int x, int y, int color, int bkcolor, char *msg, G_BOOL_T add_border) {
        int size_x, size_y;

        assert(msg != NULL);
        assert(x > 0);
        assert(y > 0);
        
        tui_set_attr(0, color, bkcolor);
        calc_box_size(&size_x, &size_y, msg);
        
        if (add_border == TRUE) {
                size_x += 4;
                size_y += 2;
        }
        
        del_box(x, y, size_x, size_y);
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


void tui_input(char *msg, char *buff, size_t len, int color, int bkcolor) {
        WINDOW_T *w;
        int x, y, size_x, size_y;
        #define SLEN 256
        char s[SLEN];
        char *p;
        int c;
        
        assert(msg != NULL);
        
        /* draw box */
        sprintf(s, "\n%s\n\n", msg);
        assert(strlen(s) < SLEN);
        calc_box_size(&size_x, &size_y, s);

        size_x += 4;
        size_y += 2;

        x = (TUI_SCR_X_SIZE - size_x) / 2;
        y = (TUI_SCR_Y_SIZE - size_y) / 2;
        w = tui_create_win(x, y, size_x, size_y, color, bkcolor, ' ');

        x = 1;
        y = 1;
        draw_box_w(w->window, x, y, size_x, size_y, s, TRUE);

        /* read input */
        p = buff;
        *p = '\0';
                
        do {
                while ((c != '\n') && ((c = tui_getk()) != 0)) {

                        switch (c) {
                                case '\n':
                                        /* enter */
                                        break;
                                case KEY_BACKSPACE:
                                        /* backspace */
                                        if (p > buff) {
                                                p--;
                                                *p = '\0';
                                                tui_gotoxy_w(w->window, x + 2, y + 3);
                                                tui_set_attr_w(w->window, 0, WHITE, bkcolor);
                                                wprintw(w->window, "%s ", buff);
                                                wrefresh(w->window);
                                                tui_flush();
                                        }
                                        break;
                                default:
                                        /* letter */
                                        if ((c > 31) && ((p - buff) < (len - 1))) {
                                                *p = c;
                                                p++;
                                                *p = '\0';
                                                tui_gotoxy_w(w->window, x + 2, y + 3);
                                                tui_set_attr_w(w->window, 0, WHITE, bkcolor);
                                                wprintw(w->window, "%s", buff);
                                                wrefresh(w->window);
                                                tui_flush();
                                        }
                        }
                }
                w_wait(2);
        } while (c != '\n');
        tui_delete_win(w);
}


void tui_set_attr(int blink, int color, int bkcolor) {
        tui_set_attr_w(stdscr, blink, color, bkcolor);
}


int tui_getk() {
        int result;

        result = 0;

        if (tui_kbhit()) {
                result = getch();
        }

        return result;
}


/* Static functions */


void tui_set_attr_w(WINDOW *w, int blink, int color, int bkcolor) {
        assert((color > -1) && (color < 32));
        assert((bkcolor > -1) && (bkcolor < 32));

        //if (curr_attr != -1)
        //        attroff(COLOR_PAIR(curr_attr));

        wattron(w, COLOR_PAIR(color));
        curr_attr = color;
}


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

        draw_box_w(w->window, 1, 1, size_x, size_y, msg, TRUE);
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
        draw_box_w(stdscr, x, y, size_x, size_y, content, add_border);
}


void draw_box_w(WINDOW *w, int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border) {
        int i, len, offx, offy;
        int orig_a = 0, curr_a = 0;
        char *p;

        /* 1st line */
        if (add_border == TRUE) {
                orig_a = get_attribute();
                tui_gotoxy_w(w, x, y);
                waddch(w, '+');
                for (i = 0; i < (size_x - 2); i++) {
                        waddch(w,'-');
                }
                waddch(w, '+');
                offx = 2;
                offy = 1;
        } else {
                offx = 0;
                offy = 0;
        }

        /* n-th line */
        p = content;
        for (i = 0; i < (size_y - 2*offy); i++) {
                tui_gotoxy_w(w, x, y + i + offy);
                if (add_border == TRUE) {
                        curr_a = get_attribute();
                        textattrw(w, orig_a); 
                        wprintw(w, "%c ", '|');
                        textattrw(w, curr_a);
                }
                len = size_x - 2*offx;
                 
		while ((*p != '\n') && (*p != '\0')) {
                        if (*p == TUI_ATTR_LEADING) {
                                p++;
                                textattrw(w, *p);
                                p++;
                                continue;
                        }
                        waddch(w, *p);
                        p++;
                        len--;
                }
                while (len != 0) {
                        waddch(w, ' ');
                        len--;
                }
                if (add_border == TRUE) {
                        curr_a = get_attribute();
                        textattrw(w, orig_a);
                        wprintw(w, " %c", '|');
                        textattrw(w, curr_a);
                }
                p++;
        }
        
        /* last line */
        if (add_border == TRUE) {
                textattrw(w, orig_a);
                tui_gotoxy(x, y + size_y - 1);
                waddch(w, '+');
                for (i = 0; i < (size_x - 2); i++) {
                        waddch(w, '-');
                }
                waddch(w, '+');
        }

        wrefresh(w);
        tui_flush();
}


void del_box(int x, int y, int size_x, int size_y) {
        int n, m;
        
        for(n = 0; n < size_y; n++) {
                for (m = 0; m < size_x; m++) {
                        tui_gotoxy(x + m, y + n);
                        tui_putchar(' ');
                }
        }
        
        tui_flush();
}


int tui_wait_for_key(char *s) {
        int c;

        while (1) {
                while ((c = tui_getk()) != 0) {
                        if (strchr(s, c) != NULL)
                                return c;
                }
                usleep(10000);
        }
}


void tui_wait_for_any_key(void) {
        while (tui_getk()) {
        }

        while (!tui_getk()) {
                usleep(10000);
        }
}


int get_attribute(void) {
        return curr_attr;
}


int tui_kbhit() {
        int ch = getch();

        if (ch != ERR) {
                ungetch(ch);
                return 1;
        }

        return 0;
}

