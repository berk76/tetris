/*
*       gui_tc.h
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


#ifndef _GUI_TC_
#define _GUI_TC_

#define G_FALSE 0
#define G_TRUE  1 

extern void gui_message(char *msg);
extern int gui_confirm(char *msg);
extern int gui_option(char *msg, char *options);
extern void gui_fill_rect(int x, int y, int width, int height, int color, int fill_type);

#endif
