/*
*       tetris.h
*       
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*       
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*       
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 28.6.2014        
*/


#ifndef _TETRIS_
#define _TETRIS_

#include "shape.h"

typedef enum {TETRIS, ADDTRIS} GAME_T;

typedef struct {
	int x;
	int y;
        int value;
	POSITION_T *shape;
	POSITION_T *current;
	int shape_size;
} BRICK_T;


typedef struct {
        GAME_T game;
        int grid_size_x;
        int grid_size_y;
        int origin_x;
        int origin_y;
        int **grid_map;
        int bk_color;
        int element_size;
        BRICK_T brick;
        int score;
        int is_initialized;
        int is_paused;       
} TETRIS_T;


#define T_COLORS_SIZE           8                               
#define TETRIS_BK_COLOR         7

                                
extern void t_create_game(TETRIS_T *tetris, GAME_T game, int x_size, int y_size, int brick_size);
extern void t_delete_game(TETRIS_T *tetris);
extern int t_go(TETRIS_T *tetris);
extern int t_move_down(TETRIS_T *tetris);
extern int t_move_left(TETRIS_T *tetris);
extern int t_move_right(TETRIS_T *tetris);
/* direction 1=anticlockwise -1=clockwise */
extern int t_rotate(TETRIS_T *tetris, int direction);

#endif
