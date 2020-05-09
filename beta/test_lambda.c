/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>

#include "lambda.h"
#include "extract.h"

char my_leaf_names[][16] = {
    "moop",
    "chit",
    "lump",
    "bopp",
};

void test_eval();

void main()
{
    init_lamb_expr_pool(); test_eval();
    free_lamb_expr_pool();
}

void test_eval()
{
    LambExpr* v0 = vrbl_expr(0);
    LambExpr* v1 = vrbl_expr(1);

    LambExpr* l0 = leaf_expr(0);
    LambExpr* l1 = leaf_expr(1);
    LambExpr* l2 = leaf_expr(2);
    LambExpr* l3 = leaf_expr(3);

    LambExpr* e0 = eval_expr(l0, l1);
    LambExpr* e1 = eval_expr(e0, l2);

    LambExpr* e2 = eval_expr(l3, e1);
    LambExpr* e3 = eval_expr(e2, e1);

    CTable ct;

    printf("\nEXPR:\n");    print_expr(e3, NULL);
    printf("\nINIT:\n");    init_table(&ct);
    printf("\nEXTRACT:\n"); extract_to(e3, &ct);
    printf("\nPRINT:\n");   print_table(&ct, NULL);

    //LambExpr* rewrite_given(LambExpr* e, LambExpr* concept);

    printf("\nWIPE:\n");    wipe_table(&ct);
    printf("\nDONE!\n");
}
