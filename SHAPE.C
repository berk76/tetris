/*
*	shape.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/

#include <stdlib.h>
#include "graph.h"
#include "shape.h"

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

static int get_shape_width(G_POSITION shape[], int size);
static int get_shape_min_x(G_POSITION shape[], int size);
static int get_shape_max_y(G_POSITION shape[], int size, int x);
static int centre_shape(G_POSITION shape[], int size);

int
create_new_shape(G_POSITION shape[], int size)
{
	int i;

	randomize();
	for (i = 0; i < size; i++) {
		if (i == 0) {
			shape[i].x = 0;
			shape[i].y = 0;
		} else {
			int width, rnd;

			width = get_shape_width(shape, i);
			rnd = rand() % (width + 2);
			shape[i].x = get_shape_min_x(shape, i) - 1 + rnd;
			shape[i].y = get_shape_max_y(shape, i, shape[i].x) + 1;
		}
	}

	return centre_shape(shape, size);
}

int
get_shape_width(G_POSITION shape[], int size)
{
	int result;

	if (size == 0) {
		result = 0;
	} else {
		int i, min, max;

		min = 0;
		max = 0;

		for (i = 0; i < size; i++){
			if (shape[i].x < min) min = shape[i].x;
			if (shape[i].x > max) max = shape[i].x;
		}

		result = max - min + 1;
	}

	return result;
}

int
get_shape_min_x(G_POSITION shape[], int size)
{
	int result, i;
	result = 0;

	for (i = 0; i < size; i++){
		if (shape[i].x < result) result = shape[i].x;
	}

	return result;
}

int
get_shape_max_y(G_POSITION shape[], int size, int x)
{
	int result, rnd, i;
	result = -1;
	rnd = rand() % 4;

	for (i = 0; i < size; i++){
		if ((shape[i].x == x) && (shape[i].y > result)) {
			result = shape[i].y;
		} else {
			if ((rnd > 0) && (shape[i].x - 1 == x) && (shape[i].y - 1 > result)) {
				result = shape[i].y - 1;
			} else {
				if ((rnd > 0) && (shape[i].x + 1 == x) && (shape[i].y - 1 > result))
					result = shape[i].y - 1;
			}
		}
	}

	return result;
}

int
centre_shape(G_POSITION shape[], int size)
{
	double	sum_x, sum_y;
	int i, cx, cy;

	sum_x = 0;
	sum_y = 0;

	for (i = 0; i < size; i++) {
		sum_x += shape[i].x;
		sum_y += shape[i].y;
	}

	cx = round(sum_x / (double) size);
	cy = round(sum_y / (double) size);

	for (i = 0; i < size; i++) {
		shape[i].x -= cx;
		shape[i].y -= cy;
	}

	return cy;
}

void
rotate_shape(G_POSITION shape[], int size, int direction)
{
	int i, x, y;

	for (i = 0; i < size; i++) {
		x = shape[i].y * direction;
		y = shape[i].x * -direction;
		shape[i].x = x;
		shape[i].y = y;
	}
}


