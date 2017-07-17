/*
*       sound_tc.h
*
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 8.7.2017
*/


#ifndef _SOUND_TC_
#define _SOUND_TC_

#include "wait_tc.h"


enum SND_OCTAVE {
        O0 = 0,
        O1 = 1,
        O2 = 2,
        O3 = 3,
        O4 = 4,
        O5 = 5,
        O6 = 6,
        O7 = 7
};

enum SND_NOTE {
        C  = 0, 
        CS = 1,
        D  = 2,
        DS = 3,
        E  = 4,
        F  = 5,
        FS = 6,
        G  = 7,
        GS = 8,
        A  = 9,
        AS = 10,
        B  = 11,
        REST = 99,
        END = 100
};

enum SND_DURATION {
        N1DOT   = 192,
        N1      = 128, 
        N2DOT   = 96,
        N2      = 64,
        N4DOT   = 48,
        N4      = 32,
        N8DOT   = 24,
        N8      = 16,
        N16DOT  = 12,
        N16     = 8,
        N32DOT  = 6,
        N32     = 4,
        N64DOT  = 3,
        N64     = 2,
        N128    = 1
};

typedef struct {
        enum SND_NOTE note;
        enum SND_OCTAVE octave;
        enum SND_DURATION duration;
} SND_PLAY_NOTE;
      
typedef struct {
        int duration;
        int rest;
        SND_PLAY_NOTE *song;
} SND_SONG;
        
        
/*      
* Set frequency of oscillator feeding speaker.
*/      
        
extern void snd_setfreq(int hertz);
        
/*      
* Turn speaker on or off.
*/      
 
extern void snd_speaker(int on);

/*      
* Play note
*/      
        
extern void snd_playnote(enum SND_NOTE n, enum SND_OCTAVE o);

/*      
* Setup song
*/

extern void snd_setsong(SND_SONG *s);

/*      
* Play song
*/

extern long snd_play_sound(enum W_ACTION a);


#endif
