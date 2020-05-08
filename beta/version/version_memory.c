/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdbool.h>
#include <stdlib.h>

#include "version.h"

/*===========================================================================*/
/*====  0. VERSION SPACE CONSTRUCTORS  ======================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.0. Lambda-Parallel Constructors  ~~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* leaf_vs(int leaf_idx)
{
    VerSpace* vs = malloc(sizeof(VerSpace)); 
    *vs = (VerSpace){
        .tag=VS_LEAF, .data={.leaf_idx=leaf_idx},
        .next=NULL, .owner=NULL
    };
    return vs;
}

VerSpace* vrbl_vs(int vrbl_idx)
{
    VerSpace* vs = malloc(sizeof(VerSpace)); 
    *vs = (VerSpace){
        .tag=VS_VRBL, .data={.vrbl_idx=vrbl_idx},
        .next=NULL, .owner=NULL
    };
    return vs;
}

VerSpace* abst_vs(VerSpace* body)
{
    VerSpace* vs = malloc(sizeof(VerSpace)); 
    *vs = (VerSpace){
        .tag=VS_ABST, .data={.abst={.body=body}},
        .next=NULL, .owner=NULL
    };
    if ( body->owner == NULL ) { body->owner = vs; }
    return vs;
}

VerSpace* eval_vs(VerSpace* fun, VerSpace* arg)
{
    VerSpace* vs = malloc(sizeof(VerSpace)); 
    *vs = (VerSpace){
        .tag=VS_EVAL, .data={.eval={.fun=fun, .arg=arg}},
        .next=NULL, .owner=NULL
    };
    if ( fun->owner == NULL ) { fun->owner = vs; }
    if ( arg->owner == NULL ) { arg->owner = vs; }
    return vs;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Union Constructor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void update_vs(VerSpace* vs, VerSpace* summand) 
{
    while ( vs->next != NULL ) { vs = vs->next; } 
    vs->next = summand;
    if ( summand->owner == NULL ) { summand->owner = vs; }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.2. Copy Constructor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* copy_vs(VerSpace const* vs)
{
    VerSpace* head;
    switch (vs->tag) {
        case VS_LEAF: head = leaf_vs(vs->data.leaf_idx); break;
        case VS_VRBL: head = vrbl_vs(vs->data.vrbl_idx); break;
        case VS_ABST: head = abst_vs(copy_vs(vs->data.abst.body)); break;
        case VS_EVAL: head = eval_vs(copy_vs(vs->data.eval.fun),
                                     copy_vs(vs->data.eval.arg)); break;
    }

    if ( vs->next != NULL ) {
        VerSpace* tail = copy_vs(vs->next);
        update_vs(head, tail);
    }

    return head;
}

/*=============================================================================
======  1. VERSION SPACE DESTRUCTORS  =========================================
=============================================================================*/

void free_if_owns(VerSpace* vs, VerSpace** kid_ptr)
{
    VerSpace* kid = *kid_ptr;

    if ( kid == NULL ) { return; }
    if ( kid->owner == vs ) { free_vs(kid_ptr); }
}

void free_vs(VerSpace** vs_ptr)
{
    VerSpace* vs = *vs_ptr;

    switch ( vs->tag ) {
        case VS_LEAF: break;
        case VS_VRBL: break; 
        case VS_ABST: free_if_owns(vs, &(vs->data.abst.body)); break;
        case VS_EVAL: free_if_owns(vs, &(vs->data.eval.fun));
                      free_if_owns(vs, &(vs->data.eval.arg)); break;
    }
    if ( vs->next != NULL ) { free_vs(&(vs->next)); }
    free(vs);

    *vs_ptr = NULL;
}
