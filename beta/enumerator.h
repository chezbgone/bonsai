/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-05-09
 *  descrp: interface for bottom-up program enumeration
 *  to use: 
 */

#ifndef ENUMERATOR_H 
#define ENUMERATOR_H 

#include <stdbool.h>
#include "lambda.h" 
//#include "type.h" 

// TODO: replace by type.h's construct

//#define NB_TYPES 3 
#define NB_TYPES 22
typedef enum EType EType;
enum EType {
    //TINT                 = 0,
    //TINT_FRM_INT         = 1, 
    //TINT_FRM_INT_FRM_INT = 2, 

    TBOOL                    =  0,
    TBOOLBOOL                =  1,
    TBOOLBOOL_BOOL           =  2,
    TBOOLCELL                =  3,
    TBOOLCELL_CELL           =  4,

    TBOOLCOLOR               =  5,
    TBOOLCOLOR_COLOR         =  6, 
    TBOOLDRCT                =  7,
    TBOOLDRCT_DRCT           =  8,
    TCELL                    =  9,

    TCELL_CELL               = 10,
    TCELL_CELL_DRCT          = 11,
    TCELL_CELL_DRCT_BOOLCELL = 12,
    TCOLOR                   = 13,
    TCOLOR_CELL              = 14,

    TDRCT                    = 15,                     
    TDRCT_CELL               = 16,
    TDRCT_CELL_CELL          = 17,   
    TDRCT_DRCT               = 18,
    TDRCT_DRCT_DRCT          = 19, 

    TBOOLCELL_DRCT_BOOLCOLOR = 20,
    TBOOLCELL_DRCT           = 21,
};

extern bool is_func[];
extern EType arg_type[];
extern EType out_type[];

/*===========================================================================*/
/*====  0. GRAMMAR and PROGRAM LIST  ========================================*/
/*===========================================================================*/

// TODO: note that right now we do bottom up enumeration with no abstractions
//       for now

typedef struct Grammar Grammar;
struct Grammar {
    int nb_leaves;
    float const* leaf_scores; 
    EType const* leaf_types; 
    float eval_score[NB_TYPES];
    float abst_score;

    bool const* is_const;
    bool const* needs_nonconst;
    bool const* commutes;
    bool const* needs_unequal;
    bool const* is_monoid_action;
};

typedef struct ScoredLamb ScoredLamb;
struct ScoredLamb { float score; LambExpr* e; bool is_const; bool needs_nonconst; }; 

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

