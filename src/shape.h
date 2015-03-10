/*
*	shape.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
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


