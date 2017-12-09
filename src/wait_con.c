/*
*       wait_con.c
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


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wait_con.h"


static JOB_T *job_q = NULL;


static void add_ms(struct timeval *tv, long ms);
static long tv_diff_ms(struct timeval *start, struct timeval *end);


void w_wait(clock_t tck) {
        clock_t ret;
        JOB_T *j, *pq;
        struct timeval  endwait, tv_current;

        add_ms(&endwait, w_tck_to_ms(tck));
        #define PRIORITY 0
        
        while (1) {
                gettimeofday(&tv_current, NULL);
                pq = job_q;
                j = NULL;
                while (pq != NULL) {
                        if ((j == NULL) ||
                            (tv_diff_ms(&(pq->endwait), &(j->endwait)) > 0) ||
                            ((tv_diff_ms(&(pq->endwait), &(j->endwait)) == 0) && (j->priority < pq->priority))) {
                                j = pq;
                        } 
                        pq = pq->next;
                }
                if ((j != NULL) && 
                    (tv_diff_ms(&(j->endwait), &tv_current) >= 0) &&
                    ((tv_diff_ms(&(j->endwait), &endwait) > 0) || ((tv_diff_ms(&(j->endwait), &endwait) == 0) && (PRIORITY < j->priority)))) {
                        ret = j->run(RUN);
                        switch (ret) {
                                case -1:
                                        w_unregister_job(j);
                                        break;
                                case 0:
                                        add_ms(&(j->endwait), j->period);
                                        break;
                                default:
                                        add_ms(&(j->endwait), w_tck_to_ms(ret));
                        }

                } else { 
                        if (tv_diff_ms(&endwait, &tv_current) >= 0) {
                                return;
                        } else {
                                usleep(tv_diff_ms(&tv_current, &endwait) * 1000);
                        }
                }
        }
}


JOB_T * w_register_job(clock_t tck, int priority, long (*run)(enum W_ACTION)) {
        JOB_T *j;

        j = (JOB_T *) malloc(sizeof(JOB_T));
        assert(j != NULL);
        
        j->run = run;
        j->run(RESET);
        j->priority = priority;
        j->period = w_tck_to_ms(tck);
        add_ms(&(j->endwait), j->period);
        j->prev = NULL;
        j->next = job_q;
        job_q = j;
        if (j->next != NULL) {
                (j->next)->prev = j;        
        } 
        
        return j;
}


void w_unregister_job(JOB_T *j) {
        assert(j != NULL);
        
        if (j->prev != NULL) {
                (j->prev)->next = j->next;
                if (j->next != NULL) {
                        (j->next)->prev = j->prev;
                }
        } else {
                job_q = j->next;
                if (j->next != NULL) {
                        (j->next)->prev = NULL;
                } 
        }
        
        free((void *) j);
}


void add_ms(struct timeval *tv, long ms) {
        long s, usec;

        gettimeofday(tv, NULL);
        usec = tv->tv_usec + (ms * 1000);
        s = usec / 1000000;

        tv->tv_usec = usec % 1000000;
        tv->tv_sec = tv->tv_sec + s;
}


long tv_diff_ms(struct timeval *start, struct timeval *end) {
        long result;
        long seconds; 
        long useconds;

        seconds  = end->tv_sec  - start->tv_sec;
        useconds = end->tv_usec - start->tv_usec;
        result = (seconds*1000 + useconds/1000);

        return result;
}

