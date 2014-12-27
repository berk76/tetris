/*
*	multi.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	12.10.2014
*	redistributable under the terms of the GNU/GPLv3
*/


#include <stdio.h>
#include <conio.h>
#include <bgi.h>
#include <malloc.h>
#include "multi.h"


extern long heap(60000);

static int graph_driver;
static int graph_mode;
static int error_code;


void 
m_initialize(void)
{
        mallinit();              /* heap cleared to empty */
        sbrk(30000,2000);        /* add 2000 bytes from addresses 30000-31999 inclusive to the heap */
        sbrk(65000,536);         /* add 536 bytes from addresses 65000-65535 inclusive to the heap  */
}


int 
m_init_graph(void)
{
	/*
	graph_driver = DETECT;
	initgraph(&graph_driver, &graph_mode, "");
	error_code = graphresult();
	if (error_code != grOk) {
		printf("Graphics system error: %s\n", grapherrormsg(error_code));
		exit(1);
	}
	*/
	initgraph(0,0,0);
	return 0;
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
m_putpixel(int x, int y)
{
	plot(x, y);
}

void 
m_unputpixel(int x, int y)
{
	unplot(x, y);
}

int 
m_getpixel(int x, int y)
{
	return point(x, y);
}


void 
m_clear_screen(void)
{
	cleardevice();
}


int 
m_kbhit(void)
{
	//return (kbhit()) ? getch() : 0;
	return getk();
}


void 
m_delay(unsigned milliseconds)
{
	delay(milliseconds);
}


void 
m_settextsize(int charsize)
{
	/* always 1 */
}


int m_gettextsize(void)
{
	return 1;
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


void
m_fill_rect(int x, int y, int width, int height)
{
	int i, j;

	for (i = x; i < x + width; i++)
		for (j = y; j < y + height; j++)
			plot(i, j);

}

void 
m_unfill_rect(int x, int y, int width, int height)
{
	int i, j;
	for (i = x; i < x + width; i++)
		for (j = y; j < y + height; j++)
			unplot(i, j);
}




unsigned 
m_imagesize(int left, int top, int right, int bottom)
{
	/*
	int w, h;

	w = right - left + 1;
	h = bottom - top + 1;

	return (w * h + 2) * sizeof(unsigned char);
	*/
	return 0;
}


void 
m_getimage(int left, int top, int right, int bottom, void *bitmap)
{
	/*
	int w, h;
	unsigned char *b;
	int i, j;

	w = right - left + 1;
	h = bottom - top + 1;
	b = (unsigned char *) bitmap;

	*b = (unsigned char) w;
	b++;
	*b = (unsigned char) h;
	b++;

	for (i = left; i < left + w; i++)
		for (j = top; j < top + h; j++) {
			*b = (unsigned char) point(i,j);
			b++;
		}
	*/
}


void 
m_putimage(int left, int top, void *bitmap)
{
	/*
	int w, h;
	unsigned char *b;
	int i, j;

	b = (unsigned char *) bitmap;
	w = *b;
	b++;
	h = *b;
	b++;

	for (i = left; i < left + w; i++)
		for (j = top; j < top + h; j++) {
			(*b != 0) ? plot(i,j) : unplot(i,j);
			b++;
		}
	*/
}

