/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include "vector.h"
#include "count_heap.h"


//bool contains_key(CountHeap* chp, int target_key);
//
////typedef struct CountHeap CountHeap;
////struct CountHeap {
////    ints keys_sorted;
////    ints heap_idxs_by_key_rank;
////    ints keys_heap;
////    ints counts_heap;
////};
//
//void init_count_heap(CountHeap* chp)
//{
//    init_ints(&(chp->keys_sorted), 8);
//    init_ints(&(chp->keys_heap), 8);
//    init_ints(&(chp->counts_heap), 8);
//}
//
//void insert(CountHeap* chp, int key)
//{
//    int idx = find(chp, key); 
//    if (chp->keys_sorted.len==0 || chp->keys_sorted[idx]!=key) {
//        // insert new key
//    } 
//}
//
//int pop_most_frequent(CountHeap* chp)
//{
//}
//
//int find(CountHeap* chp, int target_key)
//{
//    int lo = 0;
//    int hi = chp->keys_sorted.len;
//
//    int current_key;
//
//    while ( lo < hi-1 ) {
//        int mid = (hi+lo)/2;
//
//        current_key = chp->keys_sorted.data[mid];
//
//        if (target_key < current_key) {
//            hi = mid;
//        } else {
//            lo = mid;
//        }
//    }
//    return lo;
//}
