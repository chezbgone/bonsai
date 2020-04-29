/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "lambda.h"
#include "version.h"

VersionSpace* make_empty_union()
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    vs->tag = VS_DISJ; 
    vs->data.disj.elts = malloc(sizeof(VersionSpace) * 1);
    vs->data.disj.len = 0;
    vs->data.disj.cap = 1;
}
void update_vs(VersionSpace* vs, VersionSpace* summand) 
{
    if (vs->data.disj.len == vs->data.disj.cap) {
        VersionSpace** new_elts = malloc(
            sizeof(VersionSpace*) * 2*vs->data.disj.cap
        ); 
        for (int i=0; i!=vs->data.disj.len; ++i) {
            new_elts[i] = vs->data.disj.elts[i];
        }
        free(vs->data.disj.elts);
        vs->data.disj.elts = new_elts;
    }

    vs->data.disj.elts[ vs->data.disj.len++ ] = summand;
}

VersionSpace* leaf_vs(int leaf_idx)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_LEAF, .data={.leaf_idx=leaf_idx}};
}

VersionSpace* vrbl_vs(int vrbl_idx)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_VRBL, .data={.vrbl_idx=vrbl_idx}};
}

VersionSpace* abst_vs(VersionSpace* body)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_ABST, .data={.abst={.body=body}}};
}

VersionSpace* eval_vs(VersionSpace* fun, VersionSpace* arg)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_EVAL, .data={.eval={.fun=fun, .arg=arg}}};
}

VersionSpace* copy_vs(VersionSpace const* vs)
{
    switch (vs->tag) {
        case VS_LEAF: return leaf_vs(vs->data.leaf_idx);
        case VS_VRBL: return vrbl_vs(vs->data.vrbl_idx);
        case VS_ABST: return abst_vs(copy_vs(vs->data.abst.body));
        case VS_EVAL: return eval_vs(copy_vs(vs->data.eval.fun),
                                       copy_vs(vs->data.eval.arg));
        case VS_DISJ: {
            VersionSpace* union_copy = make_empty_union(); 
            for (int i=0; i != vs->data.disj.len; ++i) {
                update_vs(union_copy, copy_vs(vs->data.disj.elts[i]));
            }
            return union_copy;
        }
    }
}

void update_forward_betas(VersionSpace* accumulator, LambExpr const* e) 
{
    if ( e->tag == EVAL && e->data.eval.fun->tag == ABST ) {
        LambExpr* exp = e->data.eval.fun->data.abst.body;
        LambExpr* val = e->data.eval.arg;

        LambExpr* new_exp_a = replace(0, exp, val);
        LambExpr* new_exp_b = unwrap(0, new_exp_a); 
        free_expr(new_exp_a);

        update_vs(accumulator, rewrite(new_exp_b));
        //update_forward_betas(accumulator, new_exp_b);
        free_expr(new_exp_b);
    }
}

VersionSpace* replace_vs(int vrbl_idx, VersionSpace const* vs, VersionSpace const* val) {
    switch ( vs->tag ) {
        case VS_LEAF: return leaf_vs(vs->data.leaf_idx);
        case VS_VRBL: return (
            vs->data.vrbl_idx == vrbl_idx ?
            copy_vs(val) :
            vrbl_vs(vs->data.vrbl_idx) 
        ); 
        case VS_ABST: return abst_vs(
            replace_vs(vrbl_idx+1, vs->data.abst.body, val)
        );
        case VS_EVAL: return eval_vs(
            replace_vs(vrbl_idx, vs->data.eval.fun, val),
            replace_vs(vrbl_idx, vs->data.eval.arg, val) 
        );
        case VS_DISJ: {
            VersionSpace* accum = make_empty_union();
            for ( int i=0; i != vs->data.disj.len; ++i ) {
                update_vs(accum, replace_vs(
                    vrbl_idx, vs->data.disj.elts[i], val
                ));
            } 
            return accum;
        }
    }
}

VersionSpace* rewrite(LambExpr const* e)
{
    /* TODO: check whether in memoized...! */

    VersionSpace* vs = make_empty_union();

    //update_forward_betas(vs, e);

    /* identity ... */
    switch (e->tag) {
        case LEAF: update_vs(vs, leaf_vs(e->data.leaf_idx)); break;
        case VRBL: update_vs(vs, vrbl_vs(e->data.vrbl_idx)); break;
        case ABST: update_vs(vs, abst_vs(rewrite(e->data.abst.body))); break;
        case EVAL:  
            {
                VersionSpace* funs = rewrite(e->data.eval.fun);
                VersionSpace* args = rewrite(e->data.eval.arg);

                /*  TODO: avoid syntactically redundant trees!  */
                for ( int i=0; i != funs->data.disj.len; ++i ) {
                    for ( int j=0; j != args->data.disj.len; ++j ) {
                        VersionSpace* fun = funs->data.disj.elts[i];
                        VersionSpace* arg = args->data.disj.elts[j];
                        update_vs(vs, eval_vs(copy_vs(fun), copy_vs(arg)));

                        while ( fun->tag == VS_DISJ && fun->data.disj.len == 1) {
                            fun = fun->data.disj.elts[0];
                        }
                        if ( fun->tag != VS_ABST ) { continue; }
                        update_vs(vs, replace_vs(0, fun->data.abst.body, arg));
                    }
                }

                free_vs(funs);
                free_vs(args);
            }
            break;
    }

    // /* forward eta ... */
    // if ( e->tag == ABST && e->data.abst.body->tag == EVAL ) {
    //     LambExpr* fun = e->data.abst.body->data.eval.fun;
    //     LambExpr* arg = e->data.abst.body->data.eval.arg;
    //     if (
    //         arg->tag==VRBL &&
    //         arg->data.vrbl_idx==0 &&
    //         !mentions_vrbl(0, fun)
    //     ) {
    //         LambExpr* new_exp = unwrap(0, fun);
    //         update_vs(vs, rewrite(new_exp));
    //         free_expr(new_exp);
    //     }
    // }

    /* forward beta ... */
    //{
    //    LambExpr* exp = e->data.eval.fun->data.abst.body;
    //    LambExpr* val = e->data.eval.arg;

    //    if ( e->tag == EVAL && e->data.eval.fun->tag == ABST ) {
    //        LambExpr* new_exp_a = replace(0, exp, val);
    //        LambExpr* new_exp_b = unwrap(0, new_exp_a); 

    //        printf(">>> beta on ");
    //        print_expr(exp, NULL);
    //        printf(" and ");
    //        print_expr(val, NULL);
    //        printf(" gives ");
    //        print_expr(new_exp_b, NULL);
    //        printf("\n");

    //        update_vs(vs, rewrite(new_exp_b));

    //        free_expr(new_exp_b);
    //        free_expr(new_exp_a);
    //    }

    //}

    /* backward beta ... */

    return vs;
}

void print_vs(VersionSpace const* vs, char leaf_names[][16])
{
    bool wrap_fun; 
    bool wrap_arg; 
    bool wrap_disj; 
    int i;

    switch (vs->tag) {
        case VS_LEAF:
            crim();
            if ( leaf_names == NULL) { printf("@%d", vs->data.leaf_idx); }
            else { printf("%s", leaf_names[vs->data.leaf_idx]); }
            defc();
            break;
        case VS_VRBL: 
            lime();
            printf("%d", vs->data.vrbl_idx);
            defc();
            break;
        case VS_ABST:
            lime();
            printf("\\.");
            defc();
            print_vs(vs->data.abst.body, leaf_names);
            break;
        case VS_EVAL:  
            wrap_fun = (
                vs->data.eval.fun->tag == VS_ABST || (
                    vs->data.eval.fun->tag == VS_DISJ &&
                    vs->data.eval.fun->data.disj.len == 1 &&
                    vs->data.eval.fun->data.disj.elts[0]->tag == VS_ABST
                )
            );
            wrap_arg = (
                vs->data.eval.arg->tag == VS_EVAL || (
                    vs->data.eval.fun->tag == VS_DISJ &&
                    vs->data.eval.fun->data.disj.len == 1 &&
                    vs->data.eval.fun->data.disj.elts[0]->tag == VS_EVAL
                )
            );

            if (wrap_fun) { printf("("); }
            print_vs(vs->data.eval.fun, leaf_names);
            if (wrap_fun) { printf(")"); }
            printf(" ");
            if (wrap_arg) { printf("("); }
            print_vs(vs->data.eval.arg, leaf_names);
            if (wrap_arg) { printf(")"); }
            break;
        case VS_DISJ:
            wrap_disj = vs->data.disj.len==1 ? 0 : 1;

            if (wrap_disj) { printf("["); }
            for (i=0; i != vs->data.disj.len; ++i) {
                print_vs(vs->data.disj.elts[i], leaf_names); 
                if ( i+1 != vs->data.disj.len ) { printf(" | "); }
            }
            if (wrap_disj) { printf("]"); }
            break;
    }
} 

int depth=0;
void free_vs(VersionSpace* vs)
{
    /* TODO: below assumes no multi-parent children.  FIX THIS!!  */
    //for (int j=0; j!=depth; ++j) { printf(" "); }
    //printf("freeing [");
    //print_vs(vs, NULL);
    //printf("]\n...");
    switch (vs->tag) {
        case VS_LEAF: break;
        case VS_VRBL: break; 
        case VS_ABST: depth += 1;
                      free_vs(vs->data.abst.body);
                      depth -= 1;
                      break;
        case VS_EVAL: depth += 1;
                      free_vs(vs->data.eval.fun);              
                      free_vs(vs->data.eval.arg);
                      depth -= 1;
                      break;
        case VS_DISJ: {
            depth += 1;
            for (int i=0; i != vs->data.disj.len; ++i) {
                free_vs(vs->data.disj.elts[i]);
            }
            depth -= 1;
        }
        break;
    }

    free(vs);
}
