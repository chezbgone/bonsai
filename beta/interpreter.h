/*  author: samtenka
 *  change: 2020-05-12
 *  create: 2020-05-12
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#ifndef INTERPRETER_H 
#define INTERPRETER_H 

#include <stdbool.h>

#include "concept_table.h"
#include "lambda.h" 
#include "type.h" 

typedef struct ValGrid ValGrid;
struct ValGrid {
    char* grid;
    char* grid_b;
    int height;
    int width;
    EType tag;
};   

typedef struct PossiblyEvaldExpr PossiblyEvaldExpr;
struct PossiblyEvaldExpr {
    ValGrid* grid; 
    LambExpr* expr; 
};

typedef ValGrid* (*Routine)(PossiblyEvaldExpr const*, CTable const*);

ValGrid* evaluate(LambExpr* e, CTable* ct);

#endif//INTERPRETER_H
