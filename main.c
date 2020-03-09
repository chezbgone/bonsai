/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "verbose.h"
#include "fixpoint.h"

void main()
{
    Tasks tasks;
    populate_math(&tasks);

    srand(time(0));

    Trees trees;
    init_trees(&trees, 24);

    TaskView tv;
    for (int ti=0; ti!=2 ; ++ti) {
        printf("task %2d: ", ti);
        rand_taskview(&tv, &(tasks.data[ti]));
        print_math(&tv);

        DecTree dt;
        init_tree(&dt);
        train_tree(&tv, &dt);
        push_trees(&trees, dt);

        printf(" %2d leaves\n", nb_leaves(&dt));

        for (int didx_a=0; didx_a!=6; ++didx_a) {
            for (int didx_b=didx_a+1; didx_b!=6; ++didx_b) {
                NewDim nd = {didx_a, didx_b, OP_XOR};
                float gain = gain_from_op(&tv, &dt, &nd);
                if (1.0 < gain) {
                    printf("%d %d  : \033[32m %+5.1f \033[36m \n", didx_a, didx_b, gain);
                } else if (gain < -1.0) {
                    printf("%d %d  : \033[31m %+5.1f \033[36m \n", didx_a, didx_b, gain);
                } else {
                    printf("%d %d  : \033[36m %+5.1f \033[36m \n", didx_a, didx_b, gain);
                }
            }
        }

        print_tree(&dt);

        wipe_taskview(&tv);
        WAIT_FOR_ENTER();
    }

    free_trees(&trees);
    free_tasks(&tasks);
}
