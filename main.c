/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-04
 * descrp: 
 * to use: 
 */

#include <stdio.h>

#include "vector.h" 
#include "dataset.h" 
#include "dectree.h" 
#include "verbose.h"

#define NB_VECTORS 24

void main()
{
    Tasks ts;
    populate_math(&ts);

    TaskView tv;
    for (int ti=0; ti!=24; ++ti) {
        printf("task %2d: ", ti);
        cons_taskview(&tv, ts, ti);
        print_math(tv);

        DecTree* dtp = train_tree(tv);
        printf(" %2d leaves", nb_leaves(dtp));

        wipe_taskview(&tv);
        free_tree(dtp);
        WAIT_FOR_ENTER();
    }

    free_tasks(&ts);
}
