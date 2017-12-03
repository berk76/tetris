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
#include "wait_con.h"


static JOB_T *job_q = NULL;


void w_wait(long tck) {
        long ret, endwait;
        JOB_T *j, *pq;

        tck = w_tck_to_clocks(tck);
        
        /* debug part */
        /*
        int i;
        i = 0;
        pq = job_q;
        while (pq != NULL) {
                i++;
                pq = pq->next;
        }
        gotoxy(1,25);
        printf("%d", i);
        gotoxy(1,25);
        */
        
        endwait = tck + clock();
        #define PRIORITY 0
        
        while (1) {
                pq = job_q;
                j = NULL;
                while (pq != NULL) {
                        if ((j == NULL) || 
                            (j->endwait > pq->endwait) || 
                            ((j->endwait == pq->endwait) && (j->priority < pq->priority))) {
                                j = pq;
                        } 
                        pq = pq->next;
                }
                if ((j != NULL) && 
                    (clock() >= j->endwait) && 
                    ((endwait > j->endwait) || ((endwait == j->endwait) && (PRIORITY < j->priority)))) {
                        ret = j->run(RUN);
                        switch (ret) {
                                case -1:
                                        w_unregister_job(j);
                                        break;
                                case 0:
                                        j->endwait = j->period + clock();
                                        break;
                                default:
                                        j->endwait = ret + clock();
                        }

                } else 
                if (clock() >= endwait) {
                        return;
                }
        }
}


JOB_T * w_register_job(long tck, int priority, long (*run)(enum W_ACTION)) {
        JOB_T *j;

        tck = w_tck_to_clocks(tck);        

        j = (JOB_T *) malloc(sizeof(JOB_T));
        assert(j != NULL);
        
        j->run = run;
        j->run(RESET);
        j->priority = priority;
        j->period = tck;
        j->endwait = tck + clock();
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

