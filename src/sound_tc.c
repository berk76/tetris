/*
*       sound_tc.c
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 8.7.2017
*/

/*

 See following articles:
 -----------------------
 https://web.archive.org/web/20140307023908/http://fly.srk.fer.hr/GDM/articles/sndmus/speaker1.html
 https://web.archive.org/web/20140307043446/http://fly.srk.fer.hr/GDM/articles/sndmus/speaker2.html
 
 
 The three system timers
 =======================
 Whenever your computer is operating, there are three "timers" being controlled 
 by various chips inside your computer. These are unimaginatively known as 
 timers 0, 1 and 2.

 The frequency at which each timer oscillates is determined by a delay value. 
 The idea is that each timer counts down from this delay value to zero, and 
 when it reaches zero, the timer oscillates. In doing so it raises a signal 
 to let other parts of the computer "know" it has oscillated.

 The counter is then re-set to the predetermined value and the process starts 
 again. Each timer maintains its own independant count-down value, meaning that 
 each timer can run at a different frequency.

 The counting down process is controlled by the main system oscillator, which 
 runs at a frequency of 1,193,180 Hz, or 1.19318 MHz (MegaHertz). Every time 
 this oscillates, each one of the system timers counts down once. This has 
 nothing to do with the speed of your processor (25Mhz, 33Mhz, etc) - this 
 timer runs at exactly the same speed in *every* PC.

 To vary the frequency at which the timers oscillate, you just need to give 
 them a new count-down value. The two formulas used are easily determined:
 
              1193180 
 COUNTDOWN = ---------
             FREQUENCY
             
             
              1193180 
 FREQUENCY = ---------
             COUNTDOWN
             
 The countdown value can be any value from 1 to 65,535, giving a range of 
 available frequencies of 1,193,180Hz to 18.2Hz.
 
 Timer 0 is the main system timer. It's configured to oscillate 18.2 times 
 every second, by default (therefore, the countdown value is 65,535 from the 
 above formula!).

 Whenever timer 0 'ticks', interrupt 8h is generated. Among other things, 
 interrupt 8h is responsible for keeping the clock in your computer going.

 This means that if you change the countdown value for timer 0, say to make it 
 run twice as fast (by halving the countdown value), that your system clock 
 will also run twice as fast. This becomes important later on when we play 
 digitised sounds, when we may run the timer hundreds or even *thousands* of 
 times faster than normal!

 Timer 1 is used to regularly refresh the contents of your RAM. This timer is 
 of no value to us, but I list it here for completeness. You should NOT mess 
 around with this timer unless you like your system to crash often.

 Timer 2 is used to control sound generation. By varying the frequency at which 
 timer 2 oscillates, we can vary the frequency of sound being emitted from the 
 speaker. This requires us to tell the computer to "attach" the speaker to 
 timer 2, as you'll see.   

 Enough theory, I want to make some noise!
 =========================================
 OK, hang on, first you've got to know how to modify the countdown value for 
 timer 2. For the first time in the magazine, we're going to have to go 
 directly to the hardware ports to do this.

 [Hardware ports are a bit like pigeon holes for the hardware - by putting 
 certain values into certain ports, we can communicate directly with the 
 hardware. Sometimes the hardware returns values to us which we can retrieve 
 by reading other ports. You'll see more and more examples of port usage as the 
 magazine goes on, particularly from me when I'm running late on a deadline 
 <hic> <grin>]

 If you haven't modified ports directly before, you may not know how to do so 
 with your language. I can tell you that with C you can use the 'outport' and 
 'outportb' functions. With assembler, use the 'OUT' mnemonic. For other 
 languages, I'm afraid you'll have to consult your manual - please let me know 
 what you find.

 To tell timer 2 that you want to modify the countdown value, you first have to 
 tell it you're about to do so. You then send the new value as two bytes, the 
 low byte first and the high byte second.

 [NB: The pseudocode representation of sending bytes to a port is the OUT 
 command, where we OUT PORT,VALUE. To read a value from a port we use 
 VALUE = IN(PORT)]

 First we have to tell timer 2 that we're about to load a new countdown value, 
 which we do by sending the value B6h (dec 182) to port 43h (dec 67). ie:
 
 OUT 43h, B6h
 
 Then, in two consecutive statements, we must send the low byte and high byte 
 of the new countdown value, but we send them to port 42h, not port 43h - watch 
 this, it's a common programming mistake!

 For example, if our low and high bytes are 54 and 124 respectively, as in our 
 example above, then we do:
 
 OUT 42h, 54
 OUT 42h, 124
 
 (Remember, you are not setting the frequency here, you are setting the 
 countdown value! This is another common programming mistake! Use the formula 
 above to determine the countdown value required for the desired frequency, or 
 vice versa.)

 As soon as we have done this, the new countdown value takes effect. However, 
 before this will make any noise, we have to tell the CPU that we want to 
 "connect" the speaker to timer 2, so that every time timer 2 oscillates, so 
 does the speaker, producing a "click".

 To do this, we must set bits 0 and 1 of the value on port 61h on. Cue 
 pseudocode:

 VALUE = IN( 61h )
 VALUE = VALUE OR 3      (Turn on bits 1 and 2)
 OUT 61h, VALUE

 [If you don't understand the second line, look under 'OR' in the index of your 
 manual. While you're at it, read about 'AND' also because we're going to use 
 that shortly]

 To "disconnect" the speaker from timer 2, we need to clear bits 1 and 2 of the 
 value on port 61h:

 VALUE = IN( 61h )
 VALUE = VALUE AND 252
 OUT 61h, VALUE

 Note that this connection or disconnection stays put until told otherwise. 
 That is, once you've connected the speaker to timer 2, you can change the 
 frequency as often as you like without doing the connection again.

 If you wrote a program to do the above, you would now find that your speaker 
 is oscillating at (1193180/31798) hertz = 37.5Hz. That's just between D and 
 D sharp on the first octave of a piano. (I've included a table of frequencies 
 at the end of the article).

 To play a little tune, then, we just set up the frequency for a note, wait for 
 a little while, then set up the frequency for the next note, and so on.

 To turn the speaker off again, just disconnect it from timer 2 as above, or 
 set the frequency to something inaudible (this is cheating but has the same 
 effect).

 To make sound effects, well, there are loads of possibilities. You can quickly 
 move back and forth between two frequencies to generate an "alarm" style sound 
 effect, or you can glide between one frequency and another and back again to 
 generate a "phaser".
 
 The possibilities are limitless, and experimentation is definitely the name of 
 the game. You'll be amazed at the sounds you can make if you experiment with 
 changing frequencies rapidly.
 
 TABLE OF MUSICAL NOTE FREQUENCIES (Hz)
 ======================================
 Octave 0    1    2    3    4    5    6    7
 Note
 C     16   33   65  131  262  523 1046 2093
 C#    17   35   69  139  277  554 1109 2217
 D     18   37   73  147  294  587 1175 2349
 D#    19   39   78  155  311  622 1244 2489
 E     21   41   82  165  330  659 1328 2637
 F     22   44   87  175  349  698 1397 2794
 F#    23   46   92  185  370  740 1480 2960
 G     24   49   98  196  392  784 1568 3136
 G#    26   52  104  208  415  831 1661 3322
 A     27   55  110  220  440  880 1760 3520
 A#    29   58  116  233  466  932 1865 3729
 B     31   62  123  245  494  988 1975 3951   
 
*/

#include <stdio.h>
#include <dos.h>
#include "sound_tc.h"

/* Please note frequencies 16 and 17 are out of possibility of PC speaker */
static int not_freq[] = {  16,  17,  18,  19,  21,  22,  23,  24,  26,  27,  29,  31, \
                    33,  35,  37,  39,  41,  44,  46,  49,  52,  55,  58,  62, \
                    65,  69,  73,  78,  82,  87,  92,  98, 104, 110, 116, 123, \
                   131, 139, 147, 155, 165, 175, 185, 196, 208, 220, 233, 245, \
                   262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, \
                   523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, \
                  1046,1109,1175,1244,1328,1397,1480,1568,1661,1760,1865,1975, \
                  2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951};
       
/* Song for play */           
static SND_SONG *song = NULL;

/*
* Set frequency of oscillator feeding speaker.
*/
 
void snd_setfreq(int hertz) {
        unsigned countdown; 
        
        countdown = 1193180L/hertz;
        outportb(0x43, 0xB6);
        outportb(0x42, countdown & 0377);
        outportb(0x42, countdown >> 8);
}
 
 
/*
* Turn speaker on or off.
*/
 
void snd_speaker(int on) {
        int portval;
 
        portval = inportb(0x61);
        if (on) {
                portval |= 03;
        } else {
                portval &= 252;
        }
        outportb(0x61, portval);
}


/*      
* Play note
*/      
        
void snd_playnote(enum SND_NOTE n) {
        if ((n == REST) || (n == REPEAT)) {
                snd_speaker(0);
        } else {
                snd_setfreq(not_freq[n]);
                snd_speaker(1);
        }
}


/*      
* Setup song
*/

void snd_setsong(SND_SONG *s) {
        song = s;
        snd_play_sound(RESET);
}


/*      
* Play song
*/

long snd_play_sound(enum W_ACTION a) {
        long i;
        static int pause = 0;
        static SND_PLAY_NOTE *p = NULL;
        static paused = 0;
        
        if (song == NULL)
                return 0;
                                        
        if (a == RESET) {
                p = song->song;
                return 0;        
        }
        
        if (a == PAUSE) {
                paused = 1;
                snd_speaker(0);
        }
        
        if (a == UNPAUSE) {
                paused = 0;
        }
        
        if (a == RUN) {
                if (paused != 0)
                        return 0;
                        
                if (p == NULL) {
                        p = song->song;
                }
                if (pause == 0) {
                        snd_playnote(p->note);
                        i = p->duration;
                        switch (p->note) {
                                case REPEAT:
                                        p = song->song;
                                        break;
                                case STOP:
                                        snd_speaker(0);
                                        /* unregister job */
                                        return -1;
                                default:
                                        p++;
                        }
                        pause = 1;
                } else {
                        snd_playnote(REST);
                        pause = 0;
                }

                return (pause == 1) ? w_mstotck(song->duration*i) : w_mstotck(song->rest);
        }
        
        return 0;
}

