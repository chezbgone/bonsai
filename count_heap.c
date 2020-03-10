/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include "verbose.h"
#include "fixpoint.h"
#include "count_heap.h"

void init_counter(Counter* chp)
{
    chp->counted_idxs_by_keys = make_pibi(NULL);
    chp->keys_by_counted_idxs = make_ibpi(NULL);
    chp->next_idx = 0;
}

void free_counter(Counter* chp)
{
    free_pibi(chp->counted_idxs_by_keys);
    free_ibpi(chp->keys_by_counted_idxs);
}

void counter_observe(Counter* chp, int key)
{
    pint_by_int* node = find_pibi(chp->counted_idxs_by_keys, key);

    if ( node->is_leaf ) {
        pint ic = {1, chp->next_idx++};
        setv_ibpi(chp->keys_by_counted_idxs, ic, key);
        setv_pibi(node, key, ic); 
        ++ chp->len;
    } else {
        pint new_ic = {node->value.fst+1, node->value.snd};
        chgk_ibpi(&(chp->keys_by_counted_idxs), node->value, new_ic); 
        setv_pibi(node, key, new_ic); 
    } 
};

pint pop_most_frequent(Counter* chp)
{
    int_by_pint* node = maxi_ibpi(chp->keys_by_counted_idxs);
    pint kc = {node->value, node->key.fst}; 
    deln_ibpi(&(chp->keys_by_counted_idxs), node);
    remk_pibi(&(chp->counted_idxs_by_keys), kc.fst);
    -- chp->len;
    return kc;
};
