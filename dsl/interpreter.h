/*  author: samtenka
 *  change: 2020-05-17
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

typedef struct ValGrid ValGrid;
struct ValGrid {
    char* grid;
    char* grid_b;
    int height;
    int width;
    EType tag;
};   
typedef ValGrid const* (*Routine)(ValGrid const*, LambExpr* const*, CTable const*);

void print_grid(ValGrid const* vg); 

bool has_subroutines(LambExpr const* e, Grammar const* G);
void expand_subroutines(LambExpr* e, CTable* ct)
{
    if ( ! has_subroutines(e) ) { return; }
    switch ( e->TAG ) {
        case LEAF: return;
        case VRBL: return; 
        case EVAL:
            expand_subroutines(e->arg)
        case ABST: /*TODO*/ return;
    }
}

ValGrid const* evaluate(ValGrid const* input, LambExpr* e, CTable* ct, int const* nb_args);

ValGrid* make_grid(int H, int W, EType tag);

#endif//INTERPRETER_H
