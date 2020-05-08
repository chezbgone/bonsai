/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "lambda.h"

char my_leaf_names[][16] = {
    "moop",
    "chit",
    "lump",
    "bopp",
};

void test_two_arg();

void main()
{
    test_two_arg();
}

void test_two_arg()
{
    LambExpr v0 = {.tag=VRBL, .data={.vrbl={.idx = 0 }}};
    LambExpr v1 = {.tag=VRBL, .data={.vrbl={.idx = 1 }}};

    LambExpr l0 = {.tag=LEAF, .data={.leaf={.idx = 0 }}};
    LambExpr l1 = {.tag=LEAF, .data={.leaf={.idx = 1 }}};
    LambExpr l2 = {.tag=LEAF, .data={.leaf={.idx = 2 }}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun = &v0 , .arg = &l0 }}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun = &e0 , .arg = &v1 }}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.bod = &e1              }}};
    LambExpr a1 = {.tag=ABST, .data={.abst={.bod = &a0              }}};

    LambExpr e2 = {.tag=EVAL, .data={.eval={.fun = &a1 , .arg = &l1 }}};
    LambExpr e3 = {.tag=EVAL, .data={.eval={.fun = &e2 , .arg = &l2 }}};

    printf("\nexpr:\n");    print_expr(&e3, my_leaf_names);
    printf("\ndone!\n");
}
