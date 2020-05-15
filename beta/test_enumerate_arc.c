/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-05-10
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "enumerator.h"
#include "lambda.h"

const float exp_thouth = 1.001000500;
const float exp_hundth = 1.010050167;
const float exp_tenth  = 1.105170918;
const float exp_one    = 2.718281828;

float plog(float p)
{
    if ( p == 0.0 ) { return -9999.0; }
    float bulk = 0.0;
    while ( p < 1.0 ) { p *= exp_one   ; bulk -= 1.   ; }
    while ( 1.0 < p ) { p /= exp_tenth ; bulk +=  .1  ; }
    while ( p < 1.0 ) { p *= exp_hundth; bulk -=  .01 ; } 
    while ( 1.0 < p ) { p /= exp_thouth; bulk +=  .001; }

    float d = 1.0-p; 
    float remainder = d*(1/1.0 + d*(1/2.0 + d/3.0));
    return bulk - remainder; 
}

typedef struct Primitive Primitive;
struct Primitive {
    char name[16];
    EType type;
    float weight;
    bool is_const;
    bool needs_nonconst;
    bool commutes;
    bool needs_unequal;
    bool absorbs_self;
};

const float ABST_PROB = 0.0;

#define NB_PRIMITIVES 23
Primitive my_prims[] = {                         /*const  nnonc  comm   uneq   absbs*/
    {"here"     , tCEL                ,      64 , false, 1    , 0    , 0    , 0    }, 
    {"offset"   , tCEL_CEL_DIR        ,   16    , 1    , 1    , 0    , 0    , true }, 
    /*{"scan"     , tCEL_CEL_DIR_TWOCEL , 4       , 1    , 1    , 0    , 0    , 0    }, */
                  
    {"east"     , tDIR                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"north"    , tDIR                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"south"    , tDIR                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"west"     , tDIR                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"plus"     , tDIR_DIR_DIR        ,      64 , 1    , false, true , 0    , 0    },
    {"diff"     , tDIR_CEL_CEL        , 4       , 1    , 1    , 0    , true , 0    },
    {"negate"   , tDIR_DIR            , 4       , 1    , 1    , 0    , 0    , 0/**/},
                  
    {"black"    , tHUE                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"outside"  , tHUE                ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"view"     , tHUE_CEL            ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"blue"     , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"brown"    , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"gray"     , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"green"    , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"orange"   , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"purple"   , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"red"      , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"teal"     , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    },
    {"yellow"   , tHUE                ,   16    , 1    , 1    , 0    , 0    , 0    }, 
                  
    {"has_hue"  , tTWOCEL_HUE         ,      64 , 1    , 1    , 0    , 0    , 0    }, 
    {"sees"     , tTWOCEL_DIR_TWOCEL  ,   16    , 1    , 1    , 0    , 0    , 0    }, 
};
float eval_scores[NB_TYPES];
float leaf_scores[NB_PRIMITIVES];
EType leaf_types[NB_PRIMITIVES];

char leaf_names[NB_PRIMITIVES][16];
bool is_const[NB_PRIMITIVES];
bool needs_nonconst[NB_PRIMITIVES];
bool commutes[NB_PRIMITIVES];
bool needs_unequal[NB_PRIMITIVES];
bool absorbs_self[NB_PRIMITIVES];

void initialize_primitive_scores()
{
    float weight_to[NB_TYPES];

    fprintf(stderr, "hi!\n");
    for ( int t = 0; t != NB_TYPES; ++t ) { weight_to[t] = 0.0; }

    fprintf(stderr, "hi!\n");
    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive p = my_prims[l];
        EType t = p.type; 
        while ( t != -1 ) { 
            weight_to[t] += p.weight;
            t = out_type[t];
        }; 
    }

    fprintf(stderr, "hi!\n");
    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive* p = &(my_prims[l]);
        EType t = p->type;
        leaf_types[l] = t;
        leaf_scores[l] = plog( p->weight / weight_to[t] );

        strcpy(leaf_names[l], p->name); 
        is_const[l] = p->is_const;
        needs_nonconst[l] = p->needs_nonconst;
        commutes[l] = p->commutes;
        needs_unequal[l] = p->needs_unequal;
        absorbs_self[l] = p->absorbs_self;
    }

    fprintf(stderr, "hi!\n");
    for ( int t = 0; t != NB_TYPES; ++t) {
        eval_scores[t] = plog(
            /*  ( ( t == TTWOCEL ) ? (1-ABST_PROB) : 1.0 ) *   */
            ( is_func[t] ? ( weight_to[t] / weight_to[out_type[t]]) : 0.0 )
        );
    }
}

void main()
{
    initialize_primitive_scores();

    fprintf(stderr, "hi!\n");
    init_lamb_expr_pool();

    Grammar G = {
        .nb_leaves   = NB_PRIMITIVES,
        .leaf_scores = leaf_scores,
        .leaf_types  = leaf_types,
        .is_const    = is_const,
        .needs_nonconst = needs_nonconst,
        .commutes    = commutes,
        .needs_unequal = needs_unequal,
        .absorbs_self = absorbs_self,
        .abst_score = plog(ABST_PROB),
    };
    for ( int t = 0; t != NB_TYPES; ++t ) {
        G.eval_score[t] = eval_scores[t];
        printf("::%d %f\n", t, G.eval_score[t]);
    }

    fprintf(stderr, "hi!\n");

    LambList ll = enumerate(&G, -12, tTWO); 
    printf("%d elts\n", ll.len);
    for ( int pi = 0; pi != ll.len; ++pi ) {
        printf("%4d : ", pi);
        lava();
        printf("%8.4f ", ll.arr[pi].score);
        print_expr(ll.arr[pi].e, leaf_names);
        printf("\n");

        if ( (pi+1) % 50 ) { continue; }
        char c; scanf("%c", &c);
    }
    free(ll.arr);

    free_lamb_expr_pool();
}




