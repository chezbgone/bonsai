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

    LambExpr* l0 = leaf_expr(0); 
    LambExpr* l1 = leaf_expr(1); 
    LambExpr* l2 = leaf_expr(2); 
    LambExpr* l3 = leaf_expr(3); 

    LambExpr* inner_a = eval_expr(eval_expr(l0, l1), l2);
    LambExpr* inner_b = eval_expr(eval_expr(l2, l1), l0);
    LambExpr* medi = eval_expr(eval_expr(l3, inner_a), inner_a); 
    LambExpr* outer = eval_expr(eval_expr(l3, medi), medi);

    LambExpr* conc = eval_expr(eval_expr(l3, v0), v0);  

    CTable ct;

    printf("EXPR:\n");    print_expr(outer, NULL);  printf("\n");

    printf("INIT:\n");    init_table(&ct);          printf("\n"); 
    printf("EXTRACT:\n"); extract_to(outer, &ct);   printf("\n"); 
    printf("PRINT:\n");   print_table(&ct, NULL);   printf("\n"); 

    LambExpr* hey = rewrite_given(outer, conc); 
    print_expr(hey, NULL); printf("\n");

    printf("WIPE:\n");    wipe_table(&ct);          printf("\n");
    printf("DONE!\n");
}
