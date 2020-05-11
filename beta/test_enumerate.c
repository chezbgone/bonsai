/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-05-10
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "lambda.h"
#include "enumerator.h"

const float exp_thouth = 1.001000500;
const float exp_hundth = 1.010050167;
const float exp_tenth  = 1.105170918;
const float exp_one    = 2.718281828;

float plog(float p)
{
    float bulk = 0.0;
    while ( p < 1.0 ) { p *= exp_one   ; bulk -= 1.   ; }
    while ( 1.0 < p ) { p /= exp_tenth ; bulk +=  .1  ; }
    while ( p < 1.0 ) { p *= exp_hundth; bulk -=  .01 ; } 
    while ( 1.0 < p ) { p /= exp_thouth; bulk +=  .001; }

    float d = 1.0-p; 
    float remainder = d*(1/1.0 + d*(1/2.0 + d/3.0));
    return bulk - remainder; 
}

char leaf_names[][16] = {"zero","succ","pred","times"};

void main()
{
    init_lamb_expr_pool();

    float leaf_scores[] = {plog(0.5), plog(0.333), plog(0.333), plog(0.9999)};
    EType leaf_types[] = {TINT, TINT_FRM_INT, TINT_FRM_INT, TINT_FRM_INT_FRM_INT};
    Grammar G = {
        .nb_leaves = 4,
        .leaf_scores = leaf_scores,
        .leaf_types  = leaf_types,
        .eval_score  = {99, plog(0.5), plog(0.333)}
    };

    LambList ll = enumerate(&G, -13, TINT); 
    //for ( int pi = 0; pi != ll.len; ++pi ) {
    //    print_expr(ll.arr[pi].e, leaf_names);
    //    printf("\n");
    //}
    printf("%d elts\n", ll.len);
    free(ll.arr);


    free_lamb_expr_pool();
}

