/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

/*=============================================================================
======  0. DATASET TYPE  ======================================================
=============================================================================*/

#ifndef TASK
#ifdef BASIC_VECTOR

typedef struct Task Task; 
struct Task {
    charss negpoints; // shape nbs_neg[task] x pt_dim
    charss pospoints; // shape nbs_pos[task] x pt_dim
    int pt_dim;
}; 

typedef struct TaskView TaskView; 
struct TaskView {
    charss negpoints;   // shape nbs_neg[task] x pt_dim
    charss pospoints;   // shape nbs_pos[task] x pt_dim
    int pt_dim;
}; 

#define TASK
#endif//BASIC_VECTOR
#endif//TASK

#ifndef TASK_METHODS
#ifdef TASKS

void populate_math(Tasks* tsp);
void print_math(TaskView const* tvp);
void free_task(Task* tp);

int len_taskview(TaskView const* tvp);
void empt_taskview(TaskView* tvp, int pt_dim, int cap);
void cons_taskview(TaskView* tvp, Task const* tp);
void rand_taskview(TaskView* tvp, Task const* tp);
void wipe_taskview(TaskView* tvp);

#define TASK_METHODS
#endif//TASKS
#endif//TASK_METHODS
