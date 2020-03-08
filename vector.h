/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: Declare vector interfaces for a variety of element types.  For
 *          example, the following are relevant to the vector-of-chars case:
 *
 *              type name: chars
 *              methods  : make_chars(int)         : allocate and return ptr 
 *                         init_chars(chars*, int) : allocate given capacity 
 *                         grow_chars(chars*, int) : expand allocation
 *                         push_chars(chars*, char): push onto end 
 *                         free_chars(chars*)      : deep delete
 *                         wipe_chars(chars*)      : shallow delete 
 *
 *          See "vector_generic.h" for the here-repeated template.
 *
 *  to use: Include: 
 *
 *              #include "vector.h" 
 *
 *          To expand to a user type U, concatenate this paragraph to the end
 *          of this file, and do similarly with ".c" instead of ".h" in the
 *          file "vector.c":
 *
 *              #define EltType         <NAME IN C OF TYPE U>
 *              #define free_elt(ep)    <DESTRUCTOR APPLIED TO EltType ptr ep>
 *              #define VecType         <ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define make_vec        make_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define init_vec        init_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define grow_vec        grow_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define push_vec        push_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define free_vec        free_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #define wipe_vec        wipe_<ONE_WORD_NICKNAME_FOR_TYPE_U>s
 *              #include "vector_generic.h"
 *              #undef wipe_vec
 *              #undef free_vec
 *              #undef push_vec
 *              #undef grow_vec
 *              #undef init_vec
 *              #undef make_vec
 *              #undef VecType
 *              #undef free_elt
 *              #undef EltType
 *
 *          Above, we encounter two subtleties.  First, since types might have
 *          multi-word names (e.g., "unsigned long"), our general recipe is to
 *          specify the type-specialized method names manually.  Hence, we
 *          might use a one-word nickname such as "ulint" for that type, hence
 *          defining a vector type "ulints" with methods such as
 *          "ulints make_ulints(int)".  Second, the deep deletion method 
 *          "void free_Us(int)" needs to call destructors for U, so we need to
 *          specify these as well.  For example, if del_U(u) is a destructor,
 *          then: 
 *
 *              #define free_elt(ep)    del_U(*ep)
 *
 *          is appropriate.
 */

#ifndef VECTOR_H
#define VECTOR_H

/*=============================================================================
======  0. ITERATION HELPERS  =================================================
=============================================================================*/

#define each(eltptr, vec) \
    (eltptr=(vec).data; eltptr!=(vec).data+(vec).len; ++eltptr)

/*=============================================================================
======  1. VECTOR INTERFACES FOR EACH ELEMENT TYPE  ===========================
=============================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~  1.0. Vectors of Integers  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
~~~~~~~~~~~~~~  1.1. Vectors and Nested Vectors of Floats  ~~~~~~~~~~~~~~~~~~*/

/*--------------------  1.1.0. vector of char  ------------------------------*/

#define EltType         float
#define free_elt(ep)    ;
#define VecType         floats  
#define make_vec        make_floats
#define init_vec        init_floats
#define grow_vec        grow_floats
#define push_vec        push_floats
#define free_vec        free_floats
#define wipe_vec        wipe_floats
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

/*--------------------  1.1.1. vector of char  ------------------------------*/

#define EltType         floats
#define free_elt(ep)    free_floats(ep)  
#define VecType         floatss 
#define make_vec        make_floatss
#define init_vec        init_floatss
#define grow_vec        grow_floatss
#define push_vec        push_floatss
#define free_vec        free_floatss
#define wipe_vec        wipe_floatss
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
~~~~~~~~~~~~~~  1.2. Vectors and Nested Vectors of Characters  ~~~~~~~~~~~~~~*/

/*--------------------  1.2.0. vector of char  ------------------------------*/

#define EltType         char
#define free_elt(ep)    ;
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

/*--------------------  1.2.1. vector of vector of char  --------------------*/

#define EltType         chars
#define free_elt(ep)    free_chars(ep)
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

/*--------------------  1.2.2. vector of vector of vector of char  ----------*/

#define EltType         charss
#define free_elt(ep)    free_charss(ep)
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

#include "dectree.h"

#define EltType         DecTree 
#define free_elt(ep)    free_tree(ep)
#define VecType         trees
#define make_vec        make_trees
#define init_vec        init_trees
#define grow_vec        grow_trees
#define push_vec        push_trees
#define free_vec        free_trees
#define wipe_vec        wipe_trees
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

#ifndef VECTOR_DONE
#define VECTOR_DONE
#endif//VECTOR_DONE

#endif//VECTOR_H 
