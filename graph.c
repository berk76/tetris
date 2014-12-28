/*
*	graph.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/


#include <stdio.h>
#include <stdlib.h>
#include "multi.h"
#include "graph.h"

static G_POSITION origin;

static int MESH_HEIGHT;
static int MESH_WIDTH;
static int MESH_BK_COLOR;
static int MESH_COLOR;
static int GRID_SIZE;


static void g_print_controls();

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
	m_init_graph();
}

void
g_close()
{
	m_close_graph();
}


void
g_draw_mesh(int height, int width)
{
	m_clear_screen();

	MESH_COLOR = M_BLUE;
	MESH_HEIGHT = height;
	MESH_WIDTH = width;
	MESH_BK_COLOR = M_BLACK;
	GRID_SIZE = (m_get_max_y() - 20) / height;

	origin.x = m_get_max_x()/2 - MESH_WIDTH/2 * GRID_SIZE;
	origin.y = (m_get_max_y() - 20) /2 - MESH_HEIGHT/2 * GRID_SIZE;

	m_setcolor(M_WHITE);
	m_settextsize(2);
	m_outtextxy(0, origin.y, "Tetris");
	m_settextsize(1);

	m_setcolor(MESH_COLOR);
	m_setbkcolor(MESH_BK_COLOR);

	m_rectangle(origin.x - 1,
		origin.y - 1,
		origin.x + GRID_SIZE * MESH_WIDTH + 1,
		origin.y + GRID_SIZE * MESH_HEIGHT + 1);

	g_print_controls();
	g_update_score(0);
}

void
g_print_controls()
{
	int x, y;

	x = 1;
	y = origin.y + 4 * 8;
	m_setcolor(M_LIGHTGRAY);
	m_outtextxy(x, y, "Controls:");
	y += 10;
	m_outtextxy(x, y, " ");
	y += 10;
	m_outtextxy(x, y, "Left   ... 7");
	y += 10;
	m_outtextxy(x, y, "Right  ... 9");
	y += 10;
	m_outtextxy(x, y, "Rotate ... 8");
	y += 10;
	m_outtextxy(x, y, "Drop   ... 4");
	y += 10;
	m_outtextxy(x, y, " ");
	y += 10;
	m_outtextxy(x, y, "Pause  ... P");
	y += 10;
	m_outtextxy(x, y, "Quit   ... Q");
}

void
g_update_score(int score)
{
	int x, y, s;
	char str[100];

	str[0] = '\0';
	x = 1;
	y = origin.y + 15;
	//m_setcolor(M_BLACK);
	//g_fill_rect(x, y, 100, 8, M_BLACK);
	//m_setcolor(M_WHITE);
	//sprintf(str, "Score: %d", score);
	itoa(score, str, 10);
	m_outtextxy(x, y, "Score: ");
	m_unfill_rect(x + 30, y, 20, 10);
	m_outtextxy(x + 30, y, str);
}

void
g_put_mesh_pixel(int x, int y, int color)
{
	int i, j;

	m_setcolor(color);

	/*
	m_fill_rect(origin.x + x * GRID_SIZE, origin.y + y * GRID_SIZE,
		GRID_SIZE, GRID_SIZE);
	*/

	x = origin.x + x * GRID_SIZE;
	y = origin.y + y * GRID_SIZE;

	for (i = x+1; i < x + GRID_SIZE-1; i++)
		m_putpixel(i, y);
	
	for (j = y+1; j < y + GRID_SIZE-1; j++) {
		m_putpixel(x, j);
		m_putpixel(x + GRID_SIZE-1, j);
	}

	for (i = x+1; i < x + GRID_SIZE-1; i++)
		m_putpixel(i, y + GRID_SIZE-1);

}


void
g_empty_mesh_pixel(int x, int y)
{
	int i, j;

	m_setcolor(MESH_BK_COLOR);

	/*
	m_unfill_rect(origin.x + x * GRID_SIZE, origin.y + y * GRID_SIZE,
		GRID_SIZE, GRID_SIZE);
	*/

	x = origin.x + x * GRID_SIZE;
	y = origin.y + y * GRID_SIZE;

	for (i = x+1; i < x + GRID_SIZE-1; i++)
		m_unputpixel(i, y);

	for (j = y+1; j < y + GRID_SIZE-1; j++) {
		m_unputpixel(x, j);
		m_unputpixel(x + GRID_SIZE-1, j);
	}

	for (i = x+1; i < x + GRID_SIZE-1; i++)
		m_unputpixel(i, y + GRID_SIZE-1);
}


int
g_is_mesh_pixel_free(int x, int y)
{
	int result;

	result = G_TRUE;

	if (m_getpixel(origin.x + x * GRID_SIZE + 1, \
            origin.y + y * GRID_SIZE + 0) != 0)
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
			color = m_getpixel(origin.x + x * GRID_SIZE + 1, \
				origin.y + (y - 1) * GRID_SIZE + 0);
		} else {
			color = MESH_BK_COLOR;
		}

		if (color != 0) {
			g_put_mesh_pixel(x, y, color);
			empty = G_FALSE;
		} else {
			g_empty_mesh_pixel(x, y);
		}
	}

	return empty;
}


void
g_fill_rect(int x, int y, int width, int height, int color)
{
	m_setcolor(color);
	m_fill_rect(x, y, width, height);
}


/*
int
g_printf(int *xloc, int *yloc, char *fmt, ...)
{
	va_list argptr;
	char str[140];
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(str, fmt, argptr);
	m_outtextxy(*xloc, *yloc, str);
	*yloc += m_gettextsize() * 8 + 2;
	va_end(argptr);

	return cnt;
}
*/

