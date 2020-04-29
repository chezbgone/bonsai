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
#include "type.h"
#include "lambda.h"

Type t_bool          = {.tag=BASE, .data={.base=BOOL                        }};
Type t_numb          = {.tag=BASE, .data={.base=NUMB                        }};
Type t_bool_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_bool}}};
Type t_numb_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_numb}}};
Type t_bool_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_bool}}};
Type t_numb_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_numb}}};

Type t_bools         = {.tag=MSET, .data={.mset={.elt=&t_bool              }}};
Type t_numbs         = {.tag=MSET, .data={.mset={.elt=&t_numb              }}};

void print_expr(LambdaExpr* e, char leaf_names[][16])
{
    bool wrap_fun; 
    bool wrap_arg; 

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
            printf("\\.");
            defc();
            print_expr(e->data.abst.body, leaf_names);
            break;
        case EVAL:  
            wrap_fun = (e->data.eval.fun->tag == ABST) ? 1 : 0; 
            wrap_arg = (e->data.eval.arg->tag == EVAL) ? 1 : 0; 

            if (wrap_fun) { printf("("); }
            print_expr(e->data.eval.fun, leaf_names);
            if (wrap_fun) { printf(")"); }
            printf(" ");
            if (wrap_arg) { printf("("); }
            print_expr(e->data.eval.arg, leaf_names);
            if (wrap_arg) { printf(")"); }
            break;
    }
} 


