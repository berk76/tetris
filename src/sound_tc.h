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


enum SND_NOTE {
        C0  = 0, 
        C0S = 1,
        D0  = 2,
        D0S = 3,
        E0  = 4,
        F0  = 5,
        F0S = 6,
        G0  = 7,
        G0S = 8,
        A0  = 9,
        A0S = 10,
        B0  = 11,
        
        C1  = 12, 
        C1S = 13,
        D1  = 14,
        D1S = 15,
        E1  = 16,
        F1  = 17,
        F1S = 18,
        G1  = 19,
        G1S = 20,
        A1  = 21,
        A1S = 22,
        B1  = 23,
        
        C2  = 24, 
        C2S = 25,
        D2  = 26,
        D2S = 27,
        E2  = 28,
        F2  = 29,
        F2S = 30,
        G2  = 31,
        G2S = 32,
        A2  = 33,
        A2S = 34,
        B2  = 35,
        
        C3  = 36, 
        C3S = 37,
        D3  = 38,
        D3S = 39,
        E3  = 40,
        F3  = 41,
        F3S = 42,
        G3  = 43,
        G3S = 44,
        A3  = 45,
        A3S = 46,
        B3  = 47,
        
        C4  = 48, 
        C4S = 49,
        D4  = 50,
        D4S = 51,
        E4  = 52,
        F4  = 53,
        F4S = 54,
        G4  = 55,
        G4S = 56,
        A4  = 57,
        A4S = 58,
        B4  = 59,
        
        C5  = 60, 
        C5S = 61,
        D5  = 62,
        D5S = 63,
        E5  = 64,
        F5  = 65,
        F5S = 66,
        G5  = 67,
        G5S = 68,
        A5  = 69,
        A5S = 70,
        B5  = 71,
        
        C6  = 72, 
        C6S = 73,
        D6  = 74,
        D6S = 75,
        E6  = 76,
        F6  = 77,
        F6S = 78,
        G6  = 79,
        G6S = 80,
        A6  = 81,
        A6S = 82,
        B6  = 83,
        
        C7  = 84, 
        C7S = 85,
        D7  = 86,
        D7S = 87,
        E7  = 88,
        F7  = 89,
        F7S = 90,
        G7  = 91,
        G7S = 92,
        A7  = 93,
        A7S = 94,
        B7  = 95,
        
        REST = 98,
        REPEAT = 99,
        STOP = 100
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
        enum SND_DURATION duration;
} SND_PLAY_NOTE;
      
typedef struct {
        long duration;
        long rest;
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
        
extern void snd_playnote(enum SND_NOTE n);

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