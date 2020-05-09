/*  author: samtenka
 *  change: 2020-05-06
 *  create: 2020-05-06
 *  descrp: interface for hash table storing (lambda:score) records
 *  to use:  
 */

#ifndef CONCEPT_TABLE_H 
#define CONCEPT_TABLE_H 

#include "lambda.h"

/*===========================================================================*/
/*====  0. TABLE STRUCTURE AND SUBSTRUCTURES  ===============================*/
/*===========================================================================*/

typedef struct CRecord CRecord;
struct CRecord {
    LambExpr* bod;
    int score; 
};

typedef struct CList CList;
struct CList {
    CRecord* arr; 
    int len;
    int cap;
};

typedef struct CTable CTable;
struct CTable {
    CList* arr; 
    int nb_bins;
    int nb_elts;
};

/*===========================================================================*/
/*====  1. EXPOSED TABLE METHODS  ===========================================*/
/*===========================================================================*/

void init_table(CTable* ct);
void free_table(CTable* ct);
void update_table(CTable* ct, LambExpr* bod, int d_score);
LambExpr* best_concept(CTable* ct);

void print_table(CTable const* ct, char leaf_names[][16]);

#endif//CONCEPT_TABLE_H
