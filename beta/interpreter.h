/*  author: samtenka
 *  change: 2020-05-11
 *  create: 2020-05-11
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#ifndef ENUMERATOR_H 
#define ENUMERATOR_H 

#include <stdbool.h>
#include "lambda.h" 

typedef struct ValGrid ValGrid;
typedef ValGrid* (*Routine)(ValGrid const, ValGrid const*);

struct ValGrid {
    char* grid;
    char* grid_b;
    int height;
    int width;
    enum {
        BOOL = 0
        CELL = 1,
        COLOR= 2,
        DRCT = 3,
    } tag;
};   

/* assume for now that there are no abstractions */

ValGrid* evaluate(LambExpr* e);

endif//ENUMERATOR_H
