/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include "../containers/fixpoint.h"

int len_taskview(TaskView const* tvp)
{
    return (
        tvp->negpoints.len +
        tvp->pospoints.len
    );
}

void cons_taskview(TaskView* tvp, Task const* tp)
{
    charss negpoints = tp->negpoints;
    charss pospoints = tp->pospoints;

    tvp->pt_dim = tp->pt_dim;
    init_charss(&(tvp->negpoints), negpoints.len);
    init_charss(&(tvp->pospoints), pospoints.len);

    chars* point;
    for each (point, negpoints) {
        push_charss(&(tvp->negpoints), *point);
    }
    for each (point, pospoints) {
        push_charss(&(tvp->pospoints), *point);
    }
}

void rand_taskview(TaskView* tvp, Task const* tp)
{
    charss negpoints = tp->negpoints;
    charss pospoints = tp->pospoints;

    tvp->pt_dim = tp->pt_dim;
    init_charss(&(tvp->negpoints), negpoints.len);
    init_charss(&(tvp->pospoints), pospoints.len);

    int len = negpoints.len + pospoints.len;
    chars* point;
    for (int i=0; i!=len; ++i) {
        int idx = rand()%len; 
        if (idx < negpoints.len) {
            point = negpoints.data + idx;
            push_charss(&(tvp->negpoints), *point);
        } else {
            point = pospoints.data + (idx-negpoints.len);
            push_charss(&(tvp->pospoints), *point);
        }
    }
}

void empt_taskview(TaskView* tvp, int pt_dim, int cap)
{
    tvp->pt_dim = pt_dim;
    init_charss(&(tvp->negpoints), cap);
    init_charss(&(tvp->pospoints), cap);
}

void wipe_taskview(TaskView* tvp)
{
    wipe_charss(&(tvp->negpoints));
    wipe_charss(&(tvp->pospoints));
}

void free_task(Task* tp)
{
    free_charss(&(tp->pospoints));
    free_charss(&(tp->negpoints));
    tp->pt_dim=0;
}

char compute_new_dim(chars const* point, NewDim const* nd) 
{
    char a = point->data[nd->didx_a];
    char b = point->data[nd->didx_b];
    switch ( nd->op ) {
        case OP_AND:        return (   a  & b)?1:0; 
        case OP_OR:         return (   a  | b)?1:0;
        case OP_XOR:        return (   a  ^ b)?1:0;
        case OP_IMPLIES:    return ((1^a) | b)?1:0;
    }
}

void add_new_dim(Tasks* tsp, NewDim const* nd)
{
    Task* tp;
    for each (tp, *tsp) {
        chars* point;
        for each (point, tp->negpoints) {
            push_chars(point, compute_new_dim(point, nd));
        }
        for each (point, tp->pospoints) {
            push_chars(point, compute_new_dim(point, nd));
        }
        tp->pt_dim += 1;
    }
}
