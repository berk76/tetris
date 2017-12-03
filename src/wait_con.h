/*
*       wait_con.h
*
*       This file is part of Tetris game.
*       https://github.com/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 3.12.2017
*/


#ifndef _WAIT_CON_
#define _WAIT_CON_

#include <time.h>

enum W_ACTION {
        RUN,
        RESET,
        PAUSE,
        UNPAUSE
};

typedef struct JOB JOB_T;
struct JOB {
        long       (*run)(enum W_ACTION);
        long       period;
        clock_t    endwait;
        int        priority; /* priority: <0 low, =0 normal, >0 high */
        JOB_T      *prev;
        JOB_T      *next;
};

#define CLK_TCK 18.2
#define w_ms_to_tck(ms) (((double) ms) * CLK_TCK / 1000.0)
#define w_tck_to_clocks(tck) (tck * (((double) CLOCKS_PER_SEC) / ((double) CLK_TCK)))

extern void w_wait(long tck);
extern JOB_T * w_register_job(long tck, int priority, long (*run)(enum W_ACTION));
extern void w_unregister_job(JOB_T *j);

#endif

