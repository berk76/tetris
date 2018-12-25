/*
*       gui_tc.c
*       
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 28.6.2014
*/


#include <stdlib.h>
#include <graphics.h>
#include "gui_tc.h"


#define MSG_BORDER 20
#define MSG_BK_COLOR GREEN
#define MSG_TXT_COLOR WHITE


typedef struct {
	int x;
	int y;
} G_POSITION;


typedef struct {
	int x;
	int y;
	int width;
	int height;
	void *buff;
} GUI_CNTL;


static GUI_CNTL *gui_draw_message(char *msg);
static void gui_delete_message(GUI_CNTL *cntl);
static int gui_wait_for_key(char *s);
static int gui_wait_for_any_key();


void  gui_message(char *msg) {
	GUI_CNTL *cntl;

	cntl = gui_draw_message(msg);
	gui_wait_for_any_key();
	gui_delete_message(cntl);
}


int gui_confirm(char *msg) {
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key("yYnN");
	gui_delete_message(cntl);

	return ((c == 'y') || (c == 'Y')) ? G_TRUE : G_FALSE;
}


int gui_option(char *msg, char *options) {
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key(options);
	gui_delete_message(cntl);

	return c;
}


GUI_CNTL *gui_draw_message(char *msg) {
	GUI_CNTL *result;
	struct textsettingstype t_type;
	int x, y, width, height;
	unsigned size;

	result = (GUI_CNTL *) malloc(sizeof(GUI_CNTL));

	gettextsettings(&t_type);
	result->width = 2 * MSG_BORDER + t_type.charsize * 8 * strlen(msg);
	result->height = 2 * MSG_BORDER + t_type.charsize * 8;

	result->x = getmaxx() / 2 - result->width / 2;
	result->y = getmaxy() / 2 - result->height / 2;

	size = imagesize(result->x, result->y, 
		result->x + result->width, result->y + result->height);
	result->buff = malloc(size);
	getimage(result->x, result->y, 
		result->x + result->width, result->y + result->height, result->buff);
        
	gui_fill_rect(result->x, result->y, result->width, result->height, 
		MSG_BK_COLOR, SOLID_FILL);
	setcolor(MSG_TXT_COLOR);
	outtextxy(result->x + MSG_BORDER, result->y + MSG_BORDER + t_type.vert / 2, msg);

	return result;
}


void gui_delete_message(GUI_CNTL *cntl) {
	putimage(cntl->x, cntl->y, cntl->buff, COPY_PUT);
	free((void *) cntl->buff);
	free((void *) cntl);
}


int gui_wait_for_key(char *s) {
	int c;

	while (1) {
		while (kbhit()) {
			c = getch();
			if (strchr(s, c) != NULL)
				return c;
		}
		delay(100);
	}
}


int gui_wait_for_any_key() {
	while (kbhit()) {
		getch();
	}

	while (!kbhit()) {
		delay(100);
	}
	getch();
}


void gui_fill_rect(int x, int y, int width, int height, int color, int fill_type) {
	G_POSITION vert[4];
	vert[0].x = x;
	vert[0].y = y;
	vert[1].x = x + width;
	vert[1].y = y;
	vert[2].x = x + width;
	vert[2].y = y + height;
	vert[3].x = x;
	vert[3].y = y + height;

	setfillstyle(fill_type, color);
	fillpoly(4, (int *) vert);
}
