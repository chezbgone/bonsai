/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "lambda.h"

bool mentions_vrbl(int vrbl_idx, LambdaExpr const* e) {
    switch (e->tag) {
        case LEAF: return false;
        case VRBL: return e->data.vrbl_idx == vrbl_idx;
        case ABST: return (
            mentions_vrbl(vrbl_idx + 1, e->data.abst.body)
        );
        case EVAL: return (
            mentions_vrbl(vrbl_idx, e->data.eval.fun) ||
            mentions_vrbl(vrbl_idx, e->data.eval.arg)
        );
    }
}

void unwrap(int vrbl_idx, LambdaExpr* e) {
    switch (e->tag) {
        case LEAF: break;
        case VRBL: if ( vrbl_idx <= e->data.vrbl_idx ) {
            e->data.vrbl_idx -= 1;
        } break;
        case ABST: unwrap(vrbl_idx+1, e->data.abst.body); break;
        case EVAL: unwrap(vrbl_idx, e->data.eval.fun);
                   unwrap(vrbl_idx, e->data.eval.arg); break;
    }
}

LambdaExpr* replace(int vrbl_idx, LambdaExpr* exp, LambdaExpr* val) {
    switch (exp->tag) {
        case LEAF: return leaf_expr(exp->data.leaf_idx);
        case VRBL: return (
            exp->data.vrbl_idx == vrbl_idx ?
            val :
            vrbl_expr(exp->data.vrbl_idx) 
        ); 
        case ABST: return abst_expr(
            replace(vrbl_idx+1, exp->data.abst.body, val)
        );
        case EVAL: return eval_expr(
            replace(vrbl_idx, exp->data.eval.fun, val),
            replace(vrbl_idx, exp->data.eval.arg, val) 
        );
    }
}

LambdaExpr* leaf_expr(int leaf_idx)
{
    LambdaExpr* e = malloc(sizeof(LambdaExpr)); 
    *e = (LambdaExpr){.tag=LEAF, .data={.leaf_idx=leaf_idx}};
}

LambdaExpr* vrbl_expr(int vrbl_idx)
{
    LambdaExpr* e = malloc(sizeof(LambdaExpr)); 
    *e = (LambdaExpr){.tag=VRBL, .data={.vrbl_idx=vrbl_idx}};
}

LambdaExpr* abst_expr(LambdaExpr* body)
{

    LambdaExpr* e = malloc(sizeof(LambdaExpr)); 
    *e = (LambdaExpr){.tag=ABST, .data={.abst={.body=body}}};
}
LambdaExpr* eval_expr(LambdaExpr* fun, LambdaExpr* arg)
{
    LambdaExpr* e = malloc(sizeof(LambdaExpr)); 
    *e = (LambdaExpr){.tag=EVAL, .data={.eval={.fun=fun, .arg=arg}}};
}

void free_expr(LambdaExpr* e)
{
    /* TODO: below assumes no multi-parent children.  FIX THIS!!  */
    switch (e->tag) {
        case LEAF: break;
        case VRBL: break; 
        case ABST: free_expr(e->data.abst.body); break;
        case EVAL: free_expr(e->data.eval.fun);              
                   free_expr(e->data.eval.arg); break;
    }
    free(e);
}

void print_expr(LambdaExpr* e, char leaf_names[][16])
{
    bool wrap_fun; 
    bool wrap_arg; 

    switch (e->tag) {
        case LEAF:
            crim(); 
            if ( leaf_names == NULL) { printf("@%d", e->data.leaf_idx); }
            else { printf("%s", leaf_names[e->data.leaf_idx]); }
            defc();
            break;
        case VRBL: 
            lime();
            printf("%d", e->data.vrbl_idx);
            defc();
            break;
        case ABST:
            lime();
            printf("\\.");
            defc();
            print_expr(e->data.abst.body, leaf_names);
            break;
        case EVAL:  
            wrap_fun = (e->data.eval.fun->tag == ABST) ? 1 : 0; 
            wrap_arg = (e->data.eval.arg->tag == EVAL) ? 1 : 0; 

            if (wrap_fun) { printf("("); }
            print_expr(e->data.eval.fun, leaf_names);
            if (wrap_fun) { printf(")"); }
            printf(" ");
            if (wrap_arg) { printf("("); }
            print_expr(e->data.eval.arg, leaf_names);
            if (wrap_arg) { printf(")"); }
            break;
    }
} 


