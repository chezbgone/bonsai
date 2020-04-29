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

bool mentions_vrbl(int vrbl_idx, LambdaExpr* e) {
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

VersionSpace* make_leaf(int leaf_idx)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_LEAF, .data={.leaf_idx=leaf_idx}};
}

VersionSpace* make_vrbl(int vrbl_idx)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_VRBL, .data={.vrbl_idx=vrbl_idx}};
}

VersionSpace* make_abst(VersionSpace* body)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_ABST, .data={.abst={.body=body}}};
}

VersionSpace* make_eval(VersionSpace* fun, VersionSpace* arg)
{
    VersionSpace* vs = malloc(sizeof(VersionSpace)); 
    *vs = (VersionSpace){.tag=VS_EVAL, .data={.eval={.fun=fun, .arg=arg}}};
}

VersionSpace* rewrite(LambdaExpr* e)
{
    /* TODO: check whether in memoized...! */

    VersionSpace* vs = make_empty_union();

    /* identity ... */
    switch (e->tag) {
        case LEAF:
            update_vs(vs, make_leaf(e->data.leaf_idx));
            break;
        case VRBL: 
            update_vs(vs, make_vrbl(e->data.vrbl_idx));
            break;
        case ABST:
            update_vs(vs, make_abst(rewrite(e->data.abst.body)));
            break;
        case EVAL:  
            update_vs(vs, make_eval(
                rewrite(e->data.eval.fun),
                rewrite(e->data.eval.arg)
            ));
            break;
    }

    /* forward eta ... */
    if ( e->tag == ABST && e->data.abst.body->tag == EVAL ) {
        LambdaExpr* fun = e->data.abst.body->data.eval.fun;
        LambdaExpr* arg = e->data.abst.body->data.eval.arg;
        if (
            arg->tag==VRBL &&
            arg->data.vrbl_idx==0 &&
            !mentions_vrbl(0, fun)
        ) {
            update_vs(vs, rewrite(fun));
        }
    }

    /* forward beta ... */
    /* backward beta ... */

    return vs;
}

void print_vs(VersionSpace* vs, char leaf_names[][16])
{
    bool wrap_fun; 
    bool wrap_arg; 
    bool wrap_disj; 
    int i;

    switch (vs->tag) {
        case VS_LEAF:
            crim();
            printf("%s", leaf_names[vs->data.leaf_idx]);
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
            wrap_fun = (vs->data.eval.fun->tag == ABST) ? 1 : 0; 
            wrap_arg = (vs->data.eval.arg->tag == EVAL) ? 1 : 0; 

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

void free_vs(VersionSpace* vs)
{
    /* TODO: below assumes no multi-parent children.  FIX THIS!!  */
    switch (vs->tag) {
        case VS_LEAF: break;
        case VS_VRBL: break; 
        case VS_ABST: free_vs(vs->data.abst.body); break;
        case VS_EVAL: free_vs(vs->data.eval.fun);              
                      free_vs(vs->data.eval.arg); break;
        case VS_DISJ: {
            for (int i=0; i != vs->data.disj.len; ++i) {
                free_vs(vs->data.disj.elts[i]);
            }
        }
        break;
    }

    free(vs);
}
