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

#define NB_TYPES 16
typedef enum EType EType;
enum EType {
    tCEL                =  0,
    tCEL_CEL            =  1,
    tCEL_CEL_DIR        =  2,
    tCEL_CEL_DIR_TWOCEL =  3,

    tDIR                =  4,
    tDIR_CEL            =  5,
    tDIR_CEL_CEL        =  6,
    tDIR_DIR            =  7,

    tDIR_DIR_DIR        =  8,
    tHUE                =  9,
    tHUE_CEL            = 10,
    tTWO                = 11,

    tTWOCEL             = 12,
    tTWOCEL_DIR         = 13,
    tTWOCEL_DIR_TWOCEL  = 14,
    tTWOCEL_HUE         = 15,
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
    bool const* absorbs_self;
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

