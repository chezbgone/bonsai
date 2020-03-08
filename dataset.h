/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#ifndef DATASET_H 
#define DATASET_H 

#include "vector.h"

/*=============================================================================
======  0. DATASET TYPE  ======================================================
=============================================================================*/

typedef struct {
    charsss negpointss; // shape nb_tasks x nbs_neg[task] x pt_dim
    charsss pospointss; // shape nb_tasks x nbs_pos[task] x pt_dim
    int pt_dim;
    int nb_tasks;
} Tasks; 

typedef struct {
    charss negpoints;   // shape nbs_neg[task] x pt_dim
    charss pospoints;   // shape nbs_pos[task] x pt_dim
    int pt_dim;
} TaskView; 

void populate_math(Tasks* tsp);
void print_math(TaskView t);
void free_tasks(Tasks* tsp);

int len_taskview(TaskView tvp);
void empt_taskview(TaskView* tvp, int pt_dim, int cap);
void cons_taskview(TaskView* tvp, Tasks ts, int tidx);
void wipe_taskview(TaskView* tvp);

#endif//DATASET_H
