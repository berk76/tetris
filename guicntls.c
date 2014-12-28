/*
*	guicntls.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "multi.h"
#include "guicntls.h"


#define MSG_BORDER 20
#define MSG_BK_COLOR M_GREEN
#define MSG_TXT_COLOR M_WHITE


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


void
gui_message(char *msg)
{
	GUI_CNTL *cntl;

	cntl = gui_draw_message(msg);
	gui_wait_for_any_key();
	gui_delete_message(cntl);
}

int
gui_confirm(char *msg)
{
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key("yYnN");
	gui_delete_message(cntl);

	return ((c == 'y') || (c == 'Y')) ? G_TRUE : G_FALSE;
}

int gui_option(char *msg, char *options)
{
	GUI_CNTL *cntl;
	int c;

	cntl = gui_draw_message(msg);
	c = gui_wait_for_key(options);
	gui_delete_message(cntl);

	return c;
}

#ifdef __TURBOC__

GUI_CNTL *
gui_draw_message(char *msg)
{
	GUI_CNTL *result;
	int x, y, width, height;
	unsigned size;

	result = (GUI_CNTL *) malloc(sizeof(GUI_CNTL));

	result->width = 2 * MSG_BORDER + m_gettextsize() * 8 * strlen(msg);
	result->height = 2 * MSG_BORDER + m_gettextsize() * 8;

	result->x = m_get_max_x() / 2 - result->width / 2;
	result->y = m_get_max_y() / 2 - result->height / 2;

	size = m_imagesize(result->x, result->y,
		result->x + result->width, result->y + result->height);
	result->buff = malloc(size);
	m_getimage(result->x, result->y,
		result->x + result->width, result->y + result->height, result->buff);

	m_rectangle(result->x, result->y, result->x + result->width, result->y + result->height);
	g_fill_rect(result->x, result->y, result->width, result->height,
		MSG_BK_COLOR);
	m_setcolor(MSG_TXT_COLOR);
	m_outtextxy(result->x + MSG_BORDER, result->y + MSG_BORDER + m_gettextsize() * 8 / 2, msg);

	return result;
}

void 
gui_delete_message(GUI_CNTL *cntl)
{
	m_putimage(cntl->x, cntl->y, cntl->buff);
	free((void *) cntl->buff);
	free((void *) cntl);
}

#else

GUI_CNTL *
gui_draw_message(char *msg)
{
	GUI_CNTL *result;
	result = (GUI_CNTL *) malloc(sizeof(GUI_CNTL));

	result->x = 1;
	result->y = 180;
	result->width = 200;
	result->height = 10;
	
	//m_rectangle(result->x, result->y, result->x + result->width, result->y + result->height);
	m_outtextxy(result->x, result->y, msg);

	return result;
}

void 
gui_delete_message(GUI_CNTL *cntl)
{
	m_unfill_rect(cntl->x, cntl->y, cntl->width, cntl->height);
	free((void *) cntl);
}

#endif

int
gui_wait_for_key(char *s)
{
	int c;

	while (1) {
		while ((c = m_kbhit()) != 0) {
			if (strchr(s, c) != NULL)
				return c;
		}
		m_delay(100);
	}
}

int
gui_wait_for_any_key()
{
	while (m_kbhit()) {
	}

	while (!m_kbhit()) {
		m_delay(100);
	}
}



