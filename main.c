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
#include "mapping.h"
#include "count_heap.h"

void print_moo(int_by_int* p)
{
    if ( p->is_leaf ) { return; }
    else {
        printf("%d:%d", p->key, p->value);
        printf("(");
        print_moo(p->left);
        printf(",");
        print_moo(p->rght);
        printf(")");
    }
}
void main()
{
    //int_by_int moo;
    //init_ibi(&moo);
    //setv_ibi(&moo, 2, 0);
    //setv_ibi(&moo, 1, 0);
    //setv_ibi(&moo, 4, 0);
    //setv_ibi(&moo, 5, 0);
    //setv_ibi(&moo, 3, 0);
    //print_moo(&moo);
    //printf("\n");
    //remk_ibi(&moo, 4);
    //print_moo(&moo);
    //printf("\n");
    //free_ibi(&moo);

    Counter C;
    init_counter(&C);

    counter_observe(&C, 3);
    counter_observe(&C, 3);

    counter_observe(&C, 4);
    counter_observe(&C, 4);
    counter_observe(&C, 4);
    counter_observe(&C, 4);

    counter_observe(&C, 5);
    counter_observe(&C, 5);
    counter_observe(&C, 5);

    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);

    counter_observe(&C, 0);
    counter_observe(&C, 1);
    counter_observe(&C, 2);
    counter_observe(&C, 30);
    counter_observe(&C, 20);
    counter_observe(&C, 10);

    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);
    printf("%d\n", pop_most_frequent(&C).fst);

    free_counter(&C);

    //Tasks tasks;
    //populate_math(&tasks);

    //srand(time(0));

    //TaskView tv;

    //for (int ii=0; ii!=50; ++ii) { 
    //    Trees trees;
    //    init_trees(&trees, 24);

    //    int nodes = 0;
    //    for (int ti=0; ti!=24 ; ++ti) {
    //        //printf("task %2d: ", ti);
    //        cons_taskview(&tv, &(tasks.data[ti]));
    //        //printf("info %f\n", info_of(&tv));
    //        //print_math(&tv);

    //        DecTree dt;
    //        init_tree(&dt);
    //        train_tree(&tv, &dt);
    //        push_trees(&trees, dt);

    //        //printf(" %2d leaves\n", nb_leaves(&dt));
    //        if (ii==49 && ti==6) {
    //        print_tree(&dt);
    //        //printf("\n");
    //        }

    //        nodes += nb_nodes(&dt);
    //        wipe_taskview(&tv);
    //    }
    //    printf("%d nodes\n", nodes);

    //    float score;
    //    NewDim nd = best_new_dim(&tasks, &trees, 100, 6+ii, &score); 
    //    //printf("!%d %s %d : %f\n", nd.didx_a, (
    //    //    nd.op==OP_AND    ? "and" :
    //    //    nd.op==OP_OR     ? "or" :
    //    //    nd.op==OP_XOR    ? "xor" :
    //    //                       "implies"
    //    //), nd.didx_b, score);

    //    add_new_dim(&tasks, &nd);
    //    //printf("%d\n", tasks.data[0].pt_dim);

    //    WAIT_FOR_ENTER();

    //    free_trees(&trees);
    //}

    //free_tasks(&tasks);
}
