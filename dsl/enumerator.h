/*  author: samtenka
 *  change: 2020-06-26
 *  create: 2020-05-02
 *  descrp: interface for bottom-up program enumeration
 *  to use: 
 */

#ifndef ENUMERATOR_H 
#define ENUMERATOR_H 

#include <stdbool.h>

#include "../lambda/lambda.h" 
#include "../lambda/type.h" 

/*===========================================================================*/
/*====  0. GRAMMAR and PROGRAM LIST  ========================================*/
/*===========================================================================*/

typedef struct Library Library;
struct Library {
    int len;
    LambExpr* e;
};

typedef struct Grammar Grammar;
struct Grammar {
    int len;
    int nb_irreds;

    Library lib;

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

