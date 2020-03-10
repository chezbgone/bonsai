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

//void print_pi(pint_by_int* pi, int depth)
//{
//    if ( pi->is_leaf ) { return; }
//    for (int d=0; d!=depth; ++d) {
//        printf(" ");
//    }
//    printf("%d : %d.%d\n", pi->key, pi->value.fst, pi->value.snd);
//    print_pi(pi->left, depth+1);
//    print_pi(pi->rght, depth+1);
//}
//void print_ip(int_by_pint* ip, int depth)
//{
//    if ( ip->is_leaf ) { return; }
//    for (int d=0; d!=depth; ++d) {
//        printf(" ");
//    }
//    printf("%d.%d : %d\n", ip->key.fst, ip->key.snd, ip->value);
//    print_ip(ip->left, depth+1);
//    print_ip(ip->rght, depth+1);
//}
//void print_moo(Counter* c)
//{
//    printf(":\n");
//    print_pi(c->counted_idxs_by_keys, 0);
//    print_ip(c->keys_by_counted_idxs, 0);
//}

void main()
{
    //Counter C;
    //init_counter(&C);

    //int i;

    //for (i=0; i!=6; ++i) { counter_observe(&C, 0); }
    //for (i=0; i!=4; ++i) { counter_observe(&C, 1); }
    //for (i=0; i!=3; ++i) { counter_observe(&C, 2); }
    //for (i=0; i!=1; ++i) { counter_observe(&C, 3); }
    //for (i=0; i!=5; ++i) { counter_observe(&C, 4); }
    //for (i=0; i!=2; ++i) { counter_observe(&C, 5); }
    //print_moo(&C);

    //for (i=0; i!=10; ++i) { counter_observe(&C, 1); }
    //print_moo(&C);

    //printf("%d", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();
    //printf("%d", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();
    //printf("%d", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();
    //printf("%d", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();
    //printf("%d", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();
    //printf("%d\n", pop_most_frequent(&C).fst); WAIT_FOR_ENTER();

    //free_counter(&C);

    Tasks tasks;
    populate_math(&tasks);

    srand(time(0));

    TaskView tv;

    for (int ii=0; ii!=50; ++ii) { 
        Trees trees;
        init_trees(&trees, 24);

        int nodes = 0;
        for (int ti=0; ti!=24 ; ++ti) {
            //printf("task %2d: ", ti);
            cons_taskview(&tv, &(tasks.data[ti]));
            //printf("info %f\n", info_of(&tv));
            //print_math(&tv);

            DecTree dt;
            init_tree(&dt);
            train_tree(&tv, &dt);
            push_trees(&trees, dt);

            //printf(" %2d leaves\n", nb_leaves(&dt));
            if (ii==49 && ti==6) {
            print_tree(&dt);
            //printf("\n");
            }

            nodes += nb_nodes(&dt);
            wipe_taskview(&tv);
        }
        printf("%d nodes\n", nodes);

        float score;
        NewDim nd = best_new_dim(&tasks, &trees, 100, 6+ii, &score); 
        //printf("!%d %s %d : %f\n", nd.didx_a, (
        //    nd.op==OP_AND    ? "and" :
        //    nd.op==OP_OR     ? "or" :
        //    nd.op==OP_XOR    ? "xor" :
        //                       "implies"
        //), nd.didx_b, score);

        add_new_dim(&tasks, &nd);
        //printf("%d\n", tasks.data[0].pt_dim);

        WAIT_FOR_ENTER();

        free_trees(&trees);
    }

    free_tasks(&tasks);
}
