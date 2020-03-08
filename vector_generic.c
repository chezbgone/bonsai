/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: Implement vector methods to be specialized in "vector.c".
 *  to use: See "vector.h" for instructions and "vector.c" for examples.
 */

#include <stdlib.h> 
#include <stdio.h>  
#include "verbose.h"

#ifdef EltType 
#ifdef free_elt

#ifdef VecType
#ifdef make_vec 
#ifdef init_vec 
#ifdef grow_vec 
#ifdef push_vec 
#ifdef free_vec 
#ifdef wipe_vec 

/*=============================================================================
======  0. CONSTRUCTORS  ======================================================
=============================================================================*/

VecType make_vec(int cap)
{
    VecType v;
    init_vec(&v, cap);
    return v;
}

void init_vec(VecType* vp, int cap)
{
    vp->data = malloc(sizeof(EltType)*cap);
    vp->cap = cap;
    vp->len = 0;
} 

/*=============================================================================
======  1. MODIFIERS  =========================================================
=============================================================================*/

void grow_vec(VecType* vp, int new_cap)
{
    EltType* new_data = malloc(sizeof(EltType)*new_cap);
    EltType* new_data_ptr = new_data;
    for (EltType* ep=vp->data; ep!=vp->data + vp->len; ++ep) {
        *new_data_ptr = *ep;
        ++new_data_ptr;
    }
    free(vp->data);
    vp->data = new_data;
    vp->cap = new_cap;
    BARK(VERBOSE_VECTOR_MEM, "grew to capacity %d\n", vp->cap);
}

void push_vec(VecType* vp, EltType e)
{
    while (vp->cap <= vp->len) {
        grow_vec(vp, (4*vp->cap)/3 + 8);
    } 
    vp->data[vp->len] = e; 
    ++(vp->len);
}

/*=============================================================================
======  2. DESTRUCTORS  =======================================================
=============================================================================*/

void free_vec(VecType* vp)
{
    for (EltType* ep=vp->data; ep!=vp->data + vp->len; ++ep) {
        free_elt(ep);
    }
    BARK(VERBOSE_VECTOR_MEM, "freed %d elts\n", vp->len);
    free(vp->data);
    vp->cap = 0;
    vp->len = 0;
}

void wipe_vec(VecType* vp)
{
    BARK(VERBOSE_VECTOR_MEM, "wiped %d elts\n", vp->len);
    free(vp->data);
    vp->cap = 0;
    vp->len = 0;
}

#endif
#endif
#endif
#endif
#endif
#endif
#endif

#endif
#endif
