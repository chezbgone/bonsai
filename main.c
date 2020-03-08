/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>

#include "verbose.h"
#include "fixpoint.h"

void main()
{
    Tasks ts;
    populate_math(&ts);

    TaskView tv;
    for (int ti=0; ti!=24; ++ti) {
        printf("task %2d: ", ti);
        cons_taskview(&tv, &(ts.data[ti]));
        print_math(&tv);

        DecTree* dtp = train_tree(&tv);
        printf(" %2d leaves\n", nb_leaves(dtp));

        //printf(" %f \n", gain_from_op(&tv, dtp, 0, 5, OP_XOR));
        //printf(" %f \n", gain_from_op(&tv, dtp, 0, 1, OP_XOR));
        //printf(" %f \n", gain_from_op(&tv, dtp, 0, 1, OP_AND));

        print_tree(dtp);

        wipe_taskview(&tv);
        free_tree(dtp);
        WAIT_FOR_ENTER();
    }

    free_tasks(&ts);
}
