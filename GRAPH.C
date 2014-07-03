/*
*	graph.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <graphics.h>
#include "graph.h"

static G_POSITION origin;
static int graph_driver;
static int graph_mode;
static int error_code;

static int MESH_HEIGHT;
static int MESH_WIDTH;
static int MESH_BK_COLOR;
static int GRID_SIZE;


int
g_mesh_height()
{
	return MESH_HEIGHT;
}

int
g_mesh_width()
{
	return MESH_WIDTH;
}

void
g_initialize()
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
g_close()
{
	closegraph();
}

void
g_report_status()
{
	char *driver, *mode;
	int x, y;

	x = 10;
	y = 4;

	driver = getdrivername();
	mode = getmodename(graph_mode);

	g_printf( &x, &y, "Graphics device    : %-20s (%d)", driver, graph_driver );
	g_printf( &x, &y, "Graphics mode      : %-20s (%d)", mode, graph_mode );
	g_printf( &x, &y, "Screen resolution  : ( 0, 0, %d, %d )", getmaxx(), getmaxy() );
	g_printf( &x, &y, "Colors available   : %d", getmaxcolor() + 1 );
}

void
g_draw_mesh(int height, int width, int grid_size, int bk_color)
{
	int i;

	MESH_HEIGHT = height;
	MESH_WIDTH = width;
	MESH_BK_COLOR = bk_color;
	GRID_SIZE = grid_size;


	line(getmaxx()/2, 0, getmaxx()/2, getmaxy());
	line(0, getmaxy()/2, getmaxx(), getmaxy()/2);

	origin.x = getmaxx()/2 - MESH_WIDTH/2 * GRID_SIZE;
	origin.y = getmaxy()/2 - MESH_HEIGHT/2 * GRID_SIZE;

	setcolor(BLUE);
	setbkcolor(MESH_BK_COLOR);
	moveto(origin.x, origin.y);

	for (i = 0; i <= MESH_WIDTH; i++) {
		linerel(0, MESH_HEIGHT * GRID_SIZE);
		moverel(GRID_SIZE, -MESH_HEIGHT * GRID_SIZE);
	}

	moveto(origin.x, origin.y);
	for (i = 0; i <= MESH_HEIGHT; i++) {
		linerel(MESH_WIDTH * GRID_SIZE, 0);
		moverel(-MESH_WIDTH * GRID_SIZE, GRID_SIZE);
	}
}

void
g_put_mesh_pixel(int x, int y, int color)
{
	G_POSITION vert[4];

	vert[0].x = origin.x + x * GRID_SIZE;
	vert[0].y = origin.y + y * GRID_SIZE;
	vert[1].x = vert[0].x + GRID_SIZE;
	vert[1].y = vert[0].y;
	vert[2].x = vert[0].x + GRID_SIZE;
	vert[2].y = vert[0].y + GRID_SIZE;
	vert[3].x = vert[0].x;
	vert[3].y = vert[0].y + GRID_SIZE;

	setfillstyle(SOLID_FILL, color);
	fillpoly(4, (int *) vert);
}


void
g_empty_mesh_pixel(int x, int y)
{
	g_put_mesh_pixel(x, y, MESH_BK_COLOR);
}


int
g_is_mesh_pixel_free(int x, int y)
{
	int result;

	result = G_TRUE;

	if (getpixel(origin.x + x * GRID_SIZE + 1, \
            origin.y + y * GRID_SIZE + 1) != MESH_BK_COLOR)
		result = G_FALSE;

	if ((x < 0) || (x >= MESH_WIDTH))
		result = G_FALSE;

	if ((y < 0) || (y >= MESH_HEIGHT))
		result = G_FALSE;

	return result;
}

int 
g_copy_upper_line(int y)
{
	int x, color, empty;

	empty = G_TRUE;
	for (x = 0; x < MESH_WIDTH; x++) {
		if (y != 0) {
			color = getpixel(origin.x + x * GRID_SIZE + 1, \
		    		origin.y + (y - 1) * GRID_SIZE + 1);
		} else {
			color = MESH_BK_COLOR;
		}

		g_put_mesh_pixel(x, y, color);

		if (color != MESH_BK_COLOR)
			empty = G_FALSE;
	}

	return empty;
}

int
g_printf(int *xloc, int *yloc, char *fmt, ...)
{
	va_list argptr;
	char str[140];
	int cnt;

	va_start(argptr, format);
	cnt = vsprintf(str, fmt, argptr);
	outtextxy(*xloc, *yloc, str);
	*yloc += textheight("H") + 2;
	va_end(argptr);

	return cnt;
}

