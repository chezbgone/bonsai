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
void test_big();
void test_two_arg();

void main()
{
    test_big();
    //test_two_arg();
    //test_beta();
}

void test_beta()
{
    LambExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun=&v0, .arg=&l0}}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&l1}}};

    printf("expr: ");
    print_expr(&e1, NULL);
    printf("\n");

    VerSpace* vs = rewrite(&e1);  

    printf("vs: ");
    print_vs(vs, NULL);
    printf("\n");


    free_vs(&vs);
}

void test_two_arg()
{
    LambExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambExpr v1 = {.tag=VRBL, .data={.vrbl_idx=1}};

    LambExpr l0  = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr l1  = {.tag=LEAF, .data={.leaf_idx=1}};
    LambExpr l2  = {.tag=LEAF, .data={.leaf_idx=2}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun = &v0 , .arg = &l0 }}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun = &e0 , .arg = &v1 }}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.body= &e1              }}};
    LambExpr a1 = {.tag=ABST, .data={.abst={.body= &a0              }}};

    LambExpr e2 = {.tag=EVAL, .data={.eval={.fun = &a1 , .arg = &l1 }}};
    LambExpr e3 = {.tag=EVAL, .data={.eval={.fun = &e2 , .arg = &l2 }}};

    printf("\n expr:\n");        print_expr(&e3, my_leaf_names);
    printf("\n rewriting...\n"); VerSpace* vs = rewrite(&e3);  
    printf("\n vs:\n");          print_vs(vs, my_leaf_names);
    printf("\n freeing...:\n");  free_vs(&vs);
    printf("\n done!\n");
}

void test_big()
{
    LambExpr v0a= {.tag=VRBL, .data={.vrbl_idx=0}};
    LambExpr v0b= {.tag=VRBL, .data={.vrbl_idx=0}};
    LambExpr v1a= {.tag=VRBL, .data={.vrbl_idx=1}};
    LambExpr v1b= {.tag=VRBL, .data={.vrbl_idx=1}};
    LambExpr v2 = {.tag=VRBL, .data={.vrbl_idx=2}};

    LambExpr l0  = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr l1  = {.tag=LEAF, .data={.leaf_idx=1}};
    LambExpr l2a = {.tag=LEAF, .data={.leaf_idx=2}};
    LambExpr l2b = {.tag=LEAF, .data={.leaf_idx=2}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun =&v0a, .arg =&v2 }}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun =&v1a, .arg =&e0 }}};
    LambExpr e2 = {.tag=EVAL, .data={.eval={.fun =&e1 , .arg =&l2a}}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.body=&e2             }}};
    LambExpr a1 = {.tag=ABST, .data={.abst={.body=&a0             }}};

    LambExpr e3 = {.tag=EVAL, .data={.eval={.fun =&l0 , .arg = &l1 }}};

    LambExpr e4 = {.tag=EVAL, .data={.eval={.fun =&v0b, .arg = &l2b}}};
    LambExpr e5 = {.tag=EVAL, .data={.eval={.fun =&e4 , .arg = &v1b}}};
    LambExpr a2 = {.tag=ABST, .data={.abst={.body=&e5              }}};

    LambExpr e6 = {.tag=EVAL, .data={.eval={.fun =&a1 , .arg = &e3 }}};
    LambExpr e7 = {.tag=EVAL, .data={.eval={.fun =&e6 , .arg = &a2 }}};

    LambExpr a3 = {.tag=ABST, .data={.abst={.body=&e7              }}};

    printf("\nexpr:\n");
    print_expr(&a3, my_leaf_names);

    printf("\nrewriting...\n");
    VerSpace* vs = rewrite(&a3);  

    printf("\nvs:\n");
    print_vs(vs, my_leaf_names);

    printf("\nfreeing...:\n");
    free_vs(&vs);

    printf("\n");
}

void test_join()
{
    LambExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&v0}}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&l1}}};

    print_expr(&e1, my_leaf_names);
    printf("\n");

    VerSpace* vs = rewrite(&e1);  
    print_vs(vs, my_leaf_names);
    free_vs(&vs);

    printf("\n");
}

void test_eta()
{
    LambExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&v0}}};
    LambExpr a0 = {.tag=ABST, .data={.abst={.body=&e0}}};

    print_expr(&a0, my_leaf_names);
    printf("\n");

    VerSpace* vs = rewrite(&a0);  
    print_vs(vs, my_leaf_names);
    free_vs(&vs);

    printf("\n");
}


void test_lambda()
{
    LambExpr l0 = {.tag=LEAF, .data={.leaf_idx=0}};
    LambExpr l1 = {.tag=LEAF, .data={.leaf_idx=1}};
    LambExpr l2 = {.tag=LEAF, .data={.leaf_idx=2}};

    LambExpr v0 = {.tag=VRBL, .data={.vrbl_idx=0}};
    LambExpr v1 = {.tag=VRBL, .data={.vrbl_idx=1}};
    LambExpr v2 = {.tag=VRBL, .data={.vrbl_idx=2}};

    LambExpr e0 = {.tag=EVAL, .data={.eval={.fun=&v0, .arg=&v1}}};
    LambExpr e1 = {.tag=EVAL, .data={.eval={.fun=&l0, .arg=&e0}}};
    LambExpr e2 = {.tag=EVAL, .data={.eval={.fun=&e1, .arg=&l1}}};

    LambExpr a0 = {.tag=ABST, .data={.abst={.body=&e2}}};
    LambExpr e3 = {.tag=EVAL, .data={.eval={.fun=&a0, .arg=&v0}}};
    LambExpr a1 = {.tag=ABST, .data={.abst={.body=&e3}}};

    // the following should print 
    // "\.(\.moo (0 1) chitter) 0"
    print_expr(&a1, my_leaf_names);

    printf("\n");
}

