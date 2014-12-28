/*
*	tetris.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "shape.h"
#include "multi.h"
#include "guicntls.h"
#include "tetris.h"


typedef struct {
	int x;
	int y;
	int color;
	G_POSITION *shape;
	G_POSITION *current;
	int shape_size;
} BRICK;


#define COLOR_VEC_SIZE		7
static int color_vec[] = 	{M_LIGHTBLUE,
				M_LIGHTGREEN,
				M_LIGHTCYAN,
				M_LIGHTRED,
				M_LIGHTMAGENTA,
				M_YELLOW,
				M_WHITE};

static int _delay_ms;
static int score;



static int create_new_brick(BRICK *b);
static int is_free_space_for_brick(BRICK *b);
static int is_current(BRICK *b, int x, int y);
static void clear_current(BRICK *b);
static void draw_brick(BRICK *b);
static int move_down(BRICK *b);
static int move_left(BRICK *b);
static int move_right(BRICK *b);
static int rotate(BRICK *b);
static int check_lines();
static int is_line_full(int y);
static void destroy_line(int y);
static void wait(BRICK *b);
static void process_user_input(BRICK *b);


int
t_run(int num_of_brick_elements, int delay_ms)
{
	BRICK brick;

	brick.shape_size = num_of_brick_elements;
	brick.shape = (G_POSITION *) malloc(brick.shape_size * sizeof(G_POSITION));
	brick.current = (G_POSITION *) malloc(brick.shape_size * sizeof(G_POSITION));
        _delay_ms = delay_ms;
	score = 0;
	
	while (create_new_brick(&brick) != -1) {
		do {
			wait(&brick);
		} while (move_down(&brick) != -1);
		score += check_lines();
		g_update_score(score);
	}
	
	free((void *) brick.shape);
	free((void *) brick.current);	

	return score;
}

int
create_new_brick(BRICK *b)
{
	srand(time(NULL) % 37);

	b->color = color_vec[rand() % COLOR_VEC_SIZE];
	b->y = create_new_shape(b->shape, b->shape_size);
	b->x = 5;

	clear_current(b);
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
		if (!g_is_mesh_pixel_free(b->x + b->shape[i].x, b->y + b->shape[i].y)
			&& !is_current(b, b->x + b->shape[i].x, b->y + b->shape[i].y)) {
			return G_FALSE;
		}
	}

	return G_TRUE;
}

int
is_current(BRICK *b, int x, int y)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		if ((b->current[i].x == x) && (b->current[i].y == y))
			return G_TRUE;
	}

	return G_FALSE;
}

void
clear_current(BRICK *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = 0;
		b->current[i].y = 0;
	}

}

void
draw_brick(BRICK *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++)
		g_empty_mesh_pixel(b->current[i].x, b->current[i].y);

	for (i = 0; i < b->shape_size; i++)
		g_put_mesh_pixel(b->x + b->shape[i].x, b->y + b->shape[i].y, b->color);

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = b->x + b->shape[i].x;
		b->current[i].y = b->y + b->shape[i].y;
	}
}

int
move_down(BRICK *b)
{
	b->y++;
	if (!is_free_space_for_brick(b)) {
		b->y--;
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
move_left(BRICK *b)
{
	b->x--;
	if (!is_free_space_for_brick(b)) {
		b->x++;
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
move_right(BRICK *b)
{
	b->x++;
	if (!is_free_space_for_brick(b)) {
		b->x--;
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
rotate(BRICK *b)
{
	rotate_shape(b->shape, b->shape_size, 1);
	if (!is_free_space_for_brick(b)) {
		rotate_shape(b->shape, b->shape_size, -1);
		return -1;
	}
	draw_brick(b);

	return 0;
}

int
check_lines()
{
	int i, lines;

	lines = 0;
	for (i = g_mesh_height() - 1; i >= 0; i--) {
		while (is_line_full(i)) {
			destroy_line(i);
			lines++;
		}
	}

	return lines;
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
		m_delay(_delay_ms);
	}
	process_user_input(b);
}

void
process_user_input(BRICK *b)
{
	int c;
	while ((c = m_kbhit()) != 0) {

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
					m_delay(_delay_ms);
				break;
			case 'p':
				gui_message("Paused");
				break;
			case 'q':
				if (gui_confirm("Do you want to quit game? (Y/N)") == G_TRUE)
					exit(0);
		}
	}
}


