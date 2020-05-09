/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "lambda.h"

/*===========================================================================*/
/*====  0. CONSTRUCTORS AND DESTRUCTORS  ====================================*/
/*===========================================================================*/

int memoize_hash(LambExpr const* e);
int memoize_height(LambExpr const* e);
LambExpr* make(LambExpr val);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  0.0. Provide Tree Structure  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

LambExpr* leaf_expr(int lid)
{
    LambExpr e = {.tag=LEAF, .data={.leaf={.idx=lid}}};
    return make(e);
}

LambExpr* vrbl_expr(int vid)
{
    LambExpr e = {.tag=VRBL, .data={.vrbl={.idx=vid}}};
    return make(e);
}

LambExpr* abst_expr(LambExpr* bod)
{
    LambExpr e = {.tag=ABST, .data={.abst={.bod=bod}}};
    return make(e);
}

LambExpr* eval_expr(LambExpr* fun, LambExpr* arg)
{
    LambExpr e = {.tag=EVAL, .data={.eval={.fun=fun, .arg=arg}}};
    return make(e);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  0.1. Compute Hash and Height from Tree Structure  ~~~~~~~~~~~~~*/

#define PRIME_A   10007
#define PRIME_B  100003
#define PRIME_C 1000003

#define MAX(X,Y) ((X)<(Y) ? (Y) : (X))

int hash_step(LambExpr const* e)
{
    switch ( e->tag ) {
        case LEAF: return (e->LID + 1)<<8;
        case VRBL: return 1;
        case ABST: return 1 + PRIME_A * e->BOD->hash;
        case EVAL: return 1 + PRIME_B * e->FUN->hash 
                            + PRIME_C * e->ARG->hash;
    }
}

int height_step(LambExpr const* e) 
{
    switch ( e->tag ) {
        case LEAF: return 0;
        case VRBL: return 0;
        case ABST: return 1 + e->BOD->height;
        case EVAL: return 1 + MAX(e->FUN->height, e->ARG->height);
    }
}

int weight_step(LambExpr const* e) 
{
    switch ( e->tag ) {
        case LEAF: return 1;
        case VRBL: return 1;
        case ABST: return 1 + e->BOD->weight;
        case EVAL: return 1 + e->FUN->weight + e->ARG->weight;
    }
}

LambExpr* make(LambExpr val)
{
    LambExpr* e = malloc(sizeof(LambExpr)); 
    *e = val;
    e->hash = hash_step(e);
    e->height = height_step(e);
    e->weight = weight_step(e);
    return e;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  0.2. Destructor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void free_expr(LambExpr* e)
{
    /* TODO: below assumes no multi-parent children.  FIX THIS!!  */
    switch (e->tag) {
        case LEAF: break;
        case VRBL: break; 
        case ABST: free_expr(e->BOD); break;
        case EVAL: free_expr(e->FUN);              
                   free_expr(e->ARG); break;
    }
    free(e);
}

/*===========================================================================*/
/*====  1. BASIC OPERATIONS  ================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.0. Syntactic Equality  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool same_expr(LambExpr const* lhs, LambExpr const* rhs)
{
    return same_node((Node){lhs, 0}, (Node){rhs, 0});
}

bool same_node(Node left, Node rght)
{
    LambExpr const* lx = left.val;   int ld = left.depth;  
    LambExpr const* rx = rght.val;   int rd = left.depth;
    if ( lx->hash != rx->hash || lx->tag != rx->tag ) { return false; }
    switch ( lx->tag ) {
        case LEAF: return lx->LID == rx->LID;
        case VRBL: {
            bool l_outer = (lx->VID <= ld);
            bool r_outer = (rx->VID <= rd);
            return (l_outer == r_outer) && (
                lx->VID + (l_outer ? -ld : 0) == 
                rx->VID + (r_outer ? -rd : 0)
            );
        }
        case ABST: return same_node((Node){lx->BOD, ld+1}, (Node){rx->BOD, rd+1});
        case EVAL: return same_node((Node){lx->FUN, ld  }, (Node){rx->FUN, rd  }) &&
                          same_node((Node){lx->ARG, ld  }, (Node){rx->ARG, rd  });
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.1. Beta Substitution  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

LambExpr* shift(LambExpr* e, int vid, int gap_size); 

/*----------------  1.1.0. main loop  ---------------------------------------*/

LambExpr* subs(LambExpr* exp, int vid, LambExpr* val, int depth)
{
    switch ( exp->tag ) {
        case LEAF: return exp;
        case VRBL: return exp->VID == vid+depth ? shift(val, 0, depth) : exp; 
        case ABST: return abst_expr(subs(exp->BOD, vid, val, depth+1));
        case EVAL: return eval_expr(subs(exp->FUN, vid, val, depth  ),
                                    subs(exp->ARG, vid, val, depth  ));
    }
}

/*----------------  1.1.1. beta substitution helper  ------------------------*/

LambExpr* shift(LambExpr* e, int vid, int gap_size) 
    /*  /shift/: Create an expression analogous to /e/ except that references
        to outer variables (i.e. those /vid/ or more levels up) are displaced
        by /gap_size/.  For /gap_size/ positive, this creates a gap so that the
        resulting expression makes no reference to the variable /vid/ levels up
        from /e/.  For /gap_size/ equal to -1, this removes a gap, e.g. the gap
        created upon beta-elimination of an abstraction whose body is /e/.      
    */
{
    switch ( e->tag ) {
        case LEAF: return e;
        case VRBL: return e->VID < vid ? e : vrbl_expr(e->VID + gap_size);
        case ABST: return abst_expr(shift(e->BOD, vid+1, gap_size));
        case EVAL: return eval_expr(shift(e->FUN, vid  , gap_size),
                                    shift(e->ARG, vid  , gap_size));
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.2. Dependency Querying  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool mentions_vrbl(LambExpr* e, int vid_lo, int vid_hi)
{
    switch ( e->tag ) {
        case LEAF: return false;
        case VRBL: return (vid_lo <= e->VID && e->VID < vid_hi);
        case ABST: return mentions_vrbl(e->BOD, vid_lo+1, vid_hi+1);
        case EVAL: return mentions_vrbl(e->FUN, vid_lo  , vid_hi  ) ||
                          mentions_vrbl(e->ARG, vid_lo  , vid_hi  );
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.3. Display  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void print_expr(LambExpr* e, char leaf_nms[][16])
{
    switch ( e->tag ) {
        case LEAF: crim(); { 
            if ( leaf_nms == NULL) { printf("f%d", e->LID); }
            else { printf("%s", leaf_nms[e->LID]); }
        } defc(); break;
        case VRBL: lime(); printf("%d", e->VID); defc(); break;
        case ABST: lime(); printf("\\."); defc();
                   print_expr(e->BOD, leaf_nms); break;
        case EVAL: { 
            bool wrap_fun = e->FUN->tag == ABST; 
            bool wrap_arg = (e->ARG->tag == EVAL || e->ARG->tag == ABST);

            if ( wrap_fun ) { printf("("); } print_expr(e->FUN, leaf_nms);
            if ( wrap_fun ) { printf(")"); }
            printf(" "); 
            if ( wrap_arg ) { printf("("); } print_expr(e-> ARG, leaf_nms);
            if ( wrap_arg ) { printf(")"); }
            break;
        }
    }
} 
