/*  author: samtenka
 *  change: 2020-05-26
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
#define G_NB_PAIRS 2 
#define H_NB_PAIRS 2 

extern ArcPair a_pairs[A_NB_PAIRS];
extern ArcPair b_pairs[B_NB_PAIRS];
extern ArcPair c_pairs[C_NB_PAIRS];
extern ArcPair d_pairs[D_NB_PAIRS];
extern ArcPair e_pairs[E_NB_PAIRS];
extern ArcPair f_pairs[F_NB_PAIRS];

typedef struct TaskData TaskData;
struct TaskData {
    ArcPair const* pairs; 
    int nb_pairs; 
}; 

#define NB_ALL_TASKS 8 
extern TaskData all_pairs[NB_ALL_TASKS]; 

#define NB_EASY_TASKS 4 
extern TaskData easy_pairs[NB_EASY_TASKS]; 
extern char easy_pair_descrps[NB_EASY_TASKS][64];

#endif//TOY_DATA_H
