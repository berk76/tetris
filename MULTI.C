/*
*	multi.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	12.10.2014
*	redistributable under the terms of the GNU/GPLv3
*/


#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
#include "multi.h"


static int graph_driver;
static int graph_mode;
static int error_code;


int 
m_init_graph(void)
{
	graph_driver = DETECT;
	initgraph(&graph_driver, &graph_mode, "");
	error_code = graphresult();
	if (error_code != grOk) {
		printf("Graphics system error: %s\n", grapherrormsg(error_code));
		exit(1);
	}
}


void
m_close_graph(void)
{
	closegraph();
}


int 
m_get_max_x(void)
{
	getmaxx();
}


int 
m_get_max_y(void)
{
	getmaxy();
}


void 
m_setcolor(int color)
{
	setcolor(color);
}


void 
m_setbkcolor(int color)
{
	setbkcolor(color);
}


void 
m_putpixel(int x, int y, int color)
{
	putpixel(x, y, color);
}


int 
m_getpixel(int x, int y)
{
	getpixel(x, y);
}


void 
m_clear_screen(void)
{
	cleardevice();
}


int 
m_kbhit(void)
{
	return (kbhit()) ? getch() : 0;
}


void 
m_delay(unsigned milliseconds)
{
	delay(milliseconds);
}


void 
m_settextsize(int charsize)
{
	settextstyle(DEFAULT_FONT, HORIZ_DIR, charsize);
}


int m_gettextsize(void)
{
	struct textsettingstype t_type;
	gettextsettings(&t_type);
	return t_type.charsize;
}


void 
m_outtextxy(int x, int y, char *s)
{
	outtextxy(x, y, s);
}


void
m_rectangle(int left, int top, int right, int bottom)
{
	rectangle(left, top, right, bottom);
}

struct _position {
	int x;
	int y;
};

void
m_fill_rect(int x, int y, int width, int height, int color)
{
	struct _position vert[4];
	vert[0].x = x;
	vert[0].y = y;
	vert[1].x = x + width;
	vert[1].y = y;
	vert[2].x = x + width;
	vert[2].y = y + height;
	vert[3].x = x;
	vert[3].y = y + height;

	setfillstyle(SOLID_FILL, color);
	fillpoly(4, (int *) vert);
}


unsigned 
m_imagesize(int left, int top, int right, int bottom)
{
	return imagesize(left, top, right, bottom);
}


void 
m_getimage(int left, int top, int right, int bottom, void *bitmap)
{
	getimage(left, top, right, bottom, bitmap);
}


void 
m_putimage(int left, int top, void *bitmap)
{
	putimage(left, top, bitmap, COPY_PUT);
}

