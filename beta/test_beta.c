/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "beta.h"

void skyb() { printf("\033[36m"); }
void crim() { printf("\033[31m"); }
void lime() { printf("\033[32m"); }
void defc() { skyb(); }

char leaf_names[][16] = {
    "moo",
    "chitter",
    "lump",
    "bop",
};

void print_expr(LambdaExpr* e)
{
    char wrap_fun; 
    char wrap_arg; 

    switch (e->tag) {
        case LEAF:
            crim();
            printf("%s", leaf_names[e->data.leaf_idx]);
            defc();
            break;
        case VRBL: 
            lime();
            printf("%d", e->data.vrbl_idx);
            defc();
            break;
        case ABST:
            lime();
            printf("\\");
            defc();
            print_expr(e->data.abst.body);
            break;
        case EVAL:  
            wrap_fun = (e->data.eval.fun->tag == ABST) ? 1 : 0; 
            wrap_arg = (e->data.eval.arg->tag == EVAL) ? 1 : 0; 

            if (wrap_fun) { printf("("); }
            print_expr(e->data.eval.fun);
            if (wrap_fun) { printf(")"); }
            printf(" ");
            if (wrap_arg) { printf("("); }
            print_expr(e->data.eval.arg);
            if (wrap_arg) { printf(")"); }
            break;
    }
} 

void main()
{
    LambdaExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambdaExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambdaExpr l2 = {.tag=LEAF, .data={.leaf_idx=2}};
    LambdaExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambdaExpr v1 = {.tag=VRBL, .data={.vrbl_idx=1}};
    LambdaExpr v2 = {.tag=VRBL, .data={.vrbl_idx=2}};

    LambdaExpr e0 = {.tag=EVAL, .data={.eval={.fun=&v0, .arg=&v1}}};
    LambdaExpr e1 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&e0}}};
    LambdaExpr e2 = {.tag=EVAL, .data={.eval={.fun=&e1, .arg=&l1}}};

    LambdaExpr a0 = {.tag=ABST, .data={.abst={.body=&e2}}};
    LambdaExpr e3 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&v0}}};
    LambdaExpr a1 = {.tag=ABST, .data={.abst={.body=&e3}}};

    print_expr(&a1);
    printf("\n");
}
