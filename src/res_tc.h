/*
*       res_tc.h
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
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


#ifdef _WAIT_TC_

static char *floating_text = "Ptakovina game was created in 2017 "
        "as part of developers competition published at www.high-voltage.cz. "
        "I would like thank to Sledge for making such challenges and for " 
        "inspiring us creating new DOS games. Also I would like thank to web " 
        "www.chris.com and to all ascii art creators such as jgs, mrf, as, lc " 
        "and many others for their wonderful ascii creatures. " 
        "This software consists of three games: 1) ADDTRIS is game invented " 
        "in 2016 by Vasek Petourka. Original game was published at www.8bity.cz. "
        "2) TETRIS is well known game invented in 1984 by Russian game " 
        "designer Alexey Pajitnov. 3) X-TRIS is my own crazy modification of " 
        "Tetris game. You can change some parameters by yourself and " 
        "create crazy challenge to manage. This software is developed and " 
        "published as open source under GNU/GPL version 3 license. All sources " 
        "are available at github.com/berk76/tetris Enjoy the game! - berk -";
#endif        
        
static char *addtris_help =
        "\x01\x0f\n"
        "The target of Addtris game is to put number\n" 
        "above two others so that value of number at\n" 
        "the top is sum of two underlying. If the sum\n"
        "of two lower is for ex. 14 you should put 4\n"
        "above them.\n"
        "\n"
        "For controls use arrows or keys 7,8,9,4.\n" 
        "By arrow up you can change value of falling\n"
        "number as to be completion to 10.\n";


static char *tetris_help =
        "\x01\x0f\n"
        "The target of Tetris game is to fill and clear\n" 
        "horizontal lines by manipulating of falling objects.\n" 
        "Line is cleared at the moment when every square\n" 
        "of the row is filled.\n"
        "\n"
        "For control of falling objects use arrows or\n" 
        "keys 7,8,9,4.\n";


static char *xtris_help =
        "\x01\x0f\n"
        "The target of X-Tris game is to fill and clear\n" 
        "horizontal lines by manipulating of falling objects.\n" 
        "Line is cleared at the moment when every square\n" 
        "of the row is filled.\n"
        "\n"
        "For control of falling objects use arrows or\n" 
        "keys 7,8,9,4.\n";


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

char *gfx_bird_03 = "\x01\x06                ,\n" \
                    " ,_     ,     .'<_\n" \
                    "_> `'-,'(__.-' __<\n" \
                    ">_.--(\x01\x0b..\x01\x06 )  =;`\n" \
                    "     `\x01\x0cV\x01\x06-'`'\\/``";

char *gfx_bird_06 = "\x01\x08   .-.\n" \
                    "  /\x01\x0e'v'\x01\x08\\\n" \
                    " (/   \\)\n" \
                    "\x01\x06=\x01\x08'\x01\x06=\x01\x0e\"\x01\x06=\x01\x0e\"\x01\x06===\n" \
                    "   \x01\x08|_|";
                     
char *gfx_bird_08 = "\x01\x0a  ()\n" \
                    "<)-`\\()\n" \
                    "  <)_>\x01\x06==\x01\x0a\n" \
                    " <>-'`(>";

char *gfx_bird_09 = "\x01\x0a    ()\n" \
                    " ()/'-(>\n" \
                    "\x01\x06==\x01\x0a<_(>\n" \
                    "<)'`-<>";

char *gfx_bird_10 = "\x01\x0f .-.\n" \
                    "(. .)__.')\n" \
                    "/ \x01\x0cV\x01\x0f      )\n" \
                    "\\  (   \\/\n" \
                    " `._`._ \\\n" \
                    "\x01\x06===\x01\x04<<\x01\x06==\x01\x0f`'\x01\x06=";

char *gfx_bird_11 = "\x01\x0f      .-.\n" \
                    "(`.__(. .)\n" \
                    "(      \x01\x0cV\x01\x0f \\\n" \
                    " \\/   )  /\n" \
                    " / _.'_.'\n" \
                    "\x01\x06=\x01\x0f`'\x01\x06==\x01\x04>>\x01\x06===";

char *gfx_owl_02 =  "\x01\x06  >\\_/<\n" \
                    " _\\\x01\x07*\x01\x06v\x01\x07*\x01\x06/_\n" \
                    " \\\\   //\n" \
                    "====\"====\n" \
                    "   /^\\";
                    
char *gfx_owl_03 =  "\x01\x07 6\x01\x06')     \n" \
                    " ('\\\\     \n" \
                    "  `\x01\x07n\x01\x06\\\\\n" \
                    "   O  \\";

char *gfx_owl_04 =  "\x01\x07 6\x01\x06'),,,,,\n" \
                    " ( \\\\\\\\`\n" \
                    "  `\x01\x07n\x01\x06\\\\\n" \
                    "   O  \\";


char *gfx_mush_01 = "\x01\x04  .-\"\"\"-.\n" \
                    " /\x01\x07* * * *\x01\x04\\\n" \
                    ":_.-:`:-._;\n" \
                    "    \x01\x07(_)\n" \
                    " \x01\x02\\|/\x01\x07(_)\x01\x02\\|/";
                           
char *gfx_witch_01 = "\x01\x0e(       \"     )\n" \
                     " ( _  \x01\x0f*\x01\x0e\n" \
                     "    \x01\x0f*\x01\x0e (     /           \x01\x01___\n" \
                     "\x01\x0e       \"     \"        \x01\x01_/ /\n" \
                     "\x01\x0e      (   \x01\x0f*\x01\x0e  )    \x01\x01___/   |\n" \
                     "\x01\x0e        )   \"     \x01\x05_\x01\x0boo\x01\x05)\x01\x01_'-./__\n" \
                     "\x01\x0e       \x01\x0f*\x01\x0e  _ )    \x01\x05(_, . \x01\x08$$$\n" \
                     "\x01\x0e       (  )   \x01\x05__ \x01\x04__\x01\x05 7_ \x01\x08$$$$\n" \
                     "\x01\x0e        ( :  \x01\x05{ _)\x01\x04  '---  \x01\x08$\x01\x04\\\n" \
                     "\x01\x08   ______\x01\x0e'\x01\x08___\x01\x06//\x01\x08__\x01\x04\\   ____, \\\n" \
                     "\x01\x08    )           ( \x01\x04\\_/ _____\\_\n" \
                     "\x01\x08  .'             \\\x01\x04   \\------\x01\x01''.\n" \
                     "\x01\x08  |='           '=|\x01\x01  |         )\n" \
                     "\x01\x08  |               |\x01\x01  |  .    _/\n" \
                     "\x01\x08   \\    \x01\x0c(. ) ,   \x01\x08/\x01\x01  /__I_____\\\n" \
                     "\x01\x08    '._\x01\x0c/_)_(\\\x01\x08__.'   \x01\x05(__,(__,_]\n" \
                     "\x01\x06   @---\x01\x0c()\x01\x0e_.'\x01\x06---@";

char *gfx_rabbit_01 = "\x01\x06   \\(\x01\x0a,%%*%,\x01\x06_\n" \
                      " \x01\x0a%%\x01\x06/*(\n" \
                      "\x01\x0a%%\x01\x06 )\\|\n" \
                      " _/ #)\n" \
                      ".) ,/\n" \
                      "/)#(_\x01\x0a,,,,,,,";

char *gfx_rabbit_02 = "\x01\x06-\x01\x0a%*%%,\x01\x06)/\n" \
                      "     )*\\\x01\x0a%%\x01\x06\n" \
                      "     |/( \x01\x0a%%\x01\x06\n" \
                      "     (# \\_\n" \
                      "      \\, (.\n" \
                      "\x01\x0a,,,,,,\x01\x06_)#(\\";

char *gfx_rabbit_03 = "\x01\x0f      /)\n" \
                      "     (/ \n" \
                      "   __/')\n" \
                      " _/  /  \n" \
                      "\x01\x0a,\x01\x0f\\\x01\x0a,\x01\x0f((\x01\x0a,,,";
                      
char *gfx_rabbit_031 = "\x01\x0f      /)\n" \
                      "     (/ \n" \
                      "   __/')\n" \
                      "  /  /  \n" \
                      "\x01\x0a,\x01\x0f/\x01\x0a,\x01\x0f((\x01\x0a,,,";
 
char *gfx_rabbit_04 = "\x01\x0f  /)    \n" \
                      " (/     \n" \
                      "('\\__   \n" \
                      "  \\  \\_ \n" \
                      "\x01\x0a,,,\x01\x0f))\x01\x0a,\x01\x0f/\x01\x0a,";
                      
char *gfx_rabbit_041 = "\x01\x0f  /)    \n" \
                      " (/     \n" \
                      "('\\__   \n" \
                      "  \\  \\  \n" \
                      "\x01\x0a,,,\x01\x0f))\x01\x0a,\x01\x0f\\\x01\x0a,";
                      
char *gfx_rabbit_05 = "\x01\x07        //\n" \
                      "      _//\n" \
                      "     .. ~~-_\n" \
                      "\x01\x04___\x01\x07m<\x01\x04___\x01\x07m\x01\x04___\x01\x07~.\n" \
                      "\x01\x04_|__|__|__|__|__|\n" \
                      "|__|__|__|__|__|_";

char *gfx_plant_03 = "\x01\x0e                 _\n" \
                     "\x01\x0e               _(_)_\n" \
                     "\x01\x0c   @@@@       \x01\x0e(_)\x01\x0c@\x01\x0e(_)  \x01\x01vVVVv    \x01\x0e_\n" \
                     "\x01\x0c  @@\x01\x0e()\x01\x0c@@ \x01\x0dwWWWw  \x01\x0e(_)\x01\x0a\\   \x01\x01(___)  \x01\x0e_(_)_\n" \
                     "\x01\x0c   @@@@  \x01\x0d(___)   \x01\x0a  `|/   Y   \x01\x0e(_)\x01\x0c@\x01\x0e(_)\n" \
                     "\x01\x0a    /      Y       \\|   \\|/   /\x01\x0e(_)\n" \
                     "\x01\x0a \\ |     \\ |/       | /\\ | / \\|/\n" \
                     "\x01\x02jgs\x01\x0a|///  \\\\|/// \\\\\\\\|//\\\\|///\\|///\n" \
                     "\x01\x0a^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";

char *gfx_plant_04 = "\x01\x0d       wWWWw   \x01\x0c_\n" \
                     "\x01\x0d @@@@  (___) \x01\x0c_(_)_\n" \
                     "\x01\x0d@@\x01\x0e()\x01\x0d@@   \x01\x0aY  \x01\x0c(_)\x01\x0e@\x01\x0c(_)\n" \
                     "\x01\x0d @@@@   \x01\x0a\\|/   \x01\x0c(_)\x01\x0a\\\n" \
                     "\x01\x0a \\|      |/      |\n" \
                     "\x01\x0a  |/    \\|      \\|/\n" \
                     "\x01\x0a\\\\|//  \\\\|//  \\\\\\|/\n" \
                     "\x01\x0a^^^^^^^^^^^^^^^^^^^";
                      
char *gfx_butter_02 = "\x01\x0f __   __\n" \
                      "(  \\\x01\x0a,\x01\x0f/  )\n" \
                      " \\_ \x01\x0a|\x01\x0f _/\n" \
                      " (_/ \\_)";

char *gfx_butter_03 = "\x01\x01 ___   ___ \n" \
                      "\x01\x01(\x01\x0c(\x01\x0eo\x01\x0c)\x01\x01\\\x01\x07,\x01\x01/\x01\x0c(\x01\x0eo\x01\x0c)\x01\x01)\n" \
                      "\x01\x01 \\__ \x01\x07|\x01\x01 __/\n" \
                      "\x01\x01  {_/\x01\x07'\x01\x01\\_}";

char *gfx_bee_01 = "\x01\x07  // \\\n" \
                   "  \\\\_/ \x01\x08//\n" \
                   " -\x01\x0e(||)\x01\x08(') \n" \
                   "  '''";

char *gfx_bee_02 = "\x01\x07    // \\\n" \
                   "\x01\x08 \\\\\x01\x07 \\\\_/\n" \
                   "\x01\x08 (')\x01\x0e(||)\x01\x08- \n" \
                   "     '''";

char *gfx_bee_04 = "\x01\x07      ___ _\n" \
                   "     /.'/./\n" \
                   "    /'./'/\n" \
                   "\x01\x08 \\\\_\x01\x07\\:/./\x01\x0e__\n" \
                   "\x01\x08 (\")__)\x01\x0e}}}}}\x01\x08- \n" \
                   "\x01\x08  ^<<< \x01\x0e~~~~";

char *gfx_spray_01 = "\x01\x0f    _ \n" \
                     "   |0|\n" \
                     "\x01\x07.--`+'--.\n" \
                     "|\x01\x07`-----'|\n" \
                     "\x01\x04|       |\n" \
                     "|\x01\x07  .-.  \x01\x04|\n" \
                     "|\x01\x07 (0.0) \x01\x04|\n" \
                     "|\x01\x07 >\x01\x0e|=|\x01\x07< \x01\x04|\n" \
                     "|\x01\x07  `\"`  \x01\x04|\n" \
                     "|       |\n" \
                     "\x01\x07|       |\n" \
                     "`-.___.-'";

char *gfx_spray_02 = "       . '  .\n" \
                     "    ' .( '.) '\n" \
                     "  ('-.)' (`'.) '\n" \
                     "-(. ')`( .-`) (-')\n" \
                     "   .  (' -,).(') .\n" \
                     "    (' .) - ('. )\n" \
                     "     . (' `.  )\n" \
                     "        ` .  `";

#ifdef _SOUND_TC_

/*
*       Sounds
*/


/* Holka modrooka */
#define SD2 17
#define SR2 60
static SND_PLAY_NOTE s2[] = {{G5,N4},{A5,N4},{G5,N8},{G5,N8},{E5,N8},{G5,N8}, 
                       {F5,N8},{F5,N8},{D5,N8},{F5,N8},{G5,N8},{G5,N8},{E5,N8},{F5,N8}, 
                       {G5,N4},{A5,N4},{G5,N8},{G5,N8},{E5,N8},{G5,N8}, 
                       {F5,N8},{F5,N8},{D5,N8},{F5,N8},{E5,N4},{REST,N4}, 
                       {F5,N8},{F5,N8},{D5,N8},{F5,N8},{G5,N8},{G5,N8},{E5,N8},{G5,N8}, 
                       {F5,N8},{F5,N8},{D5,N8},{F5,N8},{G5,N8},{G5,N8},{E5,N8},{F5,N8}, 
                       {G5,N4},{A5,N4},{G5,N8},{G5,N8},{E5,N8},{G5,N8}, 
                       {F5,N8},{F5,N8},{D5,N8},{F5,N8},{E5,N4},{REST,N4}, 
                       {REST,N2}, 
                       {REPEAT,N2} 
                };


/* Saxana */
#define SD4 17
#define SR4 60
static SND_PLAY_NOTE s4[] = {{C4,N4},{A4,N4},{G4,N4},{REST,N4}, 
                       {E4,N16},{E4,N8},{E4,N16},{E4,N64},{D4,N16},{C4,N8},{E4,N4},{D4,N4},
                       {C4,N4},{A4,N4},{G4,N4},{REST,N4},
                       {E4,N8},{E4,N16},{E4,N16},{D4,N8},{D4,N8},{C4,N4},{REST,N4},
                       {C4,N4},{A4,N4},{G4,N4},{REST,N4},
                       {E4,N16},{E4,N8},{E4,N16},{E4,N64},{D4,N16},{C4,N8},{E4,N4},{D4,N4},
                       {C4,N4},{A4,N4},{G4,N4},{REST,N4},
                       {E4,N8},{E4,N16},{E4,N16},{D4,N8},{D4,N8},
                       {C4,N4},{REST,N4},{REST,N2},
                       {C4,N16},{C4,N8},{C4,N16},{C4,N16},{C4,N8},{C4,N16},{E4,N16},{E4,N16},{E4,N8},{E4,N16},{E4,N8},{E4,N16},
                       {F4,N16},{F4,N16},{F4,N8},{F4,N16},{F4,N8},{F4,N16},{A4,N4},{A4,N16},{A4,N8},{B4,N16},
                       {B4,N2},{REST,N16},{B4,N16},{B4,N8},
                       {B4,N1},
                       {C4,N16},{C4,N8},{C4,N16},{C4,N16},{C4,N8},{C4,N16},{E4,N16},{E4,N16},{E4,N8},{E4,N16},{E4,N8},{E4,N16},
                       {F4,N16},{F4,N16},{F4,N8},{F4,N16},{F4,N8},{F4,N16},{A4,N4},{A4,N16},{A4,N8},{B4,N16},
                       {B4,N2},{REST,N16},{B4,N16},{B4,N8},
                       {B4,N1},
                       {REPEAT,N32} 
                };


/* Dance of The Trolls */
#define SD5 17
#define SR5 60
static SND_PLAY_NOTE s5[] = {{F5,N8},{E5,N8},{C5,N8},{B4,N8},{A4,N4},{A4,N4},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{D5,N4},{D5,N8},{REST,N8},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{A4,N4},{A4,N4},
                               {B4,N8},{C5,N8},{D5,N8},{G4,N8},{A4,N4},{A4,N8},{REST,N8},
                               {A4,N4},{B4,N4},{C5,N8},{D5,N8},{E5,N4},
                               {E5,N8},{D5,N8},{C5,N4},{B4,N4},{B4,N4},
                               {A4,N4},{B4,N4},{C5,N8},{D5,N8},{E5,N4},
                               {E5,N8},{C5,N8},{B4,N8},{C5,N8},{A4,N4},{A4,N4},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{D5,N4},{D5,N8},{REST,N8},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{A4,N4},{A4,N4},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{D5,N4},{D5,N8},{REST,N8},
                               {B4,N8},{C5,N8},{D5,N8},{G4,N8},{A4,N4},{A4,N8},{REST,N8},
                               {A4,N4},{B4,N4},{C5,N8},{D5,N8},{E5,N4},
                               {E5,N8},{D5,N8},{C5,N4},{B4,N4},{B4,N4},
                               {A4,N4},{B4,N4},{C5,N8},{D5,N8},{E5,N4},
                               {E5,N8},{C5,N8},{B4,N8},{C5,N8},{A4,N4},{E5,N8},{F5,N8},
                               {E5,N4},{E5,N8},{F5,N8},{D5,N4},{D5,N8},{E5,N8},
                               {C5,N4},{C5,N8},{D5,N8},{B4,N4},{B4,N8},{C5,N8},
                               {E5,N8},{C5,N8},{B4,N8},{C5,N8},{A4,N4},{A4,N4},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{D5,N4},{D5,N8},{REST,N8},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{A4,N4},{A4,N4},
                               {F5,N8},{E5,N8},{C5,N8},{B4,N8},{D5,N4},{D5,N8},{REST,N8},
                               {B4,N8},{C5,N8},{D5,N8},{G4,N8},{A4,N4},{A4,N8},{REST,N8},
                               {REPEAT,N16}
                };
                
                
/* Can Can */
#define SD6 11
#define SR6 60
static SND_PLAY_NOTE s6[] = {{D4,N8},{A4,N8},{A4,N8},{B4,N8},
                             {A4,N8},{G4,N8},{G4,N8},{B4,N8},
                             {C5,N8},{E5,N8},{G5,N8},{E5,N8},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{F4,N8},{F4,N8},{E5,N8},
                             {D5,N8},{G4,N8},{G4,N8},{B4,N8},
                             {B4,N8},{A4,N8},{B4,N8},{A4,N8},
                             {B4,N8},{A4,N8},{B4,N8},{A4,N8},
                             /* 09 */
                             {A4,N8},{G4,N8},{G4,N4},
                             {B4,N4},{G4,N4},
                             {E4,N4},{D4,N4},
                             {D4,N8},{A4,N8},{B4,N8},{C5,N8},
                             {B4,N8},{A4,N8},{G4,N4},
                             {B4,N4},{G4,N4},
                             {E4,N4},{D4,N4},
                             {C4S,N8},{D4,N8},{E4,N8},{F4,N8},
                             {A4,N8},{G4,N8},{G4,N4},
                             /* 18 */
                             {G4,N8},{D4,N8},{F4,N8},{D4,N8},
                             {G4,N8},{D4,N8},{F4,N8},{D4,N8},
                             {G4,N8},{D4,N8},{F4,N8},{D4,N8},
                             {G4,N8},{D4,N8},{F4,N8},{D4,N8},
                             {G4,N8DOT},{G4,N16},{G4,N8DOT},{G4,N16},
                             {G4,N8DOT},{G4,N16},{G4,N8DOT},{G4,N16},
                             {G4,N8DOT},{G4,N16},{G4,N8DOT},{G4,N16},
                             {G4,N8DOT},{G4,N16},{G4,N8DOT},{G4,N16},
                             /* 26 */
                             {C4,N2},
                             {D4,N8},{F4,N8},{E4,N8},{D4,N8},
                             {G4,N4},{G4,N4},
                             {G4,N8},{A4,N8},{E4,N8},{F4,N8},
                             {D4,N4},{D4,N4},
                             {D4,N8},{F4,N8},{E4,N8},{D4,N8},
                             {C4,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N8},{F4,N8},{E4,N8},{D4,N8},
                             {C4,N2},
                             {D4,N8},{F4,N8},{E4,N8},{D4,N8},
                             /* 36 */
                             {G4,N4},{G4,N4},
                             {G4,N8},{A4,N8},{E4,N8},{F4,N8},
                             {D4,N4},{D4,N4},
                             {D4,N8},{F4,N8},{E4,N8},{D4,N8},
                             {C4,N8},{G4,N8},{D4,N8},{E4,N8},
                             {C4,N4},{G4,N4},
                             {C4,N4},{C4,N4},
                             {D4,N8},{A4,N8},{A4,N8},{B4,N8},
                             {A4,N8},{G4,N8},{G4,N8},{B4,N8},
                             /* 45 */
                             {C5,N8},{E5,N8},{G5,N8},{E5,N8},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{F4,N8},{F4,N8},{E5,N8},
                             {D5,N8},{G4,N8},{G4,N8},{B4,N8},
                             {B4,N8},{A4,N8},{B4,N8},{A4,N8},
                             {B4,N8},{A4,N8},{B4,N8},{A4,N8},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             /* 54 */
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{D5,N4},
                             {E5,N8},{D5,N8},{E5,N8},{D5,N8},
                             {E5,N8},{D5,N8},{E5,N8},{D5,N8},
                             {G4,N2},
                             {A4,N8},{C5,N8},{B4,N8},{A4,N8},
                             {D5,N4},{D5,N4},
                             /* 64 */
                             {D5,N8},{E5,N8},{B4,N8},{C5,N8},
                             {A4,N4},{A4,N4},
                             {A4,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N8},{G5,N8},{F5,N8},{E5,N8},
                             {D5,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N2},
                             {A4,N8},{C5,N8},{B4,N8},{A4,N8},
                             {D5,N4},{D5,N4},
                             {D5,N8},{E5,N8},{B4,N8},{C5,N8},
                             /* 73 */
                             {A4,N4},{A4,N4},
                             {A4,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N8},{D5,N8},{A4,N8},{B4,N8},
                             {G4,N4},{D4,N4},
                             {G4,N4},{G4,N4},
                             {A4,N4},{B4,N4},
                             {D5,N8},{C5,N8},{F5,N8},{E5,N8},
                             {D5,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N4},{G4,N4},
                             {A4,N4},{B4,N4},
                             /* 83 */
                             {D5,N8},{C5,N8},{F5,N8},{E5,N8},
                             {D5,N8},{C5,N8},{B4,N8},{A4,N8},
                             {G4,N8},{D5,N8},{A4,N8},{B4,N8},
                             {G4,N8},{D5,N8},{A4,N8},{B4,N8},
                             {G4,N8},{D5,N8},{A4,N8},{B4,N8},
                             {G4,N8},{D5,N8},{A4,N8},{B4,N8},
                             {G4,N4},{REST,N4},
                             {G4,N4},{G5,N4},
                             {G4,N4},{REST,N4},
                             {REPEAT,N1}
                };

#endif                
#endif

