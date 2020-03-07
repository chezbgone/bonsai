/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-04
 * descrp: 
 * to use: 
 */

#include <stdlib.h> 
#include <stdio.h>  
#include "assert.h"

#ifdef EltType 
#ifdef free_elt

#ifdef VecType
#ifdef make_vec 
#ifdef init_vec 
#ifdef grow_vec 
#ifdef push_vec 
#ifdef free_vec 
#ifdef wipe_vec

typedef struct {
    EltType* data;
    int cap;
    int len;
} VecType;

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
