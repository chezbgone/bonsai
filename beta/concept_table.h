/*  author: samtenka
 *  change: 2020-05-16
 *  create: 2020-05-05
 *  descrp: Interface for hash tables storing (lambda:value) records.  Here, 
 *          the values are either integer counts or pointers to `cargo'.  For
 *          example, count-valued tables can help us find the most frequently
 *          occuring program from among a stream, while cargo-valued tables can
 *          help us memoize bottom-up evaluation of a large list of programs.
 *  to use:  
 */

#ifndef CONCEPT_TABLE_H 
#define CONCEPT_TABLE_H 

#include "lambda.h"

/*===========================================================================*/
/*====  0. TABLE STRUCTURE AND SUBSTRUCTURES  ===============================*/
/*===========================================================================*/

#define COUNT data.count 
#define CARGO data.cargo

typedef enum CTableType CTableType;
enum CTableType {
    COUNT_VALUED = 0,
    CARGO_VALUED = 1,
};

typedef union CTableValue CTableValue;
union CTableValue { 
    int count; 
    void* cargo; 
};

typedef struct CRecord CRecord;
struct CRecord {
    LambExpr const* bod;
    CTableValue data;
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
    CTableType tag; 
};

/*===========================================================================*/
/*====  1. EXPOSED TABLE METHODS  ===========================================*/
/*===========================================================================*/

void init_table(CTable* ct, CTableType tag);
void wipe_table(CTable* ct); /* shallow destroy; doesn't free keys or values */
void update_table(CTable* ct, LambExpr const* bod, CTableValue diff);
CTableValue const* search_table(CTable const* ct, LambExpr const* bod);
LambExpr const* best_concept(CTable const* ct);

void print_table(CTable const* ct, char leaf_names[][16]);

#endif//CONCEPT_TABLE_H
