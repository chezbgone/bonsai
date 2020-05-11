/*  author: samtenka
 *  change: 2020-05-09
 *  create: 2020-05-09
 *  descrp: interface for bootom-up program enumeration
 *  to use: 
 */

#ifndef ENUMERATOR_H 
#define ENUMERATOR_H 

#include <stdbool.h>
#include "lambda.h" 
//#include "type.h" 

// TODO: replace by type.h's construct
typedef enum EType EType;
enum EType {
    TINT                = 0,
    TINT_FRM_INT        = 1, 
    TINT_FRM_INT_FRM_INT= 2, 
};
#define NB_TYPES 3 

bool is_func[NB_TYPES];
EType arg_type[NB_TYPES];
EType out_type[NB_TYPES];

/*===========================================================================*/
/*====  0. GRAMMAR and PROGRAM LIST  ========================================*/
/*===========================================================================*/

// Bottom up ENUMERATION: no abstractions for now

typedef struct Grammar Grammar;
struct Grammar {
    int nb_leaves;
    float const* leaf_scores; 
    EType const* leaf_types; 
    float eval_score[NB_TYPES];
};

typedef struct ScoredLamb ScoredLamb;
struct ScoredLamb { float score; LambExpr* e; }; 

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
