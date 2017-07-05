/*
*       tui_gfx.h
*
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 25.6.2017
*/


#ifndef _TUI_GFX_
#define _TUI_GFX_

#include "tui_tc.h"

/*
*       Gtaphics mostly comes from:
*       
*       http://www.chris.com/ascii/
*       http://patorjk.com/software/taag
*/

/*
*       Colors:
*       -------
*       blue            \x01\x01
*       green           \x01\x02
*       cyan            \x01\x03
*       red             \x01\x04
*       magenta         \x01\x05
*       brown           \x01\x06
*       lightgray       \x01\x07
*       darkgray        \x01\x08
*       lightblue       \x01\x09
*       lightgreen      \x01\x0a
*       lightcyan       \x01\x0b
*       lightred        \x01\x0c
*       lightmagenta    \x01\x0d
*       yellow          \x01\x0e
*       white           \x01\x0f
*/




 char *gfx_ptakovina =  "\x01\x0e _______ __         __                __\n" \
                        "|   _   |  |_.---.-|  |--.-----.--.--|__.-----.---.-.\n" \
                        "|.  1   |   _|  _  |    <|  _  |  |  |  |     |  _  |\n" \
                        "\x01\x0c|.  ____|____|___._|__|__|_____|\\___/|__|__|__|___._|\n" \
                        "|:  |\n" \
                        "|::.|\n" \
                        "`---'";

char *gfx_tetris =  "\x01\x0e _______       __        __ \n" \
                    "|       .-----|  |_.----|__.-----.\n" \
                    "|.|   | |  -__|   _|   _|  |__ --|\n" \
                    "\x01\x0c`-|.  |-|_____|____|__| |__|_____|\n" \
                    "  |:  |\n" \
                    "  |::.|\n" \
                    "  `---'";

char *gfx_addtris = "\x01\x0e _______    __    __ _______      __\n" \
                    "|   _   .--|  .--|  |       .----|__.-----.\n" \
                    "|.  1   |  _  |  _  |.|   | |   _|  |__ --|\n" \
                    "\x01\x0c|.  _   |_____|_____`-|.  |-|__| |__|_____|\n" \
                    "|:  |   |             |:  |\n" \
                    "|::.|:. |             |::.|\n" \
                    "`--- ---'             `---'";

char *gfx_bird_01 = "    ___\n" \
                    "   (  \">\n" \
                    "    )(\n" \
                    "   // )\n" \
                    "--//\"\"--\n" \
                    "-/------";

char *gfx_bird_02 = "|\n" \
                    "|\n" \
                    "<-*)\n" \
                    "|(()\n" \
                    "|\"/\n" \
                    "|'";

char *gfx_bird_03 = "                ,\n" \
                    " ,_     ,     .'<_\n" \
                    "_> `'-,'(__.-' __<\n" \
                    ">_.--(.. )  =;`\n" \
                    "     `V-'`'\\/``";

char *gfx_bird_04 = " ('<        >')\n" \
                    "\\(_)________( \\\n" \
                    " (___________)\\\\\n" \
                    "    (     )     \\\n" \
                    "     |   |\n" \
                    "     |   |\n" \
                    "     |   |\n" \
                    "    _|   |_\n" \
                    "   (_______)";
                    
char *gfx_bird_05 = "\x01\x0f         .-.                                              .-.\n" \
                    "        (. .)__.')                                  (`.__(. .)\n" \
                    "        / \x01\x0cV\x01\x0f      )                                  (      \x01\x0cV\x01\x0f \\\n" \
                    "  \x01\x0a()\x01\x0f    \\  (   \\/                                    \\/   )  /    \x01\x0a()\n" \
                    "\x01\x0a<)-`\\()\x01\x0f  `._`._ \\                                    / _.'_.'  \x01\x0a()/'-(>\n" \
                    "\x01\x0a  <)_>\x01\x06=====\x01\x04<<\x01\x06==\x01\x0f`'\x01\x06====================================\x01\x0f`'\x01\x06==\x01\x04>>\x01\x06=====\x01\x0a<_(>\n" \
                    "\x01\x0a <>-'`(>                                                      <)'`-<>";

char *gfx_owl_01 =  "  ,___,\n" \
                    "  (9v9)\n" \
                    "  (_^((\\\n" \
                    "^^^\"^\" \\\\^^^^\n" \
                    "^^^^^^^^^^^^^";

char *gfx_owl_02 =  "  >\\_/<\n" \
                    " _\\*v*/_\n" \
                    " \\\\   //\n" \
                    "====\"====\n" \
                    "   /^\\";

char *gfx_mush_01 = "\x01\x04  .-\"\"\"-.\n" \
                    " /\x01\x07* * * *\x01\x04\\\n" \
                    ":_.-:`:-._;\n" \
                    "    \x01\x07(_)\n" \
                    " \x01\x02\\|/\x01\x07(_)\x01\x02\\|/";

#endif
