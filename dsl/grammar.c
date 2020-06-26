/*  author: samtenka
 *  change: 2020-06-26
 *  create: 2020-06-26
 *  descrp: implementation for grammar
 *  to use: 
 */

#include <math.h>
#include <stdlib.h>

#include "../lambda/grammar.h" 

float plog(float p);

void scores_from_counts(Grammar* G)
{
    float weight_to[NB_TYPES];

    /*----------------  0.0.0. accumulate weights through each type  --------*/
                        
    for ( int t = 0; t != G->nb_types; ++t ) {
        weight_to[t] = 0.0;
    }

    for ( int l = 0; l != G->len; ++l ) {
        Primitive p = my_prims[l];
        EType t = p.type; 
        while ( t != -1 ) { 
            weight_to[t] += G->count[p];
            t = G->out_type[t];
        }; 
    }

    /*----------------  0.0.1. score for creating t as a primitive  ---------*/

    for ( int l = 0; l != G->len; ++l ) {
        leaf_scores[l] = plog( G->count[p] / weight_to[t] );
    }

    /*----------------  0.0.2. score for creating t by evaluating (t<-s)  ---*/

    for ( int t = 0; t != G->nb_types; ++t) {
        eval_scores[t] = plog(
            G->is_func[t] ? ( weight_to[t] / weight_to[G->out_type[t]])
                          : 0.0
        );
    }

    /*----------------  0.0.3. score for creating (t<-s) by abstracting s  --*/
    // TODO
}

/*===========================================================================*/
/*====  1. HELPERS  =========================================================*/
/*===========================================================================*/

const float exp_thouth = 1.001000500;
const float exp_hundth = 1.010050167;
const float exp_tenth  = 1.105170918;
const float exp_one    = 2.718281828;

float plog(float p)
{
    if ( p == 0.0 ) { return -INFINITY; }
    float bulk = 0.0;
    while ( p < 1.0 ) { p *= exp_one   ; bulk -= 1.   ; }
    while ( 1.0 < p ) { p /= exp_tenth ; bulk +=  .1  ; }
    while ( p < 1.0 ) { p *= exp_hundth; bulk -=  .01 ; } 
    while ( 1.0 < p ) { p /= exp_thouth; bulk +=  .001; }

    float d = 1.0-p; 
    float remainder = d*(1/1.0 + d*(1/2.0 + d/3.0));
    return bulk - remainder; 
}


