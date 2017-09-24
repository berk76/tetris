/*
*       shape.h
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


#ifndef _SHAPE_
#define _SHAPE_

typedef struct {
	int x;
	int y;
} POSITION_T;

extern int create_new_shape(POSITION_T shape[], int size);
extern void rotate_shape(POSITION_T shape[], int size, int direction);

#endif

