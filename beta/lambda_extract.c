/*  author: samtenka
 *  change: 2020-05-06
 *  create: 2020-05-06
 *  descrp: identify abstract concepts that help to compress a given collection
 *          of lambda expressions
 *  to use:  
 */

#include <stdbool.h>
#include <stdlib.h>

#include "lambda.h"
#include "concept_table.h"

#define DEPTH 3 

/*===========================================================================*/
/*====  0. TEMPORARY STRUCTURES  ============================================*/
/*===========================================================================*/

/*===========================================================================*/
/*====  1. EXTRACTION  ======================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.0. Extract from Single Program  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Nodes Nodes;
struct Nodes {
    Node arr[1<<(DEPTH+1)];
    int len;
}; 

bool cull_sites(LambExpr* e, int depth, Nodes* sites, int syntax_depth);
void populate_kids(LambExpr* e, int depth, Nodes* kids, int syntax_depth);
LambExpr* bod_from(LambExpr* e, int depth, Node val);

void extract_to(LambExpr* e, CTable* ct)
{
    Nodes sites;
    bool has_target = cull_sites(e, 0, &sites, 0);
    if ( ! has_target ) { populate_kids(e, 0, &sites, 0); }

    int nb_occurences, d_score;
    for ( int i=0; i != sites.len; ++i ) {
        Node val = sites.arr[i];
        LambExpr* bod = bod_from(e, 0, val); 
        d_score = e->weight - (bod->weight + val.val->weight + 2);
        update_table(ct, bod, d_score);
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.1. Helper: Perform Greedy Inverse Beta Substitution  ~~~~~~~~*/

LambExpr* bod_from(LambExpr* e, int depth, Node val)
{
    if ( same_node((Node){e, depth}, val) ) { return vrbl_expr(0); }
    switch ( e->tag ) {
        case LEAF: return e;
        case VRBL: return ( e->VID < depth ) ? e : vrbl_expr(e->VID+1);
        case ABST: return abst_expr(bod_from(e->BOD, depth+1, val));
        case EVAL: return eval_expr(bod_from(e->FUN, depth  , val),
                                    bod_from(e->ARG, depth  , val));
    }
}

/*===========================================================================*/
/*====  2. CULLING  =========================================================*/
/*===========================================================================*/

/*  OVERVIEW
    --------
    These routines compute the set of maximal subsets S of the nodes of the
    given lambda expression /e/ that enjoy the properties described below.  As
    described below, each such S can be described by giving some single node N
    of /e/, so we represent the set of Ss by populating the call-by-ref
    argument /sites/.

    PROPERTIES OF NODES
    -------------------
    A node N in /e/ is **REPLACEABLE** when:
        (A) it does not refer to any lambda node that both contains N and is
            equal to or contained by /e/
    A node N in /e/ is a **TARGET** when either:
        (A) it contains nodes of depth (w.r.t. /e/) exceeding DEPTH or
        (B) it refers to a lambda node containing /e/.
    Two nodes N, M in /e/ are **SYNTACTICALLY EQUAL** when simultaneously:
        (A) they have isomorphic syntax trees if we treat all their variable
            nodes as equivalent,
        (B) a node N' in N refers to a lambda node L containing N if and only
            if the corresponding node M' in M also refers to L, where L also
            contains M, and
        (C) a node N' in N refers to a lambda node inside N if and only if the
            corresponding node M' in M refers by the same index to some lambda
            node inside M.
    
    SETS OF INTEREST
    ----------------
    What concerns us are sets S of REPLACEABLE nodes in /e/ --- with no node in
    S containing another in S --- such that every TARGET node is contained in
    or equal to some element of S.  We are especially interested in the case
    where all nodes of S are syntactically equal. 

    ON MAXIMALITY
    -------------
    Whenever S is as described above and S has some member N, then we may add
    any other node M in /e/ that is syntactically equal to N into S while
    preserving the properties above.  Indeed, M is replaceable because N is,
    and nodes cannot contain syntactically equal nodes.  Therefore, any S
    maximal w.r.t. the properties above is equal to the syntactic equivalence
    class of any of its elements.  We call each of these elements a "site".
*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.0. Targetless Case  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void populate_kids(LambExpr* e, int depth, Nodes* kids, int syntax_depth)
    /*  Assuming /e/ contains no targets, populate /sites/ with the set of S's.
    */
{
    bool is_replaceable = ( ! mentions_vrbl(e, 1, depth) );
    if ( is_replaceable && syntax_depth != 0 ) {
        kids->arr[kids->len] = (Node){.val = e, .depth = depth};
        kids->len += 1;
    }

    if ( syntax_depth == DEPTH ) { return; }

    switch ( e->tag ) {
        case LEAF: break; 
        case VRBL: break;
        case ABST: populate_kids(e->BOD, depth+1, kids, syntax_depth+1); break; 
        case EVAL: populate_kids(e->FUN, depth  , kids, syntax_depth+1);        
                   populate_kids(e->ARG, depth  , kids, syntax_depth+1); break; 
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.1. Targetful Case  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void intersect_to(Nodes const* as, Nodes const* bs, Nodes* shared);
void copy_to(Nodes const* as, Nodes* shared);

bool cull_sites(LambExpr* e, int depth, Nodes* sites, int syntax_depth)
    /*  If e has targets, populate /sites/ with the set of S's and return
        /true/.  Otherwise, erase /sites/ and return /false/.
    */
{
    bool is_replaceable = ! mentions_vrbl(e, 1, depth);
    bool has_target = false; /* to be set below */

    sites->len = 0;
    if ( syntax_depth != DEPTH ) {
        switch ( e->tag ) {
            case LEAF: break; 
            case VRBL: break;
            case ABST:
                has_target |= cull_sites(e->BOD, depth+1, sites, syntax_depth+1);
                break; 
            case EVAL: {
                Nodes as;
                Nodes bs;
                bool a_has_target = cull_sites(e->FUN, depth, &as, syntax_depth+1);
                bool b_has_target = cull_sites(e->ARG, depth, &bs, syntax_depth+1);
                if ( a_has_target && b_has_target ) { intersect_to(&as, &bs, sites); }
                else if ( a_has_target ) { copy_to(&as, sites); }
                else if ( b_has_target ) { copy_to(&bs, sites); }
                has_target |= a_has_target || b_has_target;
            } break;
        }
    }

    if ( ! has_target ) {
        sites->len = 0;
    } else if ( is_replaceable && syntax_depth != 0 ) {
        sites->arr[sites->len] = (Node){.val = e, .depth = depth}; 
        sites->len += 1;
        has_target |= true;
    }

    return has_target;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.2. Helpers  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void copy_to(Nodes const* as, Nodes* shared)
{
    for ( int i=0; i != as->len; ++i ) {
        shared->arr[shared->len] = as->arr[i]; 
        shared->len += 1;
    }
}

void intersect_to(Nodes const* as, Nodes const* bs, Nodes* shared)
{
    for ( int i=0; i != as->len; ++i ) {
        for ( int j=0; j != bs->len; ++j ) {
            if ( same_node(as->arr[i], bs->arr[j]) ) { continue; }
            shared->arr[shared->len] = as->arr[i]; 
            shared->len += 1;
            break;
        }
    }
}


