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

/*===========================================================================*/
/*====  0. GRAMMAR and PROGRAM LIST  ========================================*/
/*===========================================================================*/

typedef struct Closure Closure;
struct Closure {
};

void* eval(LambExpr* fun, LambExpr* arg)
{
}

void* interpret(LambExpr* e)
{
    /* assume for now that there are no abstractions */
    switch ( e->tag ) {
        case LEAF: return interpret_leaf[e->LID]; 
        case VRBL: return var_stack.arr[var_stack.len-1 - e->VID]; 
        case EVAL: {
            return interpret(apply(interpret(e->FUN),
                                   interpret(e->ARG)));
        }
        case ABST: return ; 
    }
}

endif//ENUMERATOR_H
