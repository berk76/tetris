/*
*       res_tc.h
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


#ifndef _RES_TC_
#define _RES_TC_

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
                    
char *gfx_xtris = "\x01\x0e   ___ ___    _______      __\n" \
                  "  (   Y   ___|       .----|__.-----.\n" \
                  "   \\  1  |___|.|   | |   _|  |__ --|\n" \
                  "\x01\x0c   /  _  \\   `-|.  |-|__| |__|_____|\n" \
                  "  /:  |   \\    |:  |\n" \
                  " (::. |:.  )   |::.|\n" \
                  "  `--- ---'    `---'";

char *gfx_bird_01 = "\x01\x0a    ___\n" \
                    "   (  \x01\x0e\"\x01\x0c>\x01\x0a\n" \
                    "    )(\n" \
                    "   // )\n" \
                    "\x01\x07--\x01\x0a//\x01\x04\"\"\x01\x07--\n" \
                    "\x01\x07-\x01\x0a/\x01\x07------";

char *gfx_bird_02 = "|\n" \
                    "|\n" \
                    "<-*)\n" \
                    "|(()\n" \
                    "|\"/\n" \
                    "|'";

char *gfx_bird_03 = "\x01\x06                ,\n" \
                    " ,_     ,     .'<_\n" \
                    "_> `'-,'(__.-' __<\n" \
                    ">_.--(\x01\x0b..\x01\x06 )  =;`\n" \
                    "     `\x01\x0cV\x01\x06-'`'\\/``";

char *gfx_bird_04 = "\x01\x0a (\x01\x0e'\x01\x0c<        >\x01\x0e'\x01\x0a)\n" \
                    "\x01\x0a\\(\x01\x0e_)\x01\x0f________\x01\x0e( \x01\x0a\\\n" \
                    "\x01\x0f (___________)\x01\x0e\\\x01\x0a\\\n" \
                    "\x01\x0f    (     )\x01\x0e     \x01\x0a\\\n" \
                    "\x01\x0f     |   |\n" \
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

 
char *gfx_caterpillar_01 = " .--.--.--.--.--.-\\_/\n" \
                           "(__(__(__(__(__(__(\")\n" \
                           " \"\" \"\" \"\" \"\" \"\" \"\" ^"; 

/*
*       Sounds
*/

/* Skakal pes */
 
#define D1 12
#define R1 60
static SND_PLAY_NOTE s1[] = {{G,O4,N4},{G,O4,N4}, 
                       {E,O4,N4},{REST,O4,N4}, 
                       {G,O4,N4},{G,O4,N4}, 
                       {E,O4,N4},{REST,O4,N4}, 
                       {G,O4,N4},{G,O4,N4}, 
                       {A,O4,N4},{G,O4,N4}, 
                       {G,O4,N2},
                       {F,O4,N2},
                       {F,O4,N4},{F,O4,N4}, 
                       {D,O4,N4},{REST,O4,N4}, 
                       {F,O4,N4},{F,O4,N4}, 
                       {D,O4,N4},{REST,O4,N4}, 
                       {F,O4,N4},{F,O4,N4}, 
                       {G,O4,N4},{F,O4,N4}, 
                       {F,O4,N2}, 
                       {E,O4,N2}, 
                       {REST,O4,N2}, 
                       {REPEAT,O4,N2} 
                };


/* Holka modrooka */
#define D2 17
#define R2 60
static SND_PLAY_NOTE s2[] = {{G,O5,N4},{A,O5,N4},{G,O5,N8},{G,O5,N8},{E,O5,N8},{G,O5,N8}, 
                       {F,O5,N8},{F,O5,N8},{D,O5,N8},{F,O5,N8},{G,O5,N8},{G,O5,N8},{E,O5,N8},{F,O5,N8}, 
                       {G,O5,N4},{A,O5,N4},{G,O5,N8},{G,O5,N8},{E,O5,N8},{G,O5,N8}, 
                       {F,O5,N8},{F,O5,N8},{D,O5,N8},{F,O5,N8},{E,O5,N4},{REST,O5,N4}, 
                       {F,O5,N8},{F,O5,N8},{D,O5,N8},{F,O5,N8},{G,O5,N8},{G,O5,N8},{E,O5,N8},{G,O5,N8}, 
                       {F,O5,N8},{F,O5,N8},{D,O5,N8},{F,O5,N8},{G,O5,N8},{G,O5,N8},{E,O5,N8},{F,O5,N8}, 
                       {G,O5,N4},{A,O5,N4},{G,O5,N8},{G,O5,N8},{E,O5,N8},{G,O5,N8}, 
                       {F,O5,N8},{F,O5,N8},{D,O5,N8},{F,O5,N8},{E,O5,N4},{REST,O5,N4}, 
                       {REST,O5,N2}, 
                       {REPEAT,O5,N2} 
                };
                

/* Kosi pisnicka */
#define D3 10
#define R3 60
static SND_PLAY_NOTE s3[] = {{C,O5,N4},{E,O5,N4},{E,O5,N2}, 
                       {G,O5,N4},{E,O5,N4},{E,O5,N4},{REST,O5,N4}, 
                       {C,O5,N4},{E,O5,N4},{E,O5,N4},{E,O5,N4}, 
                       {G,O5,N4},{E,O5,N4},{E,O5,N4},{REST,O5,N4}, 
                       {F,O5,N4DOT},{F,O5,N8},{F,O5,N4},{G,O5,N4}, 
                       {A,O5,N4},{A,O5,N4},{G,O5,N4},{F,O5,N4}, 
                       {E,O5,N4},{E,O5,N8},{E,O5,N4},{F,O5,N4}, 
                       {G,O5,N4},{G,O5,N4},{F,O5,N4},{E,O5,N4}, 
                       {F,O5,N4},{F,O5,N4},{A,O5,N4},{REST,O5,N4}, 
                       {F,O5,N4},{E,O5,N4},{G,O5,N4},{REST,O5,N4}, 
                       {F,O5,N4},{G,O5,N4},{F,O5,N4},{E,O5,N4}, 
                       {D,O5,N4},{D,O5,N4},{C,O5,N4},{REST,O5,N4}, 
                       {REST,O5,N2}, 
                       {REPEAT,O5,N2}
                };


/* Saxana */
#define D4 17
#define R4 60
static SND_PLAY_NOTE s4[] = {{C,O5,N4},{A,O5,N4},{G,O5,N4},{REST,O5,N4}, 
                       {E,O5,N16},{E,O5,N8},{E,O5,N16},{E,O5,N64},{D,O5,N16},{C,O5,N8},{E,O5,N4},{D,O5,N4},
                       {C,O5,N4},{A,O5,N4},{G,O5,N4},{REST,O5,N4},
                       {E,O5,N8},{E,O5,N16},{E,O5,N16},{D,O5,N8},{D,O5,N8},{C,O5,N4},{REST,O5,N4},
                       {C,O5,N4},{A,O5,N4},{G,O5,N4},{REST,O5,N4},
                       {E,O5,N16},{E,O5,N8},{E,O5,N16},{E,O5,N64},{D,O5,N16},{C,O5,N8},{E,O5,N4},{D,O5,N4},
                       {C,O5,N4},{A,O5,N4},{G,O5,N4},{REST,O5,N4},
                       {E,O5,N8},{E,O5,N16},{E,O5,N16},{D,O5,N8},{D,O5,N8},
                       {C,O5,N4},{REST,O5,N4},{REST,O5,N2},
                       {C,O5,N16},{C,O5,N8},{C,O5,N16},{C,O5,N16},{C,O5,N8},{C,O5,N16},{E,O5,N16},{E,O5,N16},{E,O5,N8},{E,O5,N16},{E,O5,N8},{E,O5,N16},
                       {F,O5,N16},{F,O5,N16},{F,O5,N8},{F,O5,N16},{F,O5,N8},{F,O5,N16},{A,O5,N4},{A,O5,N16},{A,O5,N8},{B,O5,N16},
                       {B,O5,N2},{REST,O5,N16},{B,O5,N16},{B,O5,N8},
                       {B,O5,N1},
                       {C,O5,N16},{C,O5,N8},{C,O5,N16},{C,O5,N16},{C,O5,N8},{C,O5,N16},{E,O5,N16},{E,O5,N16},{E,O5,N8},{E,O5,N16},{E,O5,N8},{E,O5,N16},
                       {F,O5,N16},{F,O5,N16},{F,O5,N8},{F,O5,N16},{F,O5,N8},{F,O5,N16},{A,O5,N4},{A,O5,N16},{A,O5,N8},{B,O5,N16},
                       {B,O5,N2},{REST,O5,N16},{B,O5,N16},{B,O5,N8},
                       {B,O5,N1},
                       {REPEAT,O5,N32} 
                };


/* Dance of The Trolls */
#define D5 17
#define R5 60
static SND_PLAY_NOTE s5[] = {{F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N4},{A,O4,N4},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{D,O5,N4},{D,O5,N8},{REST,O5,N8},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N4},{A,O4,N4},
                               {B,O4,N8},{C,O5,N8},{D,O5,N8},{G,O4,N8},{A,O4,N4},{A,O4,N8},{REST,O4,N8},
                               {A,O4,N4},{B,O4,N4},{C,O5,N8},{D,O5,N8},{E,O5,N4},
                               {E,O5,N8},{D,O5,N8},{C,O5,N4},{B,O4,N4},{B,O4,N4},
                               {A,O4,N4},{B,O4,N4},{C,O5,N8},{D,O5,N8},{E,O5,N4},
                               {E,O5,N8},{C,O5,N8},{B,O4,N8},{C,O5,N8},{A,O4,N4},{A,O4,N4},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{D,O5,N4},{D,O5,N8},{REST,O5,N8},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N4},{A,O4,N4},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{D,O5,N4},{D,O5,N8},{REST,O5,N8},
                               {B,O4,N8},{C,O5,N8},{D,O5,N8},{G,O4,N8},{A,O4,N4},{A,O4,N8},{REST,O4,N8},
                               {A,O4,N4},{B,O4,N4},{C,O5,N8},{D,O5,N8},{E,O5,N4},
                               {E,O5,N8},{D,O5,N8},{C,O5,N4},{B,O4,N4},{B,O4,N4},
                               {A,O4,N4},{B,O4,N4},{C,O5,N8},{D,O5,N8},{E,O5,N4},
                               {E,O5,N8},{C,O5,N8},{B,O4,N8},{C,O5,N8},{A,O4,N4},{E,O5,N8},{F,O5,N8},
                               {E,O5,N4},{E,O5,N8},{F,O5,N8},{D,O5,N4},{D,O5,N8},{E,O5,N8},
                               {C,O5,N4},{C,O5,N8},{D,O5,N8},{B,O4,N4},{B,O4,N8},{C,O5,N8},
                               {E,O5,N8},{C,O5,N8},{B,O4,N8},{C,O5,N8},{A,O4,N4},{A,O4,N4},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{D,O5,N4},{D,O5,N8},{REST,O5,N8},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N4},{A,O4,N4},
                               {F,O5,N8},{E,O5,N8},{C,O5,N8},{B,O4,N8},{D,O5,N4},{D,O5,N8},{REST,O5,N8},
                               {B,O4,N8},{C,O5,N8},{D,O5,N8},{G,O4,N8},{A,O4,N4},{A,O4,N8},{REST,O4,N8},
                               {REPEAT,O4,N16}
                };
                
/* Can Can */
#define D6 11
#define R6 60
static SND_PLAY_NOTE s6[] = {{D,O4,N8},{A,O4,N8},{A,O4,N8},{B,O4,N8},
                             {A,O4,N8},{G,O4,N8},{G,O4,N8},{B,O4,N8},
                             {C,O5,N8},{E,O5,N8},{G,O5,N8},{E,O5,N8},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{F,O4,N8},{F,O4,N8},{E,O5,N8},
                             {D,O5,N8},{G,O4,N8},{G,O4,N8},{B,O4,N8},
                             {B,O4,N8},{A,O4,N8},{B,O4,N8},{A,O4,N8},
                             {B,O4,N8},{A,O4,N8},{B,O4,N8},{A,O4,N8},
                             /* 09 */
                             {A,O4,N8},{G,O4,N8},{G,O4,N4},
                             {B,O4,N4},{G,O4,N4},
                             {E,O4,N4},{D,O4,N4},
                             {D,O4,N8},{A,O4,N8},{B,O4,N8},{C,O5,N8},
                             {B,O4,N8},{A,O4,N8},{G,O4,N4},
                             {B,O4,N4},{G,O4,N4},
                             {E,O4,N4},{D,O4,N4},
                             {CS,O4,N8},{D,O4,N8},{E,O4,N8},{F,O4,N8},
                             {A,O4,N8},{G,O4,N8},{G,O4,N4},
                             /* 18 */
                             {G,O4,N8},{D,O4,N8},{F,O4,N8},{D,O4,N8},
                             {G,O4,N8},{D,O4,N8},{F,O4,N8},{D,O4,N8},
                             {G,O4,N8},{D,O4,N8},{F,O4,N8},{D,O4,N8},
                             {G,O4,N8},{D,O4,N8},{F,O4,N8},{D,O4,N8},
                             {G,O4,N8DOT},{G,O4,N16},{G,O4,N8DOT},{G,O4,N16},
                             {G,O4,N8DOT},{G,O4,N16},{G,O4,N8DOT},{G,O4,N16},
                             {G,O4,N8DOT},{G,O4,N16},{G,O4,N8DOT},{G,O4,N16},
                             {G,O4,N8DOT},{G,O4,N16},{G,O4,N8DOT},{G,O4,N16},
                             /* 26 */
                             {C,O4,N2},
                             {D,O4,N8},{F,O4,N8},{E,O4,N8},{D,O4,N8},
                             {G,O4,N4},{G,O4,N4},
                             {G,O4,N8},{A,O4,N8},{E,O4,N8},{F,O4,N8},
                             {D,O4,N4},{D,O4,N4},
                             {D,O4,N8},{F,O4,N8},{E,O4,N8},{D,O4,N8},
                             {C,O4,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N8},{F,O4,N8},{E,O4,N8},{D,O4,N8},
                             {C,O4,N2},
                             {D,O4,N8},{F,O4,N8},{E,O4,N8},{D,O4,N8},
                             /* 36 */
                             {G,O4,N4},{G,O4,N4},
                             {G,O4,N8},{A,O4,N8},{E,O4,N8},{F,O4,N8},
                             {D,O4,N4},{D,O4,N4},
                             {D,O4,N8},{F,O4,N8},{E,O4,N8},{D,O4,N8},
                             {C,O4,N8},{G,O4,N8},{D,O4,N8},{E,O4,N8},
                             {C,O4,N4},{G,O4,N4},
                             {C,O4,N4},{C,O4,N4},
                             {D,O4,N8},{A,O4,N8},{A,O4,N8},{B,O4,N8},
                             {A,O4,N8},{G,O4,N8},{G,O4,N8},{B,O4,N8},
                             /* 45 */
                             {C,O5,N8},{E,O5,N8},{G,O5,N8},{E,O5,N8},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{F,O4,N8},{F,O4,N8},{E,O5,N8},
                             {D,O5,N8},{G,O4,N8},{G,O4,N8},{B,O4,N8},
                             {B,O4,N8},{A,O4,N8},{B,O4,N8},{A,O4,N8},
                             {B,O4,N8},{A,O4,N8},{B,O4,N8},{A,O4,N8},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             /* 54 */
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{D,O5,N4},
                             {E,O5,N8},{D,O5,N8},{E,O5,N8},{D,O5,N8},
                             {E,O5,N8},{D,O5,N8},{E,O5,N8},{D,O5,N8},
                             {G,O4,N2},
                             {A,O4,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {D,O5,N4},{D,O5,N4},
                             /* 64 */
                             {D,O5,N8},{E,O5,N8},{B,O4,N8},{C,O5,N8},
                             {A,O4,N4},{A,O4,N4},
                             {A,O4,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N8},{G,O5,N8},{F,O5,N8},{E,O5,N8},
                             {D,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N2},
                             {A,O4,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {D,O5,N4},{D,O5,N4},
                             {D,O5,N8},{E,O5,N8},{B,O4,N8},{C,O5,N8},
                             /* 73 */
                             {A,O4,N4},{A,O4,N4},
                             {A,O4,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N8},{D,O5,N8},{A,O4,N8},{B,O4,N8},
                             {G,O4,N4},{D,O4,N4},
                             {G,O4,N4},{G,O4,N4},
                             {A,O4,N4},{B,O4,N4},
                             {D,O5,N8},{C,O5,N8},{F,O5,N8},{E,O5,N8},
                             {D,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N4},{G,O4,N4},
                             {A,O4,N4},{B,O4,N4},
                             /* 83 */
                             {D,O5,N8},{C,O5,N8},{F,O5,N8},{E,O5,N8},
                             {D,O5,N8},{C,O5,N8},{B,O4,N8},{A,O4,N8},
                             {G,O4,N8},{D,O5,N8},{A,O4,N8},{B,O4,N8},
                             {G,O4,N8},{D,O5,N8},{A,O4,N8},{B,O4,N8},
                             {G,O4,N8},{D,O5,N8},{A,O4,N8},{B,O4,N8},
                             {G,O4,N8},{D,O5,N8},{A,O4,N8},{B,O4,N8},
                             {G,O4,N4},{REST,O4,N4},
                             {G,O4,N4},{G,O5,N4},
                             {G,O4,N4},{REST,O4,N4},
                             {REPEAT,O4,N1}
                };
                
                
/* Fall */
#define D7 11
#define R7 60
static SND_PLAY_NOTE s7[] = {{D,O4,N16},
                             {STOP,O4,N1}
                };
                
/* Destroy */
#define D8 11
#define R8 60
static SND_PLAY_NOTE s8[] = {{D,O4,N16},{D,O3,N16},
                             {STOP,O4,N1}
                };
                
#endif
