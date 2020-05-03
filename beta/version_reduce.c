/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "lambda.h"
#include "version.h"

#define PRIME_A   10007
#define PRIME_B  100003
#define PRIME_C 1000003

/*===========================================================================*/
/*====  0. FORWARD BETA REWRITES  ===========================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.0. Main Loop  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* rewrite(LambExpr const* e)
{
    VerSpace* vs = vs_from_expr(e);
    beta_pass(vs);
    beta_pass(vs);
    //beta_pass(vs);
    return vs;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Initialization  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* vs_from_expr(LambExpr const* e)
{
    switch ( e->tag ) {
        case LEAF: return leaf_vs(e->data.leaf_idx);
        case VRBL: return vrbl_vs(e->data.vrbl_idx);
        case ABST: return abst_vs(vs_from_expr(e->data.abst.body));
        case EVAL: return eval_vs(vs_from_expr(e->data.eval.fun),
                                  vs_from_expr(e->data.eval.arg));
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Loop Body: one Pass of Beta Reduction  ~~~~~~~~~~~~~~~~~*/

VerSpace canonical_hole = VerSpace{
    .tag=VS_VRBL, .data={.vrbl_idx=-1},
    .next=NULL, .owner=NULL,
}; 

typedef struct VSwHole VSwHole;
struct VSwHole {
    VerSpace* context;
    VerSpace* hole_ptr;
};

#define MAX_SYNTAX_DEPTH 3
typedef struct Depths Depths;
struct Depths {
    int abst;
    int syntax;
};

void add_abst(VerSpace* vs, VSwHole vh, int abst_depth)
{
    VerSpace* old_hole = *hole_ptr;
    *hole_ptr = vrbl_vs(depth); 
    {
        VerSpace* body = copy_vs(context) 

        update_vs(vs, eval_vs(
            abst_vs(body),
            it->tag == VS_VRBL ? it : vrbl_vs(idx + 1 - abst_depth)
        ));
    }
    free_vs(hole_ptr);
    *hole_ptr = old_hole;
}

void inv_subs(VerSpace* vs, VSwHole vh, VerSpace* it, Depths depths)
{
    if ( ! ( it->tag == VS_VRBL || idx <= depths.abst) ) {
        add_abst(vs, vh, depths.abst);
    }

    if ( depths.syntax != MAX_SYNTAX_DEPTH ) {
        switch ( it->tag ) {
            case VS_LEAF: break;
            case VS_VRBL: break;
            case VS_ABST: {
                vh->hole = abst_vs(&canonical_hole); canonical_hole.owner = NULL;
                VSwHole deeper_vh  = {vh->context, vh->hole->data.abst.body};
                Depths deeper_d = {depths->abst+1, depths->syntax+1};
                {
                    inv_subs(vs, deeper_vh, it->hole->data.body, deeper_d);
                }
                free_vs(&(vh->hole))
            }
            case VS_EVAL: {
                VerSpace* fun = it->data.eval.fun;
                VerSpace* arg = it->data.eval.arg;
                {
                    vh->hole = eval_vs(&canonical_hole, vs_copy(arg)); canonical_hole.owner = NULL;
                    VSwHole deeper_vh = {vh->context, vh->hole->data.eval.fun};
                    Depths deeper_d = {depths->abst, depths->syntax+1};
                    {
                        inv_subs(vs, deeper_vh, fun, deeper_d);
                    }
                    free_vs(&(vh->hole))
                }
                {
                    vh->hole = eval_vs(vs_copy(fun), &canonical_hole); canonical_hole.owner = NULL;
                    VSwHole deeper_vh = {vh->context, vh->hole->data.eval.fun};
                    Depths deeper_d = {depths->abst, depths->syntax+1};
                    {
                        inv_subs(vs, deeper_vh, arg, deeper_d);
                    }
                    free_vs(&(vh->hole))
                }
            }                              
        }
    }

    if ( it->next != NULL ) {
        inv_subs(vs, context, hole_ptr, it->next, depth, max_depth)
    }
}

void inv_beta_pass_no_join(VerSpace* vs, int depth)
{
    /*----------------  0.1.0. inv beta on kids  ----------------------------*/
    switch ( vs->tag ) {
        case VS_LEAF: break;
        case VS_VRBL: break;
        case VS_ABST: inv_beta_pass(vs->data.abst.body); break;
        case VS_EVAL: inv_beta_pass(vs->data.eval.fun);
                      inv_beta_pass(vs->data.eval.arg); break;
    }

    /*----------------  0.1.1. inv beta on current node  --------------------*/
    update_vs()
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.1. Loop Body: one Pass of Beta Reduction  ~~~~~~~~~~~~~~~~~*/

VerSpace* subs_vs(int vrbl, VerSpace* vs, VerSpace* val);

bool beta_pass(VerSpace* vs)
{
    bool changed = false; 

    /*----------------  0.1.0. beta on kids  --------------------------------*/
    switch ( vs->tag ) {
        case VS_LEAF: break;
        case VS_VRBL: break;
        case VS_ABST: changed |= beta_pass(vs->data.abst.body); break;
        case VS_EVAL: changed |= beta_pass(vs->data.eval.fun);
                      changed |= beta_pass(vs->data.eval.arg); break;
    }
    if ( vs->next != NULL ) { changed |= beta_pass(vs->next); }

    /*----------------  0.1.1. beta on current node  ------------------------*/
    if ( vs->tag != VS_EVAL ) { return changed; }
    VerSpace* fun = vs->data.eval.fun;
    VerSpace* arg = vs->data.eval.arg;
    while ( fun != NULL ) {
        if ( fun->tag == VS_ABST ) {
            /* TODO: UNWRAP VARIABLE INDICES!! */
            VerSpace* subbed = subs_vs(0, fun->data.abst.body, arg);
            beta_pass(subbed);

            if ( !contains_term(vs, subbed) ) {
                update_vs(vs, subbed);
                changed = true;
            } else {
                free_vs(&subbed);
            } 
        }
        fun = fun->next;
    }
    return changed;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.2. Private Helpers: Substitution  ~~~~~~~~~~~~~~~~~~~~~~~~~*/

VerSpace* subs_vs_no_join(int vrbl, VerSpace* vs, VerSpace* val);

VerSpace* subs_vs(int vrbl_idx, VerSpace* vs, VerSpace* val)
{
    VerSpace* head = subs_vs_no_join(vrbl_idx, vs, val); 
    while ( vs->next != NULL ) {
        VerSpace* tail = subs_vs_no_join(vrbl_idx, vs->next, val); 
        update_vs(head, tail);
        vs = vs->next;
    }
    return head;
}

VerSpace* subs_vs_no_join(int vrbl, VerSpace* vs, VerSpace* val)
{
    /* TODO: check whether in memoized...! */
    /* TODO: switch to identity if substitution would be trivial (all
     * occurences of idx are < vrbl)...! */
    switch ( vs->tag ) {
        case VS_LEAF: return vs;
        case VS_VRBL: {
            int idx = vs->data.vrbl_idx;
            return (vrbl == idx) ? val :
                   (vrbl <  idx) ? vrbl_vs(idx-1) : vs;
        }
        case VS_ABST: return abst_vs(subs_vs(vrbl+1, vs->data.abst.body, val));
        case VS_EVAL: return eval_vs(subs_vs(vrbl, vs->data.eval.fun, val),
                                     subs_vs(vrbl, vs->data.eval.arg, val));
    }
}

/*===========================================================================*/
/*====  2. COMPARISON AND DISPLAY  ==========================================*/
/*===========================================================================*/

bool contains_all_terms(VerSpace const* lhs, VerSpace const* rhs);
bool equals_vs_no_join(VerSpace const* lhs, VerSpace const* rhs);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.0. Equality and Containment  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool equals_vs(VerSpace const* lhs, VerSpace const* rhs)
{
    return contains_all_terms(lhs, rhs) &&
           contains_all_terms(rhs, lhs);
}

bool contains_term(VerSpace const* vs, VerSpace const* term)
{
    while ( vs != NULL ) {
        if ( equals_vs_no_join(vs, term) ) { return true; }
        vs = vs->next;
    }
    return false;
}

/*--------------------  2.0.1. private helpers  -----------------------------*/ 

bool contains_all_terms(VerSpace const* lhs, VerSpace const* rhs)
{
    VerSpace const* rhs_it = rhs;
    while ( rhs_it != NULL ) {
        if ( !contains_term(lhs, rhs_it) ) { return false; }
        rhs_it = rhs_it->next;
    }
    return true;
}

bool equals_vs_no_join(VerSpace const* lhs, VerSpace const* rhs)
{
    if ( lhs->tag != rhs->tag ) { return false; }
    switch ( lhs->tag ) {
        case VS_LEAF: return lhs->data.leaf_idx == rhs->data.leaf_idx;
        case VS_VRBL: return lhs->data.vrbl_idx == rhs->data.vrbl_idx;
        case VS_ABST:
            return equals_vs(lhs->data.abst.body, rhs->data.abst.body);
        case VS_EVAL:
            return equals_vs(lhs->data.eval.fun, rhs->data.eval.fun) &&
                   equals_vs(lhs->data.eval.arg, rhs->data.eval.arg);
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.1. Hash Function  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int hash_vs_no_join(VerSpace const* vs);

/*--------------------  2.1.0. outer loop: sum terms of union  --------------*/ 

int hash_vs(VerSpace const* vs)
{
    int accum = 0;
    while ( vs != NULL ) {
        accum += hash_vs_no_join(vs->next); 
        vs = vs->next;
    }
    return accum;
}

/*--------------------  2.1.1. inner loop: homomorphism on lambda syntax  ---*/ 

int hash_vs_no_join(VerSpace const* vs)
{
    switch (vs->tag) {
        case VS_LEAF: return 1 + (int)(vs->data.leaf_idx)<<8;
        case VS_VRBL: return 3 + (int)(vs->data.vrbl_idx)<<8;
        case VS_ABST: return 1 + PRIME_A * hash_vs(vs->data.abst.body);
        case VS_EVAL: return 1 + PRIME_B * hash_vs(vs->data.eval.fun) 
                               + PRIME_C * hash_vs(vs->data.eval.arg);
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.2. Printing  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void print_vs_no_disj(VerSpace const* vs, char leaf_names[][16]);

/*--------------------  2.2.0. outer loop: wrap in union brackets  ----------*/ 

void print_vs(VerSpace const* vs, char leaf_names[][16])
{
    if ( vs->next == NULL) {
        print_vs_no_disj(vs, leaf_names);
    } else {
        printf("[");
        print_vs_no_disj(vs, leaf_names);
        while ( vs->next != NULL ) {
            printf(" | ");
            print_vs_no_disj(vs->next, leaf_names);
            vs = vs->next;
        }
        printf("]");
    }
}

/*--------------------  2.2.1. inner loop: print a given union term  --------*/ 

void print_vs_no_disj(VerSpace const* vs, char leaf_names[][16])
{
    switch ( vs->tag ) {
        case VS_LEAF:
            crim();
            if ( leaf_names == NULL) { printf("f%d", vs->data.leaf_idx); }
            else { printf("%s", leaf_names[vs->data.leaf_idx]); }
            defc();
            break;
        case VS_VRBL: 
            lime();
            printf("%d", vs->data.vrbl_idx);
            defc();
            break;
        case VS_ABST:
            lime();
            printf("\\.");
            defc();
            print_vs(vs->data.abst.body, leaf_names);
            break;
        case VS_EVAL: { 
            VerSpace* fun = vs->data.eval.fun;
            VerSpace* arg = vs->data.eval.arg;

            if ( fun->tag == VS_ABST ) {
                printf("("); print_vs(fun, leaf_names); printf(")");
            }         else { print_vs(fun, leaf_names); }
            printf(" ");
            if ( arg->tag == VS_EVAL || arg->tag == VS_ABST ) {
                printf("("); print_vs(arg, leaf_names); printf(")");
            }         else { print_vs(arg, leaf_names); }
        } break;
    }
}
