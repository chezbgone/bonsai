/*  author: samtenka
 *  change: 2020-04-26
 *  create: 2020-04-26
 *  descrp: interface for abstract syntax trees
 *  to use: 
 */

#ifndef LAMBDA_H 
#define LAMBDA_H 

#include <stdbool.h>

/*===========================================================================*/
/*====  0. TREE STRUCTURE  ==================================================*/
/*===========================================================================*/

#define LID data.leaf.idx
#define VID data.vrbl.idx 
#define BOD data.abst.bod
#define FUN data.eval.fun 
#define ARG data.eval.arg

typedef struct LambExpr LambExpr;
struct LambExpr {
    union {
        struct { int idx;                      } leaf;
        struct { int idx;                      } vrbl;
        struct { LambExpr* bod;                } abst; 
        struct { LambExpr* fun; LambExpr* arg; } eval; 
    } data;
    enum {
        LEAF = 0, 
        VRBL = 1, 
        ABST = 2, 
        EVAL = 3, 
    } tag;
    int hash;
    int height;
    int weight;
}; 

typedef struct Node Node;
struct Node {
    LambExpr const* val;
    int depth;
};

/*===========================================================================*/
/*====  1. CONSTRUCTORS AND DESTRUCTORS  ====================================*/
/*===========================================================================*/

void init_lamb_expr_pool();
void free_lamb_expr_pool();

LambExpr* leaf_expr(int lid);
LambExpr* vrbl_expr(int vid);
LambExpr* abst_expr(LambExpr* bod);
LambExpr* eval_expr(LambExpr* fun, LambExpr* arg);

void free_expr(LambExpr* e);

/*===========================================================================*/
/*====  2. BASIC OPERATIONS  ================================================*/
/*===========================================================================*/

bool same_node(Node lhs, Node rhs);
bool same_expr(LambExpr const* lhs, LambExpr const* rhs);
LambExpr* subs(LambExpr* exp, int vid, LambExpr* val, int depth);
bool mentions_vrbl(LambExpr* e, int vid_lo, int vid_hi);

void print_expr(LambExpr* e, char leaf_names[][16]);

/*===========================================================================*/
/*====  3. BETA REWRITES  ===================================================*/
/*===========================================================================*/

LambExpr* beta_normal(LambExpr* e);

#endif//LAMBDA_H
