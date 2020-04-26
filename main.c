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
#include "containers/fixpoint.h"
#include "containers/mapping.h"
#include "containers/count_heap.h"

float mylog2(float x)
{
    // assume x >= 1.0
    float rtrn = 0.0;
    while ( x > 2.0 ) { x/=2; rtrn+=1.0; }
    float dx = x-1.0; 
    return rtrn + (dx - dx*dx/2 + dx*dx*dx/3)*1.442;
}

void main()
{
    Tasks tasks;
    populate_math(&tasks);

    srand(time(0));

    TaskView tv;

    for (int ii=0; ii!=40; ++ii) { 
        Trees trees;
        init_trees(&trees, 24);

        int nodes = 0;
        int leaves = 0;
        for (int ti=0; ti!=24; ++ti) {
            //printf("task %2d: ", ti);
            rand_taskview(&tv, &(tasks.data[ti]));
            //printf("info %f\n", info_of(&tv));
            //print_math(&tv);

            DecTree dt;
            init_tree(&dt);
            train_tree(&tv, &dt);
            push_trees(&trees, dt);

            //printf(" %2d leaves\n", nb_leaves(&dt));
            if (ii%4==0 && ti==4) {
                //print_tree(&dt);
                //printf("\n");
                //WAIT_FOR_ENTER();
            }

            nodes += nb_nodes(&dt);
            leaves+= nb_leaves(&dt);
            wipe_taskview(&tv);
        }
        int nb_dims = 7+ii; 
        if (ii%1==0) {
            printf("%4d nodes; %4d leaves; %5d  bits    ",
                    nodes, leaves, (int)(leaves + mylog2(nb_dims)*(nodes + 2*(nb_dims-7))));
        }

        float score;
        NewDim nd = best_new_dim(&tasks, &trees, 100, nb_dims, &score); 
        printf("%3d: \033[32m%3d %3s %3d : %6.1f\033[36m    ",
            nb_dims, nd.didx_a, (
                nd.op==OP_AND    ? "and" :
                nd.op==OP_OR     ? "or" :
                nd.op==OP_XOR    ? "xor" :
                                   "->"
            ), nd.didx_b, score
        );

        add_new_dim(&tasks, &nd);
        cons_taskview(&tv, &(tasks.data[0]));
        print_math_feature(&tv, nb_dims); printf("\n");
        wipe_taskview(&tv);
        //printf("%d\n", tasks.data[0].pt_dim);


        free_trees(&trees);
    }

    free_tasks(&tasks);
}
