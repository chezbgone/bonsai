/*  author: samtenka
 *  change: 2020-04-26
 *  create: 2020-04-26
 *  descrp: dsl abstract syntax trees
 *  to use: 
 */

#ifndef LAMBDA_H 
#define LAMBDA_H 

#include <stdbool.h>

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


LambdaExpr* leaf_expr(int leaf_idx);
LambdaExpr* vrbl_expr(int vrbl_idx);
LambdaExpr* abst_expr(LambdaExpr* body);
LambdaExpr* eval_expr(LambdaExpr* fun, LambdaExpr* arg);
void free_expr(LambdaExpr* e);

bool mentions_vrbl(int vrbl_idx, LambdaExpr const* e);
LambdaExpr* replace(int vrbl_idx, LambdaExpr* exp, LambdaExpr* val);
void unwrap(int vrbl_idx, LambdaExpr* e);

void print_expr(LambdaExpr* e, char leaf_names[][16]);
void print_expr(LambdaExpr* e, char leaf_names[][16]);

#endif//LAMBDA_H
