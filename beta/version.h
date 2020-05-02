/*  author: samtenka
 *  change: 2020-04-28
 *  create: 2020-04-28
 *  descrp: dsl version spaces
 *  to use: 
 */

#ifndef VERSION_H 
#define VERSION_H 

#include "lambda.h"

/*===========================================================================*/
/*====  0. DATA TYPE  =======================================================*/
/*===========================================================================*/

typedef struct VerSpace VerSpace;
struct VerSpace {
    union {
        int leaf_idx; 
        int vrbl_idx;
        struct {
            VerSpace* body;
        } abst; 
        struct {
            VerSpace* fun;
            VerSpace* arg;
        } eval; 
    } data;
    enum {
        VS_LEAF = 0, 
        VS_VRBL = 1, 
        VS_ABST = 2, 
        VS_EVAL = 3, 
    } tag;

    /* for unions: */
    VerSpace* next;

    /* for memory management: specifies who will free this node */
    VerSpace* owner;
}; 

/*===========================================================================*/
/*====  1. BASIC METHODS  ===================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.0. Constructors and Destructors  ~~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* leaf_vs(int leaf_idx);
VerSpace* vrbl_vs(int vrbl_idx);
VerSpace* abst_vs(VerSpace* body);
VerSpace* eval_vs(VerSpace* fun, VerSpace* arg);
void update_vs(VerSpace* vs, VerSpace* summand); 

void free_if_owns(VerSpace* vs, VerSpace** kid);
void free_vs(VerSpace** vs_ptr);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.1. Comparison and Display  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool equals_vs(VerSpace const* lhs, VerSpace const* rhs);
bool contains_term(VerSpace const* vs, VerSpace const* term);

int hash_vs(VerSpace const* vs);

void print_vs(VerSpace const* vs, char leaf_names[][16]);

/*===========================================================================*/
/*====  2. ALGORITHMS  ======================================================*/
/*===========================================================================*/

VerSpace* vs_from_expr(LambExpr const* e);
bool beta_pass(VerSpace* vs);
VerSpace* rewrite(LambExpr const* e);

#endif//VERSION_H
