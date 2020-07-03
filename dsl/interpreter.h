/*  author: samtenka
 *  change: 2020-06-30
 *  create: 2020-05-12
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#ifndef INTERPRETER_H 
#define INTERPRETER_H 

#include <stdbool.h>
#include "../lambda/concept_table.h"
#include "../lambda/lambda.h" 
#include "../lambda/type.h" 
#include "../dsl/grammar.h" 

typedef struct ValGrid ValGrid;
struct ValGrid {
    char* grid;
    char* grid_b;
    int height;
    int width;
    EType tag;
};   

/* define the type: /Routine/                                                */
typedef ValGrid const* (*Routine)(ValGrid const*, LambExpr* const*, CTable const*);

void print_grid(ValGrid const* vg); 

ValGrid const* evaluate(ValGrid const* input, LambExpr* e, CTable* ct, Grammar const* G);

ValGrid* make_grid(int H, int W, EType tag);

#endif//INTERPRETER_H
