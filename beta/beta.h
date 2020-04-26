/*  author: samtenka
 *  change: 2020-04-26
 *  create: 2020-04-26
 *  descrp: lambda ast
 *  to use: 
 */

#ifndef BETA_H 
#define BETA_H 

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

Type t_bool          = {.tag=BASE, .data={.base=BOOL                        }};
Type t_numb          = {.tag=BASE, .data={.base=NUMB                        }};
Type t_bool_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_bool}}};
Type t_numb_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_numb}}};
Type t_bool_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_bool}}};
Type t_numb_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_numb}}};

Type t_bools         = {.tag=MSET, .data={.mset={.elt=&t_bool              }}};
Type t_numbs         = {.tag=MSET, .data={.mset={.elt=&t_numb              }}};

/*=============================================================================
======  1. PROGRAMS  ==========================================================
=============================================================================*/

typedef struct LambdaExpr LambdaExpr;
struct LambdaExpr {
    union {
        int leaf_idx; 
        int vrbl_idx;
        struct {
            LambdaExpr* body;
        } abst; 
        struct {
            LambdaExpr* fun;
            LambdaExpr* arg;
        } eval; 
    } data;
    enum {
        LEAF = 0, 
        VRBL = 1, 
        ABST = 2, 
        EVAL = 3, 
    } tag;
}; 

#endif//BETA_H
