/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include "fixpoint.h"
#include "count_heap.h"

void init_counter(Counter* chp)
{
    init_pibi(&(chp->counted_idxs_by_keys));
    init_ibpi(&(chp->keys_by_counted_idxs));
    chp->next_idx = 0;
}

void counter_observe(Counter* chp, int key)
{
    pint_by_int* node = find_pibi(&(chp->counted_idxs_by_keys), key);

    if ( node->is_leaf ) {
        pint ic = {1, chp->next_idx++};
        setv_ibpi(&(chp->keys_by_counted_idxs), ic, key);
        setv_pibi(node, key, ic); 
    } else {
        pint new_ic = {node->value.fst+1, node->value.snd};
        chgk_ibpi(&(chp->keys_by_counted_idxs), node->value, new_ic); 
        setv_pibi(node, key, new_ic); 
    } 
};

int pop_most_frequent(Counter* chp)
{
    int_by_pint* node = maxi_ibpi(&(chp->keys_by_counted_idxs));
    int count = node->key.fst; 
    int key = node->value;
    deln_ibpi(&(chp->keys_by_counted_idxs), node);
    remk_pibi(&(chp->counted_idxs_by_keys), key);
    return count;
};
