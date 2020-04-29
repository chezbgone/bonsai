/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "lambda.h"
#include "version.h"

char my_leaf_names[][16] = {
    "moo",
    "chitter",
    "lump",
    "bop",
};

void test_lambda();
void test_eta();
void test_beta();
void test_join();
void test_crash();

void main()
{
    test_crash();
}

void test_crash()
{
    LambdaExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambdaExpr e0 = {.tag=EVAL, .data={.eval={.fun=&v0, .arg=&v0}}};
    LambdaExpr a0 = {.tag=ABST, .data={.abst={.body=&e0         }}};
    LambdaExpr e1 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&a0}}};

    print_expr(&e1, my_leaf_names);
    printf("\n");

    char h;
    scanf("%c", &h);

    VersionSpace* vs = rewrite(&e1);  
    print_vs(vs, my_leaf_names);
    free_vs(vs);

    printf("\n");
}

void test_join()
{
    LambdaExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambdaExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambdaExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};

    LambdaExpr e0 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&v0}}};
    LambdaExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};
    LambdaExpr e1 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&l1}}};

    print_expr(&e1, my_leaf_names);
    printf("\n");

    VersionSpace* vs = rewrite(&e1);  
    print_vs(vs, my_leaf_names);
    free_vs(vs);

    printf("\n");
}

void test_beta()
{
    LambdaExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambdaExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambdaExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};

    LambdaExpr e0 = {.tag=EVAL, .data={.eval={.fun=&v0, .arg=&l0}}};
    LambdaExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};
    LambdaExpr e1 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&l1}}};

    print_expr(&e1, my_leaf_names);
    printf("\n");

    VersionSpace* vs = rewrite(&e1);  
    print_vs(vs, my_leaf_names);
    free_vs(vs);

    printf("\n");
}

void test_eta()
{
    LambdaExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambdaExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambdaExpr e0 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&v0}}};
    LambdaExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};

    print_expr(&a0, my_leaf_names);
    printf("\n");

    VersionSpace* vs = rewrite(&a0);  
    print_vs(vs, my_leaf_names);
    free_vs(vs);

    printf("\n");
}


void test_lambda()
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

    // the following should print 
    // "\.(\.moo (0 1) chitter) 0"
    print_expr(&a1, my_leaf_names);

    printf("\n");
}

