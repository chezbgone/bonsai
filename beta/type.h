/*  author: samtenka
 *  change: 2020-04-28
 *  create: 2020-04-28
 *  descrp: dsl type system
 *  to use: 
 */

#ifndef TYPE_H 
#define TYPE_H 

/*=============================================================================
======  0. TYPES  =============================================================
=============================================================================*/

typedef struct Type Type;
struct Type {
    union {
        enum {
            BOOL = 0,
            NUMB = 1,
        } base;
        struct {
            Type* out;
            Type* arg;
        } from; 
        struct {
            Type* elt;
        } mset; 
    } data;
    enum {
        BASE = 0,
        FROM = 1,
        MSET = 2,
    } tag;
};  

extern Type t_bool;
extern Type t_numb;
extern Type t_bool_pred;
extern Type t_numb_pred;
extern Type t_bool_size;
extern Type t_numb_size;
extern Type t_bools;
extern Type t_numbs;   

#endif//TYPE_H
