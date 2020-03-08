/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: Implement vector methods for a variety of element types.  See
 *          "vector.c" and "vector_generic.c" for details on method usage
 *          and see "vector_generic.c" for the here-repeated template and for
 *          implementation details.
 *
 *  to use: List as an argument to gcc when compiling:
 *
 *              gcc ... vector.c ... 
 */

#include "fixpoint.h"

/*=============================================================================
======  0. VECTOR IMPLEMENTATIONS FOR EACH ELEMENT TYPE  ======================
=============================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  0.0. Vectors of Integers  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         int 
#define free_elt(ep)    ;
#define VecType         ints  
#define make_vec        make_ints
#define init_vec        init_ints
#define grow_vec        grow_ints
#define push_vec        push_ints
#define free_vec        free_ints
#define wipe_vec        wipe_ints
#include "vector_generic.c"
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
~~~~~~~~~~~~~~  0.1. Vectors and Nested Vectors of Floats  ~~~~~~~~~~~~~~~~~~*/

/*--------------------  0.1.0. vector of char  ------------------------------*/

#define EltType         float
#define free_elt(ep)    ;
#define VecType         floats  
#define make_vec        make_floats
#define init_vec        init_floats
#define grow_vec        grow_floats
#define push_vec        push_floats
#define free_vec        free_floats
#define wipe_vec        wipe_floats
#include "vector_generic.c"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*--------------------  0.1.1. vector of char  ------------------------------*/

#define EltType         floats
#define free_elt(ep)    free_floats(ep)  
#define VecType         floatss 
#define make_vec        make_floatss
#define init_vec        init_floatss
#define grow_vec        grow_floatss
#define push_vec        push_floatss
#define free_vec        free_floatss
#define wipe_vec        wipe_floatss
#include "vector_generic.c"
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
~~~~~~~~~~~~~~  0.2. Vectors and Nested Vectors of Characters  ~~~~~~~~~~~~~~*/

/*--------------------  0.2.0. vector of char  ------------------------------*/

#define EltType         char
#define free_elt(ep)    ;
#define VecType         chars
#define make_vec        make_chars
#define init_vec        init_chars
#define grow_vec        grow_chars
#define push_vec        push_chars
#define free_vec        free_chars
#define wipe_vec        wipe_chars
#include "vector_generic.c"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*--------------------  0.2.1. vector of vector of char  --------------------*/

#define EltType         chars
#define free_elt(ep)    free_chars(ep)
#define VecType         charss
#define make_vec        make_charss
#define init_vec        init_charss
#define grow_vec        grow_charss
#define push_vec        push_charss
#define free_vec        free_charss
#define wipe_vec        wipe_charss
#include "vector_generic.c"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
#undef init_vec
#undef make_vec
#undef VecType
#undef free_elt
#undef EltType

/*--------------------  0.2.2. vector of vector of vector of char  ----------*/

#define EltType         charss
#define free_elt(ep)    free_charss(ep)
#define VecType         charsss
#define make_vec        make_charsss
#define init_vec        init_charsss
#define grow_vec        grow_charsss
#define push_vec        push_charsss
#define free_vec        free_charsss
#define wipe_vec        wipe_charsss
#include "vector_generic.c"
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
~~~~~~~~~~~~~~  1.3. Vectors of Tasks  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         Task
#define free_elt(ep)    free_task(ep)
#define VecType         Tasks  
#define make_vec        make_tasks
#define init_vec        init_tasks
#define grow_vec        grow_tasks
#define push_vec        push_tasks
#define free_vec        free_tasks
#define wipe_vec        wipe_tasks
#include "vector_generic.c"
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
~~~~~~~~~~~~~~  1.4. Vectors of Trees  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define EltType         DecTree 
#define free_elt(ep)    free_tree(ep)
#define VecType         Trees
#define make_vec        make_trees
#define init_vec        init_trees
#define grow_vec        grow_trees
#define push_vec        push_trees
#define free_vec        free_trees
#define wipe_vec        wipe_trees
#include "vector_generic.c"
#undef wipe_vec
#undef free_vec
#undef push_vec
#undef grow_vec
