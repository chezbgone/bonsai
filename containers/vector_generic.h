/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: Declare vector interfaces to be specialized in "vector.h"
 *  to use: See "vector.h" for instructions and examples.
 */

#include <stdlib.h> 
#include <stdio.h>  

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
======  0. VECTOR DATA  =======================================================
=============================================================================*/

typedef struct {
    EltType* data;
    int cap;
    int len;
} VecType;

/*=============================================================================
======  1. VECTOR METHODS  ====================================================
=============================================================================*/

VecType make_vec(int cap);
void init_vec(VecType* vp, int cap);
void grow_vec(VecType* vp, int new_cap);
void push_vec(VecType* vp, EltType e);
void free_vec(VecType* vp); /* deep */
void wipe_vec(VecType* vp); /* shallow  */

#endif
#endif
#endif
#endif
#endif
#endif
#endif

#endif
#endif
