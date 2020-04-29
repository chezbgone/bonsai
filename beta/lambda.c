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

bool mentions_vrbl(int vrbl_idx, LambExpr const* e) {
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

LambExpr* unwrap(int vrbl_idx, LambExpr const* e) {
    switch (e->tag) {
        case LEAF: return leaf_expr(e->data.leaf_idx);
        case VRBL: return vrbl_expr(
            vrbl_idx <= e->data.vrbl_idx ?
            e->data.vrbl_idx - 1 :
            e->data.vrbl_idx
        );
        case ABST: return abst_expr(
            unwrap(vrbl_idx+1, e->data.abst.body)
        );
        case EVAL: return eval_expr(
            unwrap(vrbl_idx, e->data.eval.fun),
            unwrap(vrbl_idx, e->data.eval.arg)
        );
    }
}

LambExpr* copy_expr(LambExpr const* e) {
    switch (e->tag) {
        case LEAF: return leaf_expr(e->data.leaf_idx);
        case VRBL: return vrbl_expr(e->data.vrbl_idx);
        case ABST: return abst_expr(copy_expr(e->data.abst.body));
        case EVAL: return eval_expr(copy_expr(e->data.eval.fun),
                                    copy_expr(e->data.eval.arg));
    }
}

LambExpr* replace(int vrbl_idx, LambExpr const* exp, LambExpr const* val) {
    switch (exp->tag) {
        case LEAF: return leaf_expr(exp->data.leaf_idx);
        case VRBL: return (
            exp->data.vrbl_idx == vrbl_idx ?
            copy_expr(val) :
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

LambExpr* leaf_expr(int leaf_idx)
{
    LambExpr* e = malloc(sizeof(LambExpr)); 
    *e = (LambExpr){.tag=LEAF, .data={.leaf_idx=leaf_idx}};
}

LambExpr* vrbl_expr(int vrbl_idx)
{
    LambExpr* e = malloc(sizeof(LambExpr)); 
    *e = (LambExpr){.tag=VRBL, .data={.vrbl_idx=vrbl_idx}};
}

LambExpr* abst_expr(LambExpr* body)
{

    LambExpr* e = malloc(sizeof(LambExpr)); 
    *e = (LambExpr){.tag=ABST, .data={.abst={.body=body}}};
}
LambExpr* eval_expr(LambExpr* fun, LambExpr* arg)
{
    LambExpr* e = malloc(sizeof(LambExpr)); 
    *e = (LambExpr){.tag=EVAL, .data={.eval={.fun=fun, .arg=arg}}};
}

void free_expr(LambExpr* e)
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

void print_expr(LambExpr* e, char leaf_names[][16])
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


