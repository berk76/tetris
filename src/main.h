/*
*       main.h
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


#ifndef _MAIN_
#define _MAIN_

#include "tetris.h"

#define TETRIS_VERSION "0.2.1"


extern void m_put_mesh_pixel(TETRIS_T *tetris, int x, int y, int color);
extern void m_empty_mesh_pixel(TETRIS_T *tetris, int x, int y);

#endif
