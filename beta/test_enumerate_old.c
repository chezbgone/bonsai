/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-05-10
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "enumerator.h"
#include "lambda.h"

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

char leaf_names[][16] = {"cow","raccoon","lump","fluffy","shivering","hugs","loves"};

void main()
{
    init_lamb_expr_pool();

    float leaf_scores[] = {plog(0.23), plog(0.12), plog(0.11), plog(0.4), plog(0.3), plog(0.4), plog(0.6)};
    EType leaf_types[] = {TINT, TINT, TINT, TINT_FRM_INT, TINT_FRM_INT, TINT_FRM_INT_FRM_INT, TINT_FRM_INT_FRM_INT};
    Grammar G = {
        .nb_leaves = 7,
        .leaf_scores = leaf_scores,
        .leaf_types  = leaf_types,
        .eval_score  = {0.0, plog(0.54), plog(0.7)}
    };

    LambList ll = enumerate(&G, -6, TINT); 
    for ( int pi = 0; pi != ll.len; ++pi ) {
        lava();
        printf("%8.4f ", ll.arr[pi].score);
        print_expr(ll.arr[pi].e, leaf_names);
        printf("%d\n", pi);

    }
    printf("%d elts\n", ll.len);
    free(ll.arr);


    free_lamb_expr_pool();
}

