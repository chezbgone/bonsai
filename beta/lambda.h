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

typedef struct LambExpr LambExpr;
struct LambExpr {
    union {
        int leaf_idx; 
        int vrbl_idx;
        struct {
            LambExpr* body;
        } abst; 
        struct {
            LambExpr* fun;
            LambExpr* arg;
        } eval; 
    } data;
    enum {
        LEAF = 0, 
        VRBL = 1, 
        ABST = 2, 
        EVAL = 3, 
    } tag;
}; 


LambExpr* leaf_expr(int leaf_idx);
LambExpr* vrbl_expr(int vrbl_idx);
LambExpr* abst_expr(LambExpr* body);
LambExpr* eval_expr(LambExpr* fun, LambExpr* arg);
void free_expr(LambExpr* e);

bool mentions_vrbl(int vrbl_idx, LambExpr const* e);
LambExpr* replace(int vrbl_idx, LambExpr const* exp, LambExpr const* val);
LambExpr* unwrap(int vrbl_idx, LambExpr const* e);
LambExpr* copy_expr(LambExpr const* e);

void print_expr(LambExpr* e, char leaf_names[][16]);
void print_expr(LambExpr* e, char leaf_names[][16]);

#endif//LAMBDA_H
