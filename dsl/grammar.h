/*  author: samtenka
 *  change: 2020-07-03
 *  create: 2020-06-26
 *  descrp: interface for grammar type
 *  to use: 
 */

#ifndef GRAMMAR_H 
#define GRAMMAR_H 

#include <stdbool.h>

#include "../lambda/concept_table.h"
#include "../lambda/lambda.h" 
#include "../lambda/type.h" 

/*===========================================================================*/
/*====  0. GRAMMAR  =========================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.0. Data Structure  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Primitive Primitive;
struct Primitive {
    char name[16];
    int arity;
    EType type;

    float count;

    bool is_const;
    bool needs_nonconst;
    bool commutes;
    bool needs_unequal;
    bool absorbs_self;
};

typedef struct Grammar Grammar;
struct Grammar {
    int nb_types;

    int len;
    struct { int cap; int len; Primitive* elts; } primitives;
    struct { int cap; int len; LambExpr** elts; EType* types; } concepts;

    float* leaf_scores; 
    float* eval_scores;
    float  abst_score;
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Methods  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void alloc_grammar(Grammar* G, int nb_prims, int nb_types);
void free_grammar(Grammar* G);

/* initialize leaf_scores, eval_score, and abst_score */
void scores_from_counts(Grammar* G, Primitive* my_prims, float abst_prob); 

int add_concept(LambExpr* e, EType t, Grammar* G);

#endif//GRAMMAR_H

