/*
*	graph.h
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*/


#ifndef _GRAPH_
#define _GRAPH_

#define G_FALSE		0
#define G_TRUE 		1

typedef struct {
	int x;
	int y;
} G_POSITION;


extern int g_mesh_height();
extern int g_mesh_width();
extern void g_initialize();
extern void g_close();
extern void g_report_status();
extern void g_draw_mesh(int height, int width, int grid_size, int bk_color);
extern void g_update_score(int score);
extern void g_put_mesh_pixel(int x, int y, int color);
extern void g_empty_mesh_pixel(int x, int y);
extern int g_is_mesh_pixel_free(int x, int y);
extern int g_copy_upper_line(int y);
extern void fill_rect(int x, int y, int width, int height, int color, int fill_type);
extern int g_printf(int *xloc, int *yloc, char *fmt, ...);

#endif

