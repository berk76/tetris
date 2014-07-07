/*
*	guicntls.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/

#include <stdlib.h>
#include <graphics.h>
#include "graph.h"
#include "guicntls.h"


#define MSG_BORDER 20
#define MSG_BK_COLOR GREEN


GUI_CNTL *
gui_show_message(char *msg)
{
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

	fill_rect(result->x, result->y, result->width, result->height, 
		MSG_BK_COLOR, SOLID_FILL);
	outtextxy(result->x + MSG_BORDER, result->y + MSG_BORDER + t_type.vert / 2, msg);

	return result;
}

void 
gui_delete_message(GUI_CNTL *cntl)
{
	putimage(cntl->x, cntl->y, cntl->buff, COPY_PUT);
	free((void *) cntl->buff);
	free((void *) cntl);
}

int
gui_wait_for_key(char *s)
{
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

int
gui_wait_for_any_key()
{
	while (kbhit()) {
		getch();
	}

	while (!kbhit()) {
		delay(100);
	}
	getch();
}



