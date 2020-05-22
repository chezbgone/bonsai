/*  author: samtenka
 *  change: 2020-05-17
 *  create: 2020-05-17
 *  descrp: toy ARC-style dataset capturing the notion of ``neighbor''
 *  to use: 
 */

#ifndef TOY_DATA_H 
#define TOY_DATA_H 

#include <stdbool.h>

#include "../dsl/interpreter.h" 

typedef struct ArcPair ArcPair; 
struct ArcPair {
    ValGrid const* x; 
    ValGrid const* y; 
}; 

#define A_NB_PAIRS 2 
#define B_NB_PAIRS 2 
#define C_NB_PAIRS 2 
#define D_NB_PAIRS 2 
#define E_NB_PAIRS 2 
#define F_NB_PAIRS 2 

extern ArcPair a_pairs[A_NB_PAIRS];
extern ArcPair b_pairs[B_NB_PAIRS];
extern ArcPair c_pairs[C_NB_PAIRS];
extern ArcPair d_pairs[D_NB_PAIRS];
extern ArcPair e_pairs[E_NB_PAIRS];
extern ArcPair f_pairs[F_NB_PAIRS];

#endif//TOY_DATA_H
