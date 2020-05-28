/*  author: samtenka
 *  change: 2020-05-28
 *  create: 2020-05-28
 *  descrp: 
 *  to use:  
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "extract.h"
#include "lambda.h"

/*===========================================================================*/
/*====  0. REWRITE  =========================================================*/
/*===========================================================================*/

typedef struct Lambs Lambs; 
struct Lambs {
    LambExpr** arr; 
    int len;
    int cap;
};

void qsort_lambs(LambExprMut* start, LambExprMut* end);
void collect(LambExpr* e, Lambs* ll, int logic_op);
LambExpr* combine(Lambs* ll, int logic_op); 

LambExpr* canonicalize_logic(LambExpr* e, int logic_op)
{
    LambExpr* combined; 
    {
        Lambs ll;
        ll = {.len = 0, .cap = 4, .arr = malloc(4 * sizeof(LambExpr*))};
        collect(e, &ll, logic_op);
        qsort_lambs(&ll, &ll + ll.len);
        combined = combine(&ll, logic_op);
        free(ll.arr);
    }
    return combined;
} 

LambExpr* combine(Lambs* ll, int logic_op)
{
    if ( ll->len == 0 ) { return NULL; }
    if ( ll->len == 1 ) { return ll->arr[0]; }

    LambExpr* operator = leaf_expr(logic_op);
    LambExpr* combined = eval_expr(eval_expr(operator, ll->arr[0]), ll->arr[1]);
    for ( int j = 2; j != ll->len; ++j ) {
        combined = eval_expr(eval_expr(operator, ll->arr[j]), combined); 
    }

    return combined;
} 

void collect(LambExpr* e, Lambs* ll, int logic_op)
{
    if ( e->tag == EVAL && e->FUN.tag == EVAL && e->FUN.LID == logic_op ) {
        collect(e->ARG, ll);
        collect(e->FUN.ARG, ll);
        return;
    }
    if ( ll->len == ll->cap ) {
        int new_cap = (3*ll->cap)/2 + 1; 
        LambExpr** new_arr = malloc( new_cap * sizeof(LambExpr*) ); 
        for ( int j = 0; j != ll->len; ++ j ) { new_arr[j] = ll->arr[j]; }
        free(ll->arr);
        ll->arr = new_arr; 
        ll->cap = new_cap;
    }
    ll->arr[ll->len] = *e; 
    ll->len += 1;
}

void qsort_lambs(LambExpr** start, LambExpr** end)
{
    if ( start == end ) { return; }

    // nonempty, so choose pivot:
    LambExpr* pivot = *start;  

    // partition:
    {
        LambExpr* temp;
        void swap(LambExpr** a, LambExpr** b)
        {
            temp = *a; 
            *a = *b;
            *b = temp;
        }

        LambExpr** left = start;
        LambExpr** rght = end;
        /* We maintain the invariant that [start, left) contains elements  
         * less than the pivot and that [rght, end) contains elements greater
         * than the pivot, and each step, the difference rght-left decreases
         * by one.
         */ 
        while ( left != rght ) {
            if ( comp_expr(*(left+1), pivot) == LT ) {
                *left = *(left+1);  left += 1;
            } else {
                swap(left, rght-1); rght -= 1;
            }
        }
        *left = pivot;
    }

    // recurse:
    qsort_lambs(start, left);
    qsort_lambs(rght, end);
}


