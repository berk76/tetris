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
static int MESH_COLOR;
static int GRID_SIZE;


static void g_print_controls();
static void fill_rect(int x, int y, int width, int height, int color, int fill_type);


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

	cleardevice();

	MESH_COLOR = BLUE;
	MESH_HEIGHT = height;
	MESH_WIDTH = width;
	MESH_BK_COLOR = bk_color;
	GRID_SIZE = grid_size;

	/*
	 *  line(getmaxx()/2, 0, getmaxx()/2, getmaxy());
	 * line(0, getmaxy()/2, getmaxx(), getmaxy()/2);
	 */

	origin.x = getmaxx()/2 - MESH_WIDTH/2 * GRID_SIZE;
	origin.y = getmaxy()/2 - MESH_HEIGHT/2 * GRID_SIZE;

	setcolor(MESH_COLOR);
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

	g_print_controls();
	g_update_score(0);
}

void
g_print_controls()
{
}

void
g_update_score(int score)
{
	int x, y;

	x = origin.x - 200;
	y = origin.y + 8;
	setcolor(BLACK);
	fill_rect(x, y, 100, 8, BLACK, SOLID_FILL);
	setcolor(WHITE);
	g_printf(&x, &y, "Score: %d", score);
}

void
g_put_mesh_pixel(int x, int y, int color)
{
	setcolor(MESH_COLOR);
	fill_rect(origin.x + x * GRID_SIZE, origin.y + y * GRID_SIZE, 
		GRID_SIZE, GRID_SIZE, color, SOLID_FILL);
}


void
g_empty_mesh_pixel(int x, int y)
{
	setcolor(MESH_COLOR);
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

void
g_print_message(char *msg)
{
	#define MSG_BORDER 20
	#define MSG_BK_COLOR GREEN

	G_POSITION vert[4];
	struct textsettingstype t_type;
	int x, y, width, height;

	gettextsettings(&t_type);
	width = 2 * MSG_BORDER + t_type.charsize * 8 * strlen(msg);
	height = 2 * MSG_BORDER + t_type.charsize * 8;

	x = getmaxx() / 2 - width / 2;
	y = getmaxy() / 2 - height / 2;

	fill_rect(x, y, width, height, MSG_BK_COLOR, SOLID_FILL);
	outtextxy(x + MSG_BORDER, y + MSG_BORDER + t_type.vert / 2, msg);
}

void 
fill_rect(int x, int y, int width, int height, int color, int fill_type) 
{
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
