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
#include "tui_con.h"


typedef enum {
        NB_DISABLE = 0,
        NB_ENABLE = 1
} NBLOCK_T;


typedef struct {
        int FG_COLOR;
        int BG_COLOR;
} ATTR_T;


static ATTR_T curr_attr;
static const char FG_COLORS[][5] = {"30","34","32","36","31","35","33","37","1;30","1;34","1;32","1;36","1;31","1;35","1;33","1;37"};
static const char BG_COLORS[][5] = {"40","44","42","46","41","45","43","47","100", "104", "102", "106", "101" ,"105" ,"103", "107"};


static WINDOW_T *tui_draw_message(char *msg, int color, int bkcolor);
static void calc_box_size(int *size_x, int *size_y, char *content);
static void draw_box(int x, int y, int size_x, int size_y, char * content, G_BOOL_T add_border);
static void del_box(int x, int y, int size_x, int size_y);
static int tui_wait_for_key(char *s);
static void tui_wait_for_any_key(void);
static int get_attribute(void);
static void textattr(int color);
static int tui_kbhit();
static void tui_nonblock(NBLOCK_T state);


/* External functions */


void tui_init() {
        tui_nonblock(NB_ENABLE);
}


void tui_cleanup() {
        printf("%c[m",27); //set default text attributes
        printf("%c[2J",27); // erases the screen with the background colour and moves the cursor to home.
        tui_nonblock(NB_DISABLE);
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

        draw_box(x, y, size_x, size_y, s, TRUE);

        /* read input */
        p = buff;
        *p = '\0';
                
        do {
                while ((c = tui_getk()) != 0) {

                        switch (c) {
                                case 13:
                                        /* enter */
                                        break;
                                case 8:
                                        /* backspace */
                                        if (p > buff) {
                                                p--;
                                                *p = '\0';
                                                tui_gotoxy(x + 2, y + 3);
                                                tui_set_attr(0, WHITE, bkcolor);
                                                printf("%s ", buff);
                                                tui_flush();
                                        }
                                        break;
                                default:
                                        /* letter */
                                        if ((c > 31) && ((p - buff) < (len - 1))) {
                                                *p = c;
                                                p++;
                                                *p = '\0';
                                                tui_gotoxy(x + 2, y + 3);
                                                tui_set_attr(0, WHITE, bkcolor);
                                                printf("%s", buff);
                                                tui_flush();
                                        }
                        }
                }
                //w_wait(2);
        } while (c != 13);
        tui_delete_win(w);
}


void tui_set_attr(int blink, int color, int bkcolor) {
        assert((color > -1) && (color < 16));
        assert((bkcolor > -1) && (bkcolor < 16));
        curr_attr.FG_COLOR = color;
        curr_attr.BG_COLOR = bkcolor;
        printf("%c[%s;%sm", ESC, FG_COLORS[color], BG_COLORS[bkcolor]);
}


int tui_getk() {
        int result;

        result = 0;

        if (tui_kbhit()) {
                result = getchar();
        }

        return result;
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
        int orig_a = 0, curr_a = 0;
        char *p;

        /* 1st line */
        if (add_border == TRUE) {
                orig_a = get_attribute();
                tui_gotoxy(x, y);
                putchar('+');
                for (i = 0; i < (size_x - 2); i++) {
                        putchar('-');
                }
                putchar('+');
                offx = 2;
                offy = 1;
        } else {
                offx = 0;
                offy = 0;
        }

        /* n-th line */
        p = content;
        for (i = 0; i < (size_y - 2*offy); i++) {
                tui_gotoxy(x, y + i + offy);
                if (add_border == TRUE) {
                        curr_a = get_attribute();
                        textattr(orig_a); 
                        printf("%c ", '|');
                        textattr(curr_a);
                }
                len = size_x - 2*offx;
                 
		while ((*p != '\n') && (*p != '\0')) {
                        if (*p == TUI_ATTR_LEADING) {
                                p++;
                                textattr(*p);
                                p++;
                                continue;
                        }
                        putchar(*p);
                        p++;
                        len--;
                }
                while (len != 0) {
                        putchar(' ');
                        len--;
                }
                if (add_border == TRUE) {
                        curr_a = get_attribute();
                        textattr(orig_a);
                        printf(" %c", '|');
                        textattr(curr_a);
                }
                p++;
        }
        
        /* last line */
        if (add_border == TRUE) {
                textattr(orig_a);
                tui_gotoxy(x, y + size_y - 1);
                putchar('+');
                for (i = 0; i < (size_x - 2); i++) {
                        putchar('-');
                }
                putchar('+');
        }

        tui_flush();
}


void del_box(int x, int y, int size_x, int size_y) {
        int n, m;
        
        for(n = 0; n < size_y; n++) {
                for (m = 0; m < size_x; m++) {
                        tui_gotoxy(x + m, y + n);
                        putchar(' ');
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
        return curr_attr.FG_COLOR;
}


void textattr(int color) {
        assert((color > -1) && (color < 16));
        curr_attr.FG_COLOR = color;
        printf("%c[%sm", ESC, FG_COLORS[color]);
}


int tui_kbhit() {
        struct timeval tv;
        fd_set fds;

        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
        select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &fds);
}


void tui_nonblock(NBLOCK_T state) {
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

