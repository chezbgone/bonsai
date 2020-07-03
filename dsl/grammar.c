/*  author: samtenka
 *  change: 2020-07-01
 *  create: 2020-06-26
 *  descrp: implementation for grammar
 *  to use: 
 */

#include <math.h>
#include <stdlib.h>
#include "../dsl/grammar.h" 

/*===========================================================================*/
/*====  0. GRAMMAR SET UP  ==================================================*/
/*===========================================================================*/

float plog(float p);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.0. Allocate (and Free)  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void alloc_grammar(Grammar* G, int nb_prims, int nb_types)
{
    *G = (Grammar){
        .nb_types    = nb_types                                           ,
        .len         = nb_prims + 0                                       ,
        .primitives  = {.cap = nb_prims, .len = nb_prims, .elts = malloc(sizeof(Primitive)*nb_prims) },
        .concepts    = {.cap = 0       , .len = 0       , .elts = NULL}, 
        .leaf_scores = malloc(sizeof(float) * nb_prims)                   , 
        .eval_scores = malloc(sizeof(float) * nb_types)                   , 
    };
}
void free_grammar(Grammar* G)
{
    free(G->leaf_scores);
    free(G->eval_scores);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Compute Scores  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void scores_from_counts(Grammar* G, Primitive* prims, float abst_prob)
{
    G->primitives.elts = prims; 
    G->abst_score = plog(abst_prob); 

    float weight_to[NB_TYPES];

    /*----------------  0.1.0. accumulate weights through each type  --------*/
                        
    for ( int t = 0; t != G->nb_types; ++t ) {
        weight_to[t] = 0.0;
    }

    for ( int l = 0; l != G->len; ++l ) {
        Primitive p = prims[l];
        EType t = p.type; 
        while ( t != -1 ) { 
            weight_to[t] += p.count;
            t = out_type[t];
        }; 
    }

    /*----------------  0.1.1. score for creating t as a primitive  ---------*/

    for ( int l = 0; l != G->len; ++l ) {
        Primitive p = prims[l];
        EType t = p.type; 
        G->leaf_scores[l] = plog( p.count / weight_to[t] );
    }

    /*----------------  0.1.2. score for creating t by evaluating (t<-s)  ---*/

    for ( int t = 0; t != G->nb_types; ++t) {
        G->eval_scores[t] = plog(
               is_func[t] ? ( weight_to[t] / weight_to[out_type[t]])
                          : 0.0
        );
    }

    /*----------------  0.1.3. score for creating (t<-s) by abstracting s  --*/
    // TODO
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.2. Helpers  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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

/*===========================================================================*/
/*====  1. GRAMMAR USAGE  ===================================================*/
/*===========================================================================*/

int add_concept(LambExpr* e, Grammar* G)
{
    if ( G->concepts.len == G->concepts.cap ) {
        int new_cap = 1 + 2*G->concepts.cap;
        LambExpr** new_elts = malloc(sizeof(LambExpr*) * new_cap); 
        for ( int i=0; i!=G->concepts.len; ++i ) { 
            new_elts[i] = G->concepts.elts[i]; 
        }
        free(G->concepts.elts);
        G->concepts.elts = new_elts; 
    }
    G->concepts.elts[G->concepts.len] = e;
    return (G->concepts.len++);
}
