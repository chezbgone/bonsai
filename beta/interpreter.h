/*  author: samtenka
 *  change: 2020-05-12
 *  create: 2020-05-12
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#ifndef INTERPRETER_H 
#define INTERPRETER_H 

#include <stdbool.h>
#include "lambda.h" 
#include "type.h" 

typedef struct ValGrid ValGrid;
typedef ValGrid* (*Routine)(ValGrid const*, ValGrid const*);

struct ValGrid {
    char* grid;
    char* grid_b;
    int height;
    int width;
    EType tag;
};   

ValGrid* evaluate(LambExpr* e);

#endif//INTERPRETER_H
