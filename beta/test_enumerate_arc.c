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
    float score;
    float weight;
};

const float ABST_PROB = 0.9;

const int NB_PRIMITIVES = 32;
Primitive my_prims[] = {
    {"not"      , TBOOLBOOL                 , 99 , 1         },
    {"and"      , TBOOLBOOL_BOOL            , 99 , 1         },
    {"or"       , TBOOLBOOL_BOOL            , 99 , 1         },
    {"eq_dir"   , TBOOLDRCT_DRCT            , 99 , 1         },
    {"eq_color" , TBOOLCOLOR_COLOR          , 99 ,     16    },

    //{"eq_cell"  , TBOOLCELL_CELL            , 99 , 1         },
    {"dummy"    , TBOOL                     , 99 , 1         },

    {"base"     , TCELL                     , 99 ,        64 }, 
    {"offset"   , TCELL_CELL_DRCT           , 99 ,     16    }, 
    {"scan"     , TCELL_CELL_DRCT_BOOLCELL  , 99 ,   4       }, 

    {"view"     , TCOLOR_CELL               , 99 ,     16    },
    {"outside"  , TCOLOR                    , 99 ,   4       },
    {"black"    , TCOLOR                    , 99 ,   4       },
    {"gray"     , TCOLOR                    , 99 ,   4       },
    {"blue"     , TCOLOR                    , 99 , 1         },
    {"brown"    , TCOLOR                    , 99 , 1         },
    {"green"    , TCOLOR                    , 99 , 1         },
    {"orange"   , TCOLOR                    , 99 , 1         },
    {"purple"   , TCOLOR                    , 99 , 1         },
    {"red"      , TCOLOR                    , 99 , 1         },
    {"teal"     , TCOLOR                    , 99 , 1         },
    {"yellow"   , TCOLOR                    , 99 , 1         }, 

    {"diff"     , TDRCT_CELL_CELL           , 99 , 1         },
    {"negate"   , TDRCT_DRCT                , 99 , 1         },
    {"plus"     , TDRCT_DRCT_DRCT           , 99 ,   4       },
    {"east"     , TDRCT                     , 99 ,     16    },
    {"north"    , TDRCT                     , 99 ,     16    },
    {"west"     , TDRCT                     , 99 ,     16    },
    {"south"    , TDRCT                     , 99 ,     16    },
    {"n_east"   , TDRCT                     , 99 ,   4       },
    {"n_west"   , TDRCT                     , 99 ,   4       },
    {"s_west"   , TDRCT                     , 99 ,   4       },
    {"s_east"   , TDRCT                     , 99 ,   4       },
};
float eval_scores[NB_TYPES];
char leaf_names[][16] = {
    "not"    , "and"    , "or"     , "eq_dir" , "eq_color" , "dummy"   ,
    "base"   , "offset" , "scan"   , "view"   , "outside"  , "black"   ,
    "gray"   , "blue"   , "brown"  , "green"  , "orange"   , "purple"  ,
    "red"    , "teal"   , "yellow" , "diff"   , "negate"   , "plus"    ,
    "east"   , "north"  , "west"   , "south"  , "n_east"   , "n_west"  ,
    "s_west" , "s_east"   
};

bool is_const[] = {
    true     , true     , true     , true     , true       , true      ,
    false/**/, true     , true     , true     , true       , true      ,
    true     , true     , true     , true     , true       , true      ,
    true     , true     , true     , true     , true       , true      ,
    true     , true     , true     , true     , true       , true      ,
    true     , true
};

bool needs_nonconst[] = {
    false    , false    , false    , true/**/ , true/**/   , true/**/  ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , true/**/ , false      , false     ,
    false    , false    , false    , false    , false      , false     ,
    false    , false
};

bool commutes[] = {
    false    , true/**/ , true/**/ , true/**/ , true/**/   , true/**/  ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , false    , false      , true/**/  ,
    false    , false    , false    , false    , false      , false     ,
    false    , false
};

bool needs_unequal[] = {
    false    , true/**/ , true/**/ , true/**/ , true/**/   , true/**/  ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , false    , false      , false     ,
    false    , false    , false    , true/**/ , false      , false     ,
    false    , false    , false    , false    , false      , false     ,
    false    , false
};

void initialize_primitive_scores()
{
    float weight_to[NB_TYPES];

    for ( int t = 0; t != NB_TYPES; ++t ) { weight_to[t] = 0.0; }

    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive p = my_prims[l];
        EType t = p.type; 
        while ( t != -1 ) { 
            weight_to[t] += p.weight;
            t = out_type[t];
        }; 
    }

    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive* p = &(my_prims[l]);
        EType t = p->type;
        p->score = plog( p->weight / weight_to[t] );
    }

    for ( int t = 0; t != NB_TYPES; ++t) {
        eval_scores[t] = plog(
            ( ( t == TBOOLCELL ) ? (1-ABST_PROB) : 1.0 ) *
            ( is_func[t] ? ( weight_to[t] / weight_to[out_type[t]]) : 0.0 )
        );
    }
}

void main()
{
    initialize_primitive_scores();
    //for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
    //    Primitive p = my_prims[l];
    //    printf("%10s : %f\n", p.name, p.score);
    //}

    init_lamb_expr_pool();

    float leaf_scores[NB_PRIMITIVES];
    EType leaf_types[NB_PRIMITIVES];
    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        leaf_scores[l] = my_prims[l].score;
        leaf_types[l] = my_prims[l].type;
    }
    Grammar G = {
        .nb_leaves   = NB_PRIMITIVES,
        .leaf_scores = leaf_scores,
        .leaf_types  = leaf_types,
        .is_const    = is_const,
        .needs_nonconst = needs_nonconst,
        .commutes    = commutes,
        .needs_unequal = needs_unequal,

        .abst_score = plog(ABST_PROB),
    };
    for ( int t = 0; t != NB_TYPES; ++t ) {
        G.eval_score[t] = eval_scores[t];
        printf("::%d %f\n", t, G.eval_score[t]);
    }

    fprintf(stderr, "hi!\n");

    LambList ll = enumerate(&G, -11, TCELL); 
    for ( int pi = 0; pi != ll.len; ++pi ) {
        lava();
        printf("%8.4f ", ll.arr[pi].score);
        print_expr(ll.arr[pi].e, leaf_names);
        printf("\n");
    }
    printf("%d elts\n", ll.len);
    free(ll.arr);

    free_lamb_expr_pool();
}



