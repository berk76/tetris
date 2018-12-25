/*
*       tetris.c
*       
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 28.6.2014
*/


#include <stdio.h>
#include <stdlib.h>
#include "shape.h"
#include "tetris.h"
#include "main.h"


#define COLOR_VEC_SIZE		7

static char mem_err[] = "No memory space left";
                                
static int create_new_brick(TETRIS_T *tetris);
static int is_free_space_for_brick(TETRIS_T *tetris);
static int is_current(BRICK_T *b, int x, int y);
static void clear_current(BRICK_T *b);
static void draw_brick(TETRIS_T *tetris);
static int check_lines(TETRIS_T *tetris);
static int is_line_full(TETRIS_T *tetris, int y);
static void destroy_line(TETRIS_T *tetris, int y);
static int copy_upper_line(TETRIS_T *tetris, int y);
static int check_lines_addtris(TETRIS_T *tetris);


void t_create_game(TETRIS_T *tetris, GAME_T game, int x_size, int y_size, int brick_size) {
        int i, ii;
        
        tetris->game = game;
        if ((tetris->game == TETRIS) || (tetris->game == XTRIS)) {
                tetris->bk_color = TETRIS_BK_COLOR; 
        } else {
                tetris->bk_color = -1;
        }
        tetris->grid_size_x = x_size;
        tetris->grid_size_y = y_size;
        tetris->origin_x = 0;
        tetris->origin_y = 0;
        tetris->grid_map = (int **) malloc(sizeof(int *) * x_size);
        if (tetris->grid_map == NULL) {
                puts(mem_err);
                exit(-1);
        }
        for (i = 0; i < x_size; i++) {
                tetris->grid_map[i] = (int *) malloc(sizeof(int) * y_size);
                if (tetris->grid_map[i] == NULL) {
                        puts(mem_err);
                        exit(-1);
                }
                for (ii = 0; ii < y_size; ii++) {
                        tetris->grid_map[i][ii] = tetris->bk_color;
                }
        }
        tetris->element_size = 1;
        tetris->score = 0;
        tetris->brick.shape_size = brick_size;
	tetris->brick.shape = (POSITION_T *) malloc(tetris->brick.shape_size * sizeof(POSITION_T));
        if (tetris->brick.shape == NULL) {
                puts(mem_err);
                exit(-1);
        }
	tetris->brick.current = (POSITION_T *) malloc(tetris->brick.shape_size * sizeof(POSITION_T));
        if (tetris->brick.current == NULL) {
                puts(mem_err);
                exit(-1);
        }
        tetris->is_initialized = 0;
}


void t_delete_game(TETRIS_T *tetris) {
        int i;
        
        for (i = 0; i < tetris->grid_size_x; i++) {
                free((void *) tetris->grid_map[i]);
        }
        free((void *) tetris->grid_map);
        free((void *) tetris->brick.shape);
	free((void *) tetris->brick.current);
}


int t_go(TETRIS_T *tetris) {
        int result;
        
        if (!tetris->is_initialized) {
                result = create_new_brick(tetris);
                tetris->is_initialized = 1;
        } else {
                result = t_move_down(tetris);
                if (result == -1) {
                        if ((tetris->game == TETRIS) || (tetris->game == XTRIS)) {
                                tetris->score += check_lines(tetris);
                        } else {
                                tetris->score += check_lines_addtris(tetris);
                        }
                        result = create_new_brick(tetris);
                }
        }
                
        return result;
}


int create_new_brick(TETRIS_T *tetris) {
        BRICK_T *b;
                                         
        b = &tetris->brick;
        if ((tetris->game == TETRIS) || (tetris->game == XTRIS)) { 
                b->value = rand() % COLOR_VEC_SIZE;
        } else {
                b->value = rand() % 10;
        }
	b->y = create_new_shape(b->shape, b->shape_size);
	b->x = tetris->grid_size_x / 2;

	clear_current(b);
	if (!is_free_space_for_brick(tetris)) {
		return -1;
	}

	draw_brick(tetris);
	return 0;
}


int is_free_space_for_brick(TETRIS_T *tetris) {
        BRICK_T *b;
	int i;

        b = &tetris->brick;
	for (i = 0; i < b->shape_size; i++) {
                if ((b->x + b->shape[i].x) < 0 || (b->x + b->shape[i].x) >= tetris->grid_size_x)
                        return 0;
                        
                if ((b->y + b->shape[i].y) < 0 || (b->y + b->shape[i].y) >= tetris->grid_size_y)
                        return 0;
                                
		if ((tetris->grid_map[b->x + b->shape[i].x][b->y + b->shape[i].y] != tetris->bk_color)
			&& !is_current(b, b->x + b->shape[i].x, b->y + b->shape[i].y)) {
			return 0;
		}
	}

	return 1;
}


int is_current(BRICK_T *b, int x, int y) {
	int i;

	for (i = 0; i < b->shape_size; i++) {
		if ((b->current[i].x == x) && (b->current[i].y == y))
			return 1;
	}

	return 0;
}


void clear_current(BRICK_T *b) {
	int i;

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = 0;
		b->current[i].y = 0;
	}

}


void draw_brick(TETRIS_T *tetris) {
        BRICK_T *b;
	int i;

        b = &tetris->brick;
	for (i = 0; i < b->shape_size; i++) {
                tetris->grid_map[b->current[i].x][b->current[i].y] = tetris->bk_color; 
                m_empty_mesh_pixel(tetris, b->current[i].x, b->current[i].y);
        }
		
	for (i = 0; i < b->shape_size; i++) {
                tetris->grid_map[b->x + b->shape[i].x][b->y + b->shape[i].y] = b->value;
                m_put_mesh_pixel(tetris, b->x + b->shape[i].x, b->y + b->shape[i].y, b->value);
        }

	for (i = 0; i < b->shape_size; i++) {
		b->current[i].x = b->x + b->shape[i].x;
		b->current[i].y = b->y + b->shape[i].y;
	}
}


int t_move_down(TETRIS_T *tetris) {
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


int t_move_left(TETRIS_T *tetris) {
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


int t_move_right(TETRIS_T *tetris) {
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


int t_rotate(TETRIS_T *tetris, int direction) {
        BRICK_T *b;
        
        b = &tetris->brick;
        if ((tetris->game == TETRIS) || (tetris->game == XTRIS)) {
                rotate_shape(b->shape, b->shape_size, direction);
                if (!is_free_space_for_brick(tetris)) {
                        rotate_shape(b->shape, b->shape_size, -direction);
                        return -1;
                }
        } else {
                if (b->value != 0)
                        b->value = 10 - b->value; 
        }
	draw_brick(tetris);

	return 0;
}


int check_lines(TETRIS_T *tetris) {
	int i, lines;

	lines = 0;
	for (i = tetris->grid_size_y - 1; i >= 0; i--) {
		while (is_line_full(tetris, i)) {
			destroy_line(tetris, i);
                        m_line_destroyed();
			lines++;
		}
	}

	return lines;
}


int is_line_full(TETRIS_T *tetris, int y) {
	int x, result;

	result = 1;
	for (x = 0; x < tetris->grid_size_x; x++) {
		if (tetris->grid_map[x][y] == tetris->bk_color) {
			result = 0;
			break;
		}
	}

	return result;
}


void destroy_line(TETRIS_T *tetris, int y) {
	int empty;

	empty = copy_upper_line(tetris, y);
	if (!empty) {
		destroy_line(tetris, y - 1);
	}
}


int copy_upper_line(TETRIS_T *tetris, int y) {
	int x, color, empty;

	empty = 1;
	for (x = 0; x < tetris->grid_size_x; x++) {
		if (y != 0) {
			color = tetris->grid_map[x][y-1]; 
		} else {
			color = tetris->bk_color;
		}

                tetris->grid_map[x][y] = color;
                
		if (color != tetris->bk_color) {
			empty = 0;
                        m_empty_mesh_pixel(tetris, x, y);
                        m_put_mesh_pixel(tetris, x, y, color);
		} else {
                        m_empty_mesh_pixel(tetris, x, y);
                }
	}

	return empty;
}

int check_lines_addtris(TETRIS_T *tetris) {
	int lines, a, b;

	lines = 0;
	if ((tetris->grid_size_y - 1) >= (tetris->brick.y + 2)) {
		a = tetris->grid_map[tetris->brick.x][tetris->brick.y + 1];
                b = tetris->grid_map[tetris->brick.x][tetris->brick.y + 2];
                a = (a + b) % 10;
                if ((a != tetris->bk_color) && (b != tetris->bk_color) 
                        && (tetris->brick.value == a)) {
                        
                        lines++;
                        tetris->grid_map[tetris->brick.x][tetris->brick.y + 0] = tetris->bk_color;
                        tetris->grid_map[tetris->brick.x][tetris->brick.y + 1] = tetris->bk_color;
                        tetris->grid_map[tetris->brick.x][tetris->brick.y + 2] = tetris->brick.value;
                        m_empty_mesh_pixel(tetris, tetris->brick.x, tetris->brick.y + 0);
                        m_empty_mesh_pixel(tetris, tetris->brick.x, tetris->brick.y + 1);
                        m_put_mesh_pixel(tetris, tetris->brick.x, tetris->brick.y + 2, tetris->brick.value);
                }
	}

	return lines;
}
