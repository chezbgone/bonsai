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
    "funk",
};

LambExpr* make_nested_eval();
LambExpr* make_eval_conc();
void test_eval();

LambExpr* make_nested_abst();
LambExpr* make_abst_conc();
void test_abst();

void main()
{
    init_lamb_expr_pool();
    test_abst();
    free_lamb_expr_pool();
}

void test_abst()
{
    LambExpr* expr = make_nested_abst();
    LambExpr* conc = make_abst_conc();
    LambExpr* name = leaf_expr(4);

    printf("EXPR: ");    print_expr(expr, NULL);  printf("\n");
    printf("CONC: ");    print_expr(conc, NULL);  printf("\n");
    print_expr(rewrite_given(expr, conc, name), NULL);   printf("\n");

    //CTable ct;
    //printf("EXPR:\n");    print_expr(outer, NULL);  printf("\n");
    //printf("INIT:\n");    init_table(&ct);          printf("\n"); 
    //printf("EXTRACT:\n"); extract_to(outer, &ct);   printf("\n"); 
    //printf("PRINT:\n");   print_table(&ct, NULL);   printf("\n"); 
    //printf("WIPE:\n");    wipe_table(&ct);          printf("\n");
    //printf("DONE!\n");

}

//LambExpr* make_nested_abst()
//{
//    LambExpr* v0 = vrbl_expr(0);      
//    LambExpr* v1 = vrbl_expr(1);
//
//    LambExpr* l0 = leaf_expr(0);  
//    LambExpr* l3 = leaf_expr(3);  
//
//    LambExpr* inner = abst_expr(eval_expr(v0, v1));  
//    LambExpr* outer = abst_expr(eval_expr(v0, inner));
//
//    return outer;
//}
LambExpr* make_nested_abst()
{
    LambExpr* v0 = vrbl_expr(0);
    LambExpr* v1 = vrbl_expr(1);

    LambExpr* l0 = leaf_expr(0);  
    LambExpr* l2 = leaf_expr(2);  
    LambExpr* l3 = leaf_expr(3);  

    LambExpr* e022 = eval_expr(eval_expr(l0, l2), l2);
    LambExpr* f0  = abst_expr(eval_expr(eval_expr(l3, v0), abst_expr(eval_expr(v0, v1)))); 
    LambExpr* f02 = eval_expr(eval_expr(l3, e022), abst_expr(eval_expr(v0, e022))); 

    LambExpr* outer = eval_expr(eval_expr(l3, f0), f02);  

    return outer;
}

LambExpr* make_abst_conc()
{
    LambExpr* v0 = vrbl_expr(0);
    LambExpr* v1 = vrbl_expr(1);
    LambExpr* l1 = leaf_expr(1);
    LambExpr* l3 = leaf_expr(3);

    LambExpr* inner = abst_expr(eval_expr(v0, v1)); 
    LambExpr* body  = eval_expr(eval_expr(l3, v0), inner);

    return body;
}

/*---------------------------------------------------------------------------*/

void test_eval()
{
    LambExpr* outer = make_nested_eval();
    LambExpr* conc = make_eval_conc();

    CTable ct;

    printf("EXPR:\n");    print_expr(outer, NULL);  printf("\n");
    printf("INIT:\n");    init_table(&ct);          printf("\n"); 
    printf("EXTRACT:\n"); extract_to(outer, &ct);   printf("\n"); 
    printf("PRINT:\n");   print_table(&ct, NULL);   printf("\n"); 
    //print_expr(rewrite_given(outer, conc), NULL);   printf("\n");
    printf("WIPE:\n");    wipe_table(&ct);          printf("\n");
    printf("DONE!\n");
}

LambExpr* make_nested_eval()
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
    return outer;
}

LambExpr* make_eval_conc()
{
    LambExpr* v0 = vrbl_expr(0); 
    LambExpr* l3 = leaf_expr(3); 

    return eval_expr(eval_expr(l3, v0), v0);  
}
