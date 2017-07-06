/*
*       wait_tc.c
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


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "wait_tc.h"


static JOB_T *job_q = NULL;
#define calc_endwait(ms) (clock() + ((double) ms) * CLK_TCK / 1000.0)


void w_wait(long ms) {
        clock_t endwait;
        JOB_T *j, *pq;

        endwait = calc_endwait(ms);
        while (1) {
                pq = job_q;
                j = NULL;
                while (pq != NULL) {
                        if ((j == NULL) || (j->endwait > pq->endwait)) {
                                j = pq;
                        } 
                        pq = pq->next;
                }
                if ((j != NULL) && (clock() >= j->endwait) && (endwait > j->endwait)) {
                        j->run();
                        j->endwait = calc_endwait(j->period);
                } else 
                if (clock() >= endwait) {
                        return;
                }
        }
}


JOB_T * w_register_job(unsigned ms, void (*run)(void)) {
        JOB_T *j;
        
        j = (JOB_T *) malloc(sizeof(JOB_T));
        assert(j != NULL);
        
        j->run = run;
        j->period = ms;
        j->endwait = calc_endwait(ms);
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

