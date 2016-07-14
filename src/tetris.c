/*
*	tetris.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "shape.h"
#include "tetris.h"


#define COLOR_VEC_SIZE		7

void (*g_put_mesh_pixel)(TETRIS_T *tetris, int x, int y, int color);
void (*g_empty_mesh_pixel)(TETRIS_T *tetris, int x, int y);
                                
static int create_new_brick(TETRIS_T *tetris);
static int is_free_space_for_brick(TETRIS_T *tetris);
static int is_current(BRICK_T *b, int x, int y);
static void clear_current(BRICK_T *b);
static void draw_brick(TETRIS_T *tetris);
static int check_lines(TETRIS_T *tetris);
static int is_line_full(TETRIS_T *tetris, int y);
static void destroy_line(TETRIS_T *tetris, int y);
static int copy_upper_line(TETRIS_T *tetris, int y);


void 
t_create_game(TETRIS_T *tetris, int x_size, int y_size, int brick_size)
{
        int i, ii;
        
        tetris->grid_size_x = x_size;
        tetris->grid_size_y = y_size;
        tetris->origin_x = 0;
        tetris->origin_y = 0;
        tetris->grid_map = (int **) malloc(sizeof(int *) * x_size);
        for (i = 0; i < x_size; i++) {
                tetris->grid_map[i] = (int *) malloc(sizeof(int) * y_size);
                for (ii = 0; ii < y_size; ii++) {
                        tetris->grid_map[i][ii] = TETRIS_BK_COLOR;
                }
        }
        tetris->element_size = 1;
        tetris->score = 0;
        tetris->brick.shape_size = brick_size;
	tetris->brick.shape = (POSITION_T *) malloc(tetris->brick.shape_size * sizeof(POSITION_T));
	tetris->brick.current = (POSITION_T *) malloc(tetris->brick.shape_size * sizeof(POSITION_T));
        tetris->is_initialized = 0;
}


void 
t_delete_game(TETRIS_T *tetris)
{
        int i;
        
        for (i = 0; i < tetris->grid_size_x; i++) {
                free((void *) tetris->grid_map[i]);
        }
        free((void *) tetris->grid_map);
        free((void *) tetris->brick.shape);
	free((void *) tetris->brick.current);
}


int 
t_go(TETRIS_T *tetris)
{
        int result;
        
        if (!tetris->is_initialized) {
                result = create_new_brick(tetris);
                tetris->is_initialized = 1;
        } else {
                result = t_move_down(tetris);
                if (result == -1) {
                        tetris->score += check_lines(tetris);
                        result = create_new_brick(tetris);
                }
        }
                
        return result;
}


int
create_new_brick(TETRIS_T *tetris)
{
        BRICK_T *b;
               
        b = &tetris->brick;
	srand(time(NULL) % 37); 
	b->color = rand() % COLOR_VEC_SIZE;
	b->y = create_new_shape(b->shape, b->shape_size);
	b->x = tetris->grid_size_x / 2;

	clear_current(b);
	if (!is_free_space_for_brick(tetris)) {
		return -1;
	}

	draw_brick(tetris);
	return 0;
}


int
is_free_space_for_brick(TETRIS_T *tetris)
{
        BRICK_T *b;
	int i;

        b = &tetris->brick;
	for (i = 0; i < b->shape_size; i++) {
                if ((b->x + b->shape[i].x) < 0 || (b->x + b->shape[i].x) >= tetris->grid_size_x)
                        return 0;
                        
                if ((b->y + b->shape[i].y) < 0 || (b->y + b->shape[i].y) >= tetris->grid_size_y)
                        return 0;
                                
		if ((tetris->grid_map[b->x + b->shape[i].x][b->y + b->shape[i].y] != TETRIS_BK_COLOR)
			&& !is_current(b, b->x + b->shape[i].x, b->y + b->shape[i].y)) {
			return 0;
		}
	}

	return 1;
}


int
is_current(BRICK_T *b, int x, int y)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		if ((b->current[i].x == x) && (b->current[i].y == y))
			return 1;
	}

	return 0;
}


void
clear_current(BRICK_T *b)
{
	int i;

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = 0;
		b->current[i].y = 0;
	}

}


void
draw_brick(TETRIS_T *tetris)
{
        BRICK_T *b;
	int i;

        b = &tetris->brick;
	for (i = 0; i < b->shape_size; i++) {
                tetris->grid_map[b->current[i].x][b->current[i].y] = TETRIS_BK_COLOR; 
                g_empty_mesh_pixel(tetris, b->current[i].x, b->current[i].y);
        }
		
	for (i = 0; i < b->shape_size; i++) {
                tetris->grid_map[b->x + b->shape[i].x][b->y + b->shape[i].y] = b->color;
                g_put_mesh_pixel(tetris, b->x + b->shape[i].x, b->y + b->shape[i].y, b->color);
        }

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = b->x + b->shape[i].x;
		b->current[i].y = b->y + b->shape[i].y;
	}
}


int
t_move_down(TETRIS_T *tetris)
{
        BRICK_T *b;
        
        b = &tetris->brick;
	b->y++;
	if (!is_free_space_for_brick(tetris)) {
		b->y--;
		return -1;
	}
	draw_brick(tetris);

	return 0;
}


int
t_move_left(TETRIS_T *tetris)
{
        BRICK_T *b;
        
        b = &tetris->brick;
	b->x--;
	if (!is_free_space_for_brick(tetris)) {
		b->x++;
		return -1;
	}
	draw_brick(tetris);

	return 0;
}


int
t_move_right(TETRIS_T *tetris)
{
        BRICK_T *b;
        
        b = &tetris->brick;
	b->x++;
	if (!is_free_space_for_brick(tetris)) {
		b->x--;
		return -1;
	}
	draw_brick(tetris);

	return 0;
}


int
t_rotate(TETRIS_T *tetris, int direction)
{
        BRICK_T *b;
        
        b = &tetris->brick;
	rotate_shape(b->shape, b->shape_size, direction);
	if (!is_free_space_for_brick(tetris)) {
		rotate_shape(b->shape, b->shape_size, -direction);
		return -1;
	}
	draw_brick(tetris);

	return 0;
}


int
check_lines(TETRIS_T *tetris)
{
	int i, lines;

	lines = 0;
	for (i = tetris->grid_size_y - 1; i >= 0; i--) {
		while (is_line_full(tetris, i)) {
			destroy_line(tetris, i);
			lines++;
		}
	}

	return lines;
}


int
is_line_full(TETRIS_T *tetris, int y)
{
	int x, result;

	result = 1;
	for (x = 0; x < tetris->grid_size_x; x++) {
		if (tetris->grid_map[x][y] == TETRIS_BK_COLOR) {
			result = 0;
			break;
		}
	}

	return result;
}


void
destroy_line(TETRIS_T *tetris, int y)
{
	int empty;

	empty = copy_upper_line(tetris, y);
	if (!empty) {
		destroy_line(tetris, y - 1);
	}
}


int
copy_upper_line(TETRIS_T *tetris, int y)
{
	int x, color, empty;

	empty = 1;
	for (x = 0; x < tetris->grid_size_x; x++) {
		if (y != 0) {
			color = tetris->grid_map[x][y-1]; 
		} else {
			color = TETRIS_BK_COLOR;
		}

                tetris->grid_map[x][y] = color;
                g_put_mesh_pixel(tetris, x, y, color);
                
		if (color != TETRIS_BK_COLOR) {
			empty = 0;
		}
	}

	return empty;
}


void t_set_f_put_mesh_pixel(void (*put_mesh_pixel)(TETRIS_T *tetris, int x, int y, int color)) {
        g_put_mesh_pixel = put_mesh_pixel;
}


void t_set_f_empty_mesh_pixel(void (*empty_mesh_pixel)(TETRIS_T *tetris, int x, int y)) {
        g_empty_mesh_pixel = empty_mesh_pixel;
}
