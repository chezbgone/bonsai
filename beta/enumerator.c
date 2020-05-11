/*  author: samtenka
 *  change: 2020-05-09
 *  create: 2020-05-09
 *  descrp: interface for best-first program enumeration
 *  to use: 
 */

#include <stdlib.h> 
#include <stdio.h> 
#include "lambda.h" 
#include "type.h" 
#include "enumerator.h" 

bool is_func[]   = {false,true,true};
EType arg_type[] = {-1,TINT,TINT};
EType out_type[] = {-1,TINT,TINT_FRM_INT};

void insert(LambList* ll, ScoredLamb e)
{
    while ( ! (ll->len < ll->cap) ) {
        int new_cap = (3*ll->cap)/2 + 1;
        ScoredLamb* new_arr = malloc(sizeof(ScoredLamb) * new_cap);
        for ( int pi = 0; pi != ll->len; ++pi ) { new_arr[pi] = ll->arr[pi]; }
        free(ll->arr);
        ll->arr = new_arr;
        ll->cap = new_cap; 
    }
    ll->arr[ll->len] = e;
    ll->len += 1;
}

LambsByEType* init_lbt(Grammar const* G)
{
    LambsByEType* lbt = malloc(sizeof(LambsByEType));
    for ( int t = 0; t != NB_TYPES; ++t ) {
        lbt->arr[t] = (LambList){.arr = malloc(sizeof(ScoredLamb)*1), .len = 0, .cap = 1};
    }
    for ( int l = 0; l != G->nb_leaves; ++l ) {
        ScoredLamb sp = {.score = G->leaf_scores[l], .e = leaf_expr(l)};  
        insert(&(lbt->arr[G->leaf_types[l]]), sp);
    }
    for ( int t = 0; t != NB_TYPES; ++t ) {
        lbt->arr[t].active_lo = 0;
        lbt->arr[t].active_hi = lbt->arr[t].len;
    }
    return lbt;
}

LambList free_all_but(LambsByEType* lbt, EType target)
{
    LambList remaining = lbt->arr[target];  
    for ( int t = 0; t != NB_TYPES; ++t ) {
        if ( t == target ) { continue; } 
        free(lbt->arr[t].arr);
    }
    free(lbt);
    return remaining;
}

void pass(float eval_score, LambList* funs, LambList* outs, LambList* args, float min_score)
{ 
    for ( int fun_i = 0; fun_i != funs->active_hi; ++fun_i ) {
        ScoredLamb fun = funs->arr[fun_i]; 
        int arg_start = (fun_i < funs->active_lo) ? args->active_lo : 0;
        for ( int arg_i = arg_start; arg_i != args->active_hi; ++arg_i ) {
            ScoredLamb arg = args->arr[arg_i]; 
            float new_score = eval_score + fun.score + arg.score; 
            if ( new_score < min_score ) { continue; }
            ScoredLamb new_prog = {
                .score = new_score,
                .e = eval_expr(fun.e, arg.e)
            };
            insert(outs, new_prog);
        }
    }
}

// TODO: maintain /sorted/ lists? 

// TODO: can prune more if fun_t isn't of target type and has
//       a score close to min_score (need to do some computations
//       with G)
 
LambList enumerate(Grammar const* G, float min_score, EType target)
{
    LambsByEType* lbt = init_lbt(G);

    bool found = true; 
    while ( found ) {
        found = false;
        for ( EType fun_t = 0; fun_t != NB_TYPES; ++fun_t ) {
            if ( ! is_func[fun_t] ) { continue; }
            LambList* funs = &(lbt->arr[fun_t]);
            LambList* args = &(lbt->arr[arg_type[fun_t]]);
            LambList* outs = &(lbt->arr[out_type[fun_t]]);

            printf(
                "pass (%d):     fun %6d        arg %6d        out %6d\n",
                fun_t, funs->len, args->len, outs->len
            );
            int old_len = outs->len;
            pass(G->eval_score[fun_t], funs, outs, args, min_score); 
            found |= ( outs->len != old_len );
        }
        for ( int t = 0; t != NB_TYPES; ++t ) {
            lbt->arr[t].active_lo = lbt->arr[t].active_hi;
            lbt->arr[t].active_hi = lbt->arr[t].len;
        }
    }

    return free_all_but(lbt, target);
} 
