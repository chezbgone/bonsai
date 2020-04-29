/*  author: samtenka
 *  change: 2020-04-26
 *  create: 2020-04-26
 *  descrp: dsl abstract syntax trees
 *  to use: 
 */

#ifndef LAMBDA_H 
#define LAMBDA_H 

/*=============================================================================
======  1. PROGRAMS  ==========================================================
=============================================================================*/

typedef struct LambdaExpr LambdaExpr;
struct LambdaExpr {
    union {
        int leaf_idx; 
        int vrbl_idx;
        struct {
            LambdaExpr* body;
        } abst; 
        struct {
            LambdaExpr* fun;
            LambdaExpr* arg;
        } eval; 
    } data;
    enum {
        LEAF = 0, 
        VRBL = 1, 
        ABST = 2, 
        EVAL = 3, 
    } tag;
}; 

void print_expr(LambdaExpr* e, char leaf_names[][16]);

#endif//LAMBDA_H
