/*
*	tetris.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/

#ifndef _TETRIS_
#define _TETRIS_

#include "shape.h"


typedef struct {
	int x;
	int y;
	int color;
	POSITION_T *shape;
	POSITION_T *current;
	int shape_size;
} BRICK_T;


typedef struct {
        int grid_size_x;
        int grid_size_y;
        int origin_x;
        int origin_y;
        int **grid_map;
        int element_size;
        BRICK_T brick;
        int score;
        int is_initialized;
        int is_paused;       
} TETRIS_T;


#define T_COLORS_SIZE           8                               
#define TETRIS_BK_COLOR         7

                                
extern void t_create_game(TETRIS_T *tetris, int x_size, int y_size, int brick_size);
extern void t_delete_game(TETRIS_T *tetris);
extern int t_go(TETRIS_T *tetris);
extern int t_move_down(TETRIS_T *tetris);
extern int t_move_left(TETRIS_T *tetris);
extern int t_move_right(TETRIS_T *tetris);
/* direction 1=anticlockwise -1=clockwise */
extern int t_rotate(TETRIS_T *tetris, int direction);

extern void t_set_f_put_mesh_pixel(void (*put_mesh_pixel)(TETRIS_T *tetris, int x, int y, int color));
extern void t_set_f_empty_mesh_pixel(void (*empty_mesh_pixel)(TETRIS_T *tetris, int x, int y));

#endif
