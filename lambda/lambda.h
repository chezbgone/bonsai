/*  author: samtenka
 *  change: 2020-06-29
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

typedef struct LAMB_EXPR_MUT const LambExpr;
typedef struct LAMB_EXPR_MUT       LambExprMut;
struct LAMB_EXPR_MUT {
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
    LambExpr* val;
    int depth;
};

/*===========================================================================*/
/*====  1. BASIC OPERATIONS  ================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.0. Constructors and Destructors  ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
void init_lamb_expr_pool();
void free_lamb_expr_pool();

LambExpr* leaf_expr(int lid);
LambExpr* vrbl_expr(int vid);
LambExpr* abst_expr(LambExpr* bod);
LambExpr* eval_expr(LambExpr* fun, LambExpr* arg);

void free_expr(LambExpr* e);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.1. Comparators  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                
bool same_node(Node lhs, Node rhs);
bool same_expr(LambExpr* lhs, LambExpr* rhs);

typedef enum Comp Comp;
enum Comp { LT, EQ, GT }; 
Comp comp_expr(LambExpr* lhs, LambExpr* rhs);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.2. Display  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void print_expr(LambExpr* e, char leaf_names[][16]);

/*===========================================================================*/
/*====  2. BETA REWRITES  ===================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.0. Beta Elimination  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  /normalize/ gives /e/'s normal form w.r.t. beta elimination.
 */
LambExpr* normalize(LambExpr* e);
/*
 *  /syllabize/ gives /e/'s normal form w.r.t. beta elimination AND /lib/'s
 *  definitions.  The (n-1)th element of /lib/ is referred to as the negative
 *  literal (-n).  Assumes that /lib/ has no cyclic dependencies.
 */
LambExpr* syllabize(LambExpr* e, LambExpr* const* lib); 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.1. Substitution Helpers  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  /subs/ replaces a variable by a value within an expression.  More
 *  precisely, /subs/ returns a version of /e/ in which each reference to the
 *  variable /vid+depth/ many levels above /exp/ is replaced by a version of
 *  /val/ that points to the same levels as /val/ except that whenever /val/
 *  references an outer variable, that reference is shifted up by /depth/.
 *
 *  So: subs(0 \.(1 0 3), 0, 2 \.(1 0), 0) == 2 \.(1 0) \.(3 \.(4 0) 0 3)
 *
 *  Typical usage will have /vid/ and /depth/ both equal to 0. 
 */
LambExpr* subs(LambExpr* exp, int vid, LambExpr* val, int depth);
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.2. Variable Manipulation Helpers  ~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------  2.2.0. mutate dependencies  -------------------------*/
/*
 *  /shift/ renumbers variables.  More precisely, /shift/ creates an expression
 *  analogous to /e/ except that references to outer variables (i.e., those
 *  /vid/ or more levels up) are increased by /gap_size/.  This renumbering is
 *  appropriate for the following two steps of beta elimination:
 *
 *    * If /e/ is a subtree that we would like to lower within the ambient
 *      tree, then we need to renumber its variables so that they point to the
 *      same absolute locations.  Here, /gap_size/ is positive.  An example is
 *      shift(\.(0 1 2 \.(2 1 3)), 0, 1) == \.(0 2 3 \.(3 1 4)).
 *
 *    * If /e/ is the body of an abstraction each occurence of whose variable
 *      has been replaced by some value, then we seek to shift the references
 *      to the deeper variables up by one, to reflect the disappearance of one
 *      level of abstraction.  Here, /gap_size/ equals -1.  An example is
 *      shift(1 \.(0 2 \.(1 3)), 0, -1) == 0 \.(0 1 \.(1 2)).
 *
 *  Typical usage will have /vid/ equal to 0. 
 */
LambExpr* shift(LambExpr* e, int vid, int gap_size); 

/*--------------------  2.2.1. query dependencies  --------------------------*/

bool mentions_vrbl(LambExpr* e, int vid_lo, int vid_hi);

#endif//LAMBDA_H
