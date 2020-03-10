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

void print_pi(pint_by_int* pi, int depth)
{
    for (int d=0; d!=depth; ++d) {
        printf(" | ");
    }
    if ( pi->is_leaf ) { printf(".\n"); return; }
    printf("\033[31m%d\033[36m : %d,%d\n", pi->key, pi->value.fst, pi->value.snd);
    print_pi(pi->left, depth+1);
    print_pi(pi->rght, depth+1);
}
int XXX = 9;
void print_ip(int_by_pint* ip, int depth)
{

    if (depth>=XXX) {
        WAIT_FOR_ENTER();
        XXX=8;
    }
    if ( ip->is_leaf ) { /*printf(".\n");*/ return; }

    for (int d=0; d!=depth; ++d) {
        printf(" | ");
    }
    printf("%d,%d : \033[32m%d\033[36m (par \033[31m%d\033[36m)\n",
            ip->key.fst, ip->key.snd, ip->value, ip->prnt==NULL?-1:ip->prnt->value);
    print_ip(ip->left, depth+1);
    print_ip(ip->rght, depth+1);
}
void print_moo(Counter* c)
{
    //print_pi(c->counted_idxs_by_keys, 0);
    print_ip(c->keys_by_counted_idxs, 0);
}

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

        //chgk_ibpi(&(chp->keys_by_counted_idxs), node->value, new_ic); 
        {
            //printf("about to update old key %d...\n", key); 
            int_by_pint** mpp = &(chp->keys_by_counted_idxs);
            int_by_pint* found = find_ibpi(*mpp, node->value); 
            int vvv = found->value;
            deln_ibpi(mpp, found);

            setv_ibpi(*mpp, new_ic, vvv);

            //print_moo(chp);
            //printf("addd %d,%d to:\n", found->key.fst, found->key.snd);
            //print_moo(chp);

            //printf("set to:\n");
            //print_moo(chp);
        }

        setv_pibi(node, key, new_ic); 
        //printf("updated!");
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
