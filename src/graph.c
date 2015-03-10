/*
*	graph.c
*	Jaroslav Beran - jaroslav.beran@gmail.com
*	28.6.2014
*	redistributable under the terms of the GNU/GPLv3
*/


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "tetris.h"
#include "graph.h"


void 
g_draw_mesh(HDC hdc, TETRIS_T *tetris, RECT client)
{
        int i1, i2;
        int margin;
        
        margin = 15;
        i1 = (client.right - client.left - margin) / tetris->grid_size_x;
        i2 = (client.bottom - client.top - margin) / tetris->grid_size_y;
        tetris->element_size = (i1 < i2) ? i1 : i2;
        tetris->origin_x = ((client.right - client.left) - (tetris->grid_size_x * tetris->element_size)) / 2;
        tetris->origin_y = ((client.bottom - client.top) - (tetris->grid_size_y * tetris->element_size)) / 2;
        
        for (i1 = 0; i1 < tetris->grid_size_x; i1++) {
                for (i2 = 0; i2 < tetris->grid_size_y; i2++)
                        g_put_mesh_pixel(hdc, tetris, i1, i2, tetris->grid_map[i1][i2]);
        }
}


void 
g_put_mesh_pixel(HDC hdc, TETRIS_T *tetris, int x, int y, int color)
{
        HPEN hPen;
        HBRUSH hBrush;
        
        hPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, 2, color);
        hBrush = CreateSolidBrush(color);
        SelectObject(hdc, hPen);
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 
                tetris->origin_x + x * tetris->element_size, 
                tetris->origin_y + y * tetris->element_size, 
                tetris->origin_x + x * tetris->element_size + tetris->element_size, 
                tetris->origin_y + y * tetris->element_size + tetris->element_size);
        DeleteObject(hBrush);
        DeleteObject(hPen);
}


void 
g_empty_mesh_pixel(HDC hdc, TETRIS_T *tetris, int x, int y)
{
        g_put_mesh_pixel(hdc, tetris, x, y, TETRIS_BK_COLOR);
}

