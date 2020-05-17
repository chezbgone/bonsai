/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-05-02
 *  descrp: interface for bottom-up program enumeration
 *  to use: 
 */

#ifndef ENUMERATOR_H 
#define ENUMERATOR_H 

#include <stdbool.h>

#include "lambda.h" 
#include "type.h" 

/*===========================================================================*/
/*====  0. GRAMMAR and PROGRAM LIST  ========================================*/
/*===========================================================================*/

typedef struct Grammar Grammar;
struct Grammar {
    int nb_leaves;
    int const* nb_args;
    float const* leaf_scores; 
    EType const* leaf_types; 
    float const* eval_score;
    float abst_score;

    bool const* is_const;
    bool const* needs_nonconst;
    bool const* commutes;
    bool const* needs_unequal;
    bool const* absorbs_self;
};

typedef struct ScoredLamb ScoredLamb;
struct ScoredLamb {
    float score;
    LambExpr* e;

    // the following are useful during interpretation:
    bool is_const;
    bool needs_nonconst;
}; 

typedef struct LambList LambList; 
struct LambList {
    ScoredLamb* arr;   
    int len;
    int cap;
    int active_lo;
    int active_hi;
};

typedef struct LambsByEType LambsByEType;
struct LambsByEType { LambList arr[NB_TYPES]; };

LambList enumerate(Grammar const* G, float min_score, EType target);

#endif//ENUMERATOR_H

