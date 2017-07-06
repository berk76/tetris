/*
*       wait_tc.h
*
*       This file is part of Tetris game.
*       https://bitbucket.org/berk76/tetris
*
*       Tetris is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 3 of the License, or
*       (at your option) any later version. <http://www.gnu.org/licenses/>
*
*       Written by Jaroslav Beran <jaroslav.beran@gmail.com>, on 6.7.2017
*/


#ifndef _WAIT_TC_
#define _WAIT_TC_

#include <time.h>


typedef struct JOB JOB_T;
struct JOB {
        void       (*run)();
        long       period;
        clock_t    endwait;
        JOB_T      *prev;
        JOB_T      *next;
};


extern void w_wait(long ms);
extern JOB_T * w_register_job(unsigned ms, void (*run)(void));
extern void w_unregister_job(JOB_T *j);

#endif
