/*
*	tetris.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/


#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include "graph.h"
#include "shape.h"


#define NUM_OF_BRICK_ELEMENTS	4
typedef struct {
	int x;
	int y;
	int color;
	G_POSITION shape[NUM_OF_BRICK_ELEMENTS];
	int shape_size;
} BRICK;


#define COLOR_VEC_SIZE		7
static int color_vec[] = 	{LIGHTBLUE,
    				LIGHTGREEN,
	    			LIGHTCYAN,
    				LIGHTRED,
    				LIGHTMAGENTA,
    				YELLOW,
				WHITE};



static int create_new_brick(BRICK *b);
static int is_free_space_for_brick(BRICK *b);
static void draw_brick(BRICK *b);
static void hide_brick(BRICK *b);
static int move_down(BRICK *b);
static int move_left(BRICK *b);
static int move_right(BRICK *b);
static int rotate(BRICK *b);
static void check_lines();
static int is_line_full(int y);
static void destroy_line(int y);
static void wait(BRICK *b);
static void process_user_input(BRICK *b);


int
t_run()
{
	BRICK brick;

	while (create_new_brick(&brick) != -1) {
		do {
			wait(&brick);
		} while (move_down(&brick) != -1);
		check_lines();
	}

	return 0;
}

int
create_new_brick(BRICK *b)
{
	srand(time(NULL) % 37);

	b->color = color_vec[rand() % COLOR_VEC_SIZE];
	b->shape_size = NUM_OF_BRICK_ELEMENTS;
	b->y = create_new_shape(b->shape, b->shape_size);
	b->x = 5;

	if (!is_free_space_for_brick(b)) {
		return -1;
	}

	draw_brick(b);
	return 0;
}

int
is_free_space_for_brick(BRICK *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		if (!g_is_mesh_pixel_free(b->x + b->shape[i].x, b->y + b->shape[i].y)) {
			return G_FALSE;
		}
	}

	return G_TRUE;
}

void
draw_brick(BRICK *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		g_put_mesh_pixel(b->x + b->shape[i].x, b->y + b->shape[i].y, b->color);
		/*
		if ((b->shape[i].x == 0) && (b->shape[i].y == 0)) {
			g_put_mesh_pixel(b->x + b->shape[i].x, b->y + b->shape[i].y, RED);
		} else {
			g_put_mesh_pixel(b->x + b->shape[i].x, b->y + b->shape[i].y, b->color);
		}
		*/
	}
}

void
hide_brick(BRICK *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++)
		g_empty_mesh_pixel(b->x + b->shape[i].x, b->y + b->shape[i].y);
}

int
move_down(BRICK *b)
{
	hide_brick(b);
	b->y++;
	if (!is_free_space_for_brick(b)) {
		b->y--;
		draw_brick(b);
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
move_left(BRICK *b)
{
	hide_brick(b);
	b->x--;
	if (!is_free_space_for_brick(b)) {
		b->x++;
		draw_brick(b);
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
move_right(BRICK *b)
{
	hide_brick(b);
	b->x++;
	if (!is_free_space_for_brick(b)) {
		b->x--;
		draw_brick(b);
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
rotate(BRICK *b)
{
	hide_brick(b);
	rotate_shape(b->shape, b->shape_size, 1);
	if (!is_free_space_for_brick(b)) {
		rotate_shape(b->shape, b->shape_size, -1);
		draw_brick(b);
		return -1;
	}
	draw_brick(b);

	return 0;
}

void
check_lines()
{
	int i;
	for (i = g_mesh_height() - 1; i >= 0; i--) {
		while (is_line_full(i)) {
			destroy_line(i);
		}
	}
}

int
is_line_full(int y)
{
	int x, result;

	result = G_TRUE;
	for (x = 0; x < g_mesh_width(); x++) {
		if (g_is_mesh_pixel_free(x, y)) {
			result = G_FALSE;
			break;
		}
	}

	return result;
}

void
destroy_line(int y)
{
	int empty;

	empty = g_copy_upper_line(y);
	if (!empty) {
		destroy_line(y - 1);
	}
}

void
wait(BRICK *b)
{
	int i;

	for (i = 0; i < 5; i++) {
		process_user_input(b);
		delay(50);
	}
	process_user_input(b);
}

void
process_user_input(BRICK *b)
{
	while (kbhit()) {
		int c;
		c = getch();

		switch (c) {
			case '7':
				move_left(b);
				break;
			case '8':
				rotate(b);
				break;
			case '9':
				move_right(b);
				break;
			case '4':
				while(move_down(b) != -1)
					delay(50);
				break;
			case 'q':
				exit(0);
		}
	}
}

