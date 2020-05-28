/*  author: samtenka
 *  change: 2020-05-27
 *  create: 2020-04-04
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
    printf("DONE!\n");
}

void test_abst()
{
    LambExpr* expr;
    LambExpr* conc; 
    LambExpr* rewr; 

    expr = make_nested_abst();

    printf("EXPR: ");    print_expr(expr, NULL);            printf("\n");
    {
        CTable ct;
        init_table(&ct, COUNT_VALUED);
        extract_to(expr, &ct);
        conc = best_concept(&ct); 
        wipe_table(&ct);
    }
    printf("CONC: ");     print_expr(conc, NULL);           printf("\n");

    rewr = rewrite_given(expr, conc, leaf_expr(4));
    printf("REWR: ");     print_expr(rewr, NULL);           printf("\n");
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
    LambExpr* v2 = vrbl_expr(2);

    LambExpr* l0 = leaf_expr(0);  
    LambExpr* l3 = leaf_expr(3);  

    LambExpr* xa0 = eval_expr(l0, v0);
    LambExpr* xa1 = eval_expr(l0, v1);
    LambExpr* xb = eval_expr(eval_expr(l0, v2), v2);

    LambExpr* fa = eval_expr(eval_expr(l3, xa0), abst_expr(eval_expr(v0, xa1))); 
    LambExpr* fb = eval_expr(eval_expr(l3, xb), abst_expr(eval_expr(v0, xb))); 

    LambExpr* outer = abst_expr(eval_expr(eval_expr(v0, fa), abst_expr(fb)));

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

    printf("EXPR:\n");    print_expr(outer, NULL);          printf("\n");
    printf("INIT:\n");    init_table(&ct, COUNT_VALUED);    printf("\n"); 
    printf("EXTRACT:\n"); extract_to(outer, &ct);           printf("\n"); 
    printf("PRINT:\n");   print_table(&ct, NULL);           printf("\n"); 
    //print_expr(rewrite_given(outer, conc), NULL);           printf("\n");
    printf("WIPE:\n");    wipe_table(&ct);                  printf("\n");
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
