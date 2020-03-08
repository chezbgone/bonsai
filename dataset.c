/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include "fixpoint.h"

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

void free_task(Task* tp)
{
    free_charss(&(tp->pospoints));
    free_charss(&(tp->negpoints));
    tp->pt_dim=0;
}
