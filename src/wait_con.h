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

#include <sys/time.h>

enum W_ACTION {
        RUN,
        RESET,
        PAUSE,
        UNPAUSE
};

typedef struct JOB JOB_T;
struct JOB {
        long            (*run)(enum W_ACTION);
        long            period;   /* in ms */
        struct timeval  endwait;
        int             priority; /* priority: <0 low, =0 normal, >0 high */
        JOB_T           *prev;
        JOB_T           *next;
};

#define DOS_CLK_TCK 18.2
#define w_ms_to_tck(ms) (((double) ms) * DOS_CLK_TCK / 1000.0)
#define w_tck_to_ms(tck) (55 * (tck)) /* 1000 / DOS_CLK_TCK = 54.94 */

extern void w_wait(clock_t tck);
extern JOB_T * w_register_job(clock_t tck, int priority, long (*run)(enum W_ACTION));
extern void w_unregister_job(JOB_T *j);

#endif

