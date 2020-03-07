/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-04
 * descrp: 
 * to use: 
 */

#ifndef VECTOR_H
#define VECTOR_H

#define each(eltptr, vec) (eltptr=(vec).data; eltptr!=(vec).data+(vec).len; ++eltptr)

/*=============================================================================
======  0. VECTOR TYPES  ======================================================
=============================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  0.0. vector of int  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         int 
#define free_elt(ep)    ;
#define VecType         ints  
#define make_vec        make_ints
#define init_vec        init_ints
#define grow_vec        grow_ints
#define push_vec        push_ints
#define free_vec        free_ints
#define wipe_vec        wipe_ints
#include "vector_generic.h"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  0.1. vector of char  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         char
#define free_elt(e)     ;
#define VecType         chars
#define make_vec        make_chars
#define init_vec        init_chars
#define grow_vec        grow_chars
#define push_vec        push_chars
#define free_vec        free_chars
#define wipe_vec        wipe_chars
#include "vector_generic.h"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  0.2. vector of vector of char  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         chars
#define free_elt(e)     free_chars
#define VecType         charss
#define make_vec        make_charss
#define init_vec        init_charss
#define grow_vec        grow_charss
#define push_vec        push_charss
#define free_vec        free_charss
#define wipe_vec        wipe_charss
#include "vector_generic.h"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  0.3. vector of vector of vector of char  ~~~~~~~~~~~~~~~~~~~~*/

#define EltType         charss
#define free_elt(e)     free_charss
#define VecType         charsss
#define make_vec        make_charsss
#define init_vec        init_charsss
#define grow_vec        grow_charsss
#define push_vec        push_charsss
#define free_vec        free_charsss
#define wipe_vec        wipe_charsss
#include "vector_generic.h"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

#endif//VECTOR_H 
