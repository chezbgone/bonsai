/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include "dataset.h"

int len_taskview(TaskView tv)
{
    return (
        tv.negpoints.len +
        tv.pospoints.len
    );
}

void cons_taskview(TaskView* tvp, Tasks ts, int tidx)
{
    charss negpoints = ts.negpointss.data[tidx];
    charss pospoints = ts.pospointss.data[tidx];

    tvp->pt_dim = ts.pt_dim;
    init_charss(&(tvp->negpoints), negpoints.len);
    init_charss(&(tvp->pospoints), pospoints.len);

    chars* point;
    for each(point, negpoints) {
        push_charss(&(tvp->negpoints), *point);
    }
    for each(point, pospoints) {
        push_charss(&(tvp->pospoints), *point);
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

void free_tasks(Tasks* tsp)
{
    free_charsss(&(tsp->pospointss));
    free_charsss(&(tsp->negpointss));
    tsp->pt_dim=0;
    tsp->nb_tasks=0;
}
