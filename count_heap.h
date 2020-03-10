/*  author: samtenka
 *  change: 2020-03-09
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#ifndef COUNT_HEAP
#ifdef BASIC_MAPPING_H

typedef struct Counter Counter;
struct Counter {
    pint_by_int counted_idxs_by_keys;
    int_by_pint keys_by_counted_idxs; //sorted by counts
    int next_idx;
};

void init_counter(Counter* chp);
void counter_observe(Counter* chp, int key);
int pop_most_frequent(Counter* chp);

#define COUNT_HEAP
#endif//BASIC_MAPPING_H
#endif//COUNT_HEAP
