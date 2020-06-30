/*  author: samtenka
 *  change: 2020-06-29
 *  create: 2020-06-29
 *  descrp: 
 *  to use: 
 */

#include "lambda.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.0. Normalize  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool is_normal(LambExpr* e);
LambExpr* beta_pass(LambExpr* e);

/*--------------------  0.0.0. main loop  -----------------------------------*/

LambExpr* normalize(LambExpr* e)
{
    while ( ! is_normal(e) ) { e = beta_pass(e); }
    return e;
}

/*--------------------  0.0.1. helpers  -------------------------------------*/

bool is_normal(LambExpr* e)
{
    if ( e->tag == EVAL && e->FUN->tag == ABST ) { return false; }
    switch ( e->tag ) {
        case LEAF: return true;
        case VRBL: return true;
        case EVAL: return is_normal(e->FUN) && is_normal(e->ARG); 
        case ABST: return is_normal(e->BOD);
    }
}

LambExpr* beta_pass(LambExpr* e)
{
    if ( e->tag == EVAL && e->FUN->tag == ABST ) {
        return beta_pass(
            shift(subs(e->FUN->BOD, 0, e->ARG, 0), 0, -1)
        );
    }

    switch ( e->tag ) {
        case LEAF: return e;
        case VRBL: return e;
        case EVAL: return eval_expr(beta_pass(e->FUN), beta_pass(e->ARG)); 
        case ABST: return abst_expr(beta_pass(e->BOD));
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Syllabize  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool is_unwrap(LambExpr* e);
LambExpr* unwrap(LambExpr* e, LambExpr* const* lib);

/*--------------------  0.1.0. main loop  -----------------------------------*/

LambExpr* syllabize(LambExpr* e, LambExpr* const* lib)
{
    while ( ! is_unwrap(e) ) { e = unwrap(e, lib); }
    return normalize(e);
}

/*--------------------  0.1.1. helpers  -------------------------------------*/

bool is_unwrap(LambExpr* e)
{
    if ( e->tag == EVAL && e->FUN->tag == ABST ) { return false; }
    switch ( e->tag ) {
        case LEAF: return 0 <= e->LID;
        case VRBL: return true;
        case EVAL: return is_unwrap(e->FUN) && is_normal(e->ARG); 
        case ABST: return is_unwrap(e->BOD);
    }
}

LambExpr* unwrap(LambExpr* e, LambExpr* const* lib)
{
    switch ( e->tag ) {
        case LEAF: return e->LID < 0 ? lib[-e->LID-1] : e;
        case VRBL: return e;
        case EVAL: return eval_expr(unwrap(e->FUN, lib), unwrap(e->ARG, lib)); 
        case ABST: return abst_expr(unwrap(e->BOD, lib));
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.2. Low-Level Variable Manipulation  ~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------  1.2.0. substitute  ----------------------------------*/

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

/*--------------------  1.2.1. mutate dependencies  -------------------------*/

LambExpr* shift(LambExpr* e, int vid, int gap_size) 
{
    switch ( e->tag ) {
        case LEAF: return e;
        case VRBL: return e->VID < vid ? e : vrbl_expr(e->VID + gap_size);
        case ABST: return abst_expr(shift(e->BOD, vid+1, gap_size));
        case EVAL: return eval_expr(shift(e->FUN, vid  , gap_size),
                                    shift(e->ARG, vid  , gap_size));
    }
}

/*--------------------  1.2.2. query dependencies   -------------------------*/

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
