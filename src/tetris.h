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
} TETRIS_T;


enum M_COLORS {
    M_BLACK		= 0,
    M_BLUE		= 1,
    M_GREEN		= 2,
    M_CYAN		= 3,
    M_RED		= 4,
    M_MAGENTA		= 5,
    M_BROWN		= 6,
    M_LIGHTGRAY		= 7,
    M_DARKGRAY		= 8,
    M_LIGHTBLUE		= 0xFF0000,
    M_LIGHTGREEN	= 0x00FF00,
    M_LIGHTCYAN		= 0xFFFF00,
    M_LIGHTRED		= 0x0000FF,
    M_LIGHTMAGENTA 	= 0xFF00FF,
    M_YELLOW		= 0x00FFFF,
    M_WHITE		= 0xFFFFFF
};
                                
#define TETRIS_BK_COLOR         0x000000

                                
extern void t_create_game(TETRIS_T *tetris, int x_size, int y_size);
extern void t_delete_game(TETRIS_T *tetris);
extern int t_go(HDC hdc, TETRIS_T *tetris);
extern int t_move_down(HDC hdc, TETRIS_T *tetris);
extern int t_move_left(HDC hdc, TETRIS_T *tetris);
extern int t_move_right(HDC hdc, TETRIS_T *tetris);
extern int t_rotate(HDC hdc, TETRIS_T *tetris);

#endif


