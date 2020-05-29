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

typedef struct Lambs Lambs; 
struct Lambs {
    LambExpr** arr; 
    int len;
    int cap;
};

void qsort_lambs(LambExpr** start, LambExpr** end);
void collect(LambExpr* e, Lambs* ll, int logic_op);
LambExpr* combine(Lambs* ll, int logic_op); 

LambExpr* canonicalize_logic(LambExpr* e, int logic_op)
{
    LambExpr* combined; 
    {
        Lambs ll = {.len = 0, .cap = 4, .arr = malloc(4 * sizeof(LambExpr*))};
        collect(e, &ll, logic_op);
        qsort_lambs(ll.arr, ll.arr + ll.len);
        combined = combine(&ll, logic_op);
        free(ll.arr);
    }
    return combined;
} 

/*===========================================================================*/
/*====  0. HELPERS  =========================================================*/

LambExpr* combine(Lambs* ll, int logic_op)
{
    if ( ll->len == 0 ) { return NULL; /* UH OH!*/ }
    if ( ll->len == 1 ) { return ll->arr[0]; }

    for ( int j = 0; j != ll->len; ++j ) {
        printf(" [%d] ", j);
        print_expr(ll->arr[j], NULL);
        printf("\n");
    }

    LambExpr* operator = leaf_expr(logic_op);
    LambExpr* combined = eval_expr(eval_expr(operator, ll->arr[0]), ll->arr[1]);
    for ( int j = 2; j != ll->len; ++j ) {
        combined = eval_expr(eval_expr(operator, ll->arr[j]), combined); 
    }

    return combined;
} 

void collect(LambExpr* e, Lambs* ll, int logic_op)
{
    if ( e->tag == EVAL &&
         e->FUN->tag == EVAL &&
         e->FUN->FUN->tag == LEAF &&
         e->FUN->FUN->LID == logic_op ) {
        collect(e->ARG     , ll, logic_op);
        collect(e->FUN->ARG, ll, logic_op);
        return;
    }
    if ( ll->len == ll->cap ) {
        int new_cap = (3*ll->cap)/2 + 1; 
        LambExpr** new_arr = malloc( new_cap * sizeof(LambExpr*) ); 
        for ( int j = 0; j != ll->len; ++j ) { new_arr[j] = ll->arr[j]; }
        free(ll->arr);
        ll->arr = new_arr; 
        ll->cap = new_cap;
    }
    ll->arr[ll->len] = e; 
    ll->len += 1;
}

#define SWAP(temp, a, b) {(temp)=(a); (a)=(b); (b)=(temp);} 

void qsort_lambs(LambExpr** start, LambExpr** end)
    /* Sorts from LARGEST to SMALLEST (i.e. in reverse order).
     * We do this because the /combine/ is easier to write this way,
     * and we want small terms in near the top of an expression tree.
     */
{
    if ( start == end ) { return; }

    // nonempty, so partition on a pivot:
    LambExpr** left = start;
    LambExpr** rght = end;
    {
        /* We maintain the invariants that:
         *      [start, left) contains elt.s *at least* the pivot,
         *      [left, left+1) contains the pivot, and
         *      [rght, end) contains elt.s *less than* the pivot.
         * In each step, the difference ( rght - left ) decreases by one.
         */ 

        LambExpr* temp;
        while ( left+1 != rght ) {
            switch ( comp_expr(*(left+1), *left) ) {
                case GT: // fall through
                case EQ: SWAP(temp, *(left+1), *left    ); left += 1; break;
                case LT: SWAP(temp, *(left+1), *(rght-1)); rght -= 1; break;
            }
        }
    }

    // recurse:
    qsort_lambs(start, left);
    qsort_lambs(rght, end);
}
