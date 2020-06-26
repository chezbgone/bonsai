/*  author: samtenka
 *  change: 2020-06-26
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

typedef struct Grammar Grammar;
struct Grammar {
    int nb_types;

    struct {
        int len;
        LambExpr** exprs;
    } library;

    int len;
    int nb_irreds;
    EType const* leaf_types; 
    int const* arity;

    int const*   counts;
    float const* leaf_scores; 
    float const* eval_score;
    float        abst_score;

    bool const* is_const;
    bool const* needs_nonconst;
    bool const* commutes;
    bool const* needs_unequal;
    bool const* absorbs_self;
};

void scores_from_counts(Grammar* G); 

/*===========================================================================*/
/*====  1. LIBRARY  =========================================================*/
/*===========================================================================*/

#endif//GRAMMAR_H

