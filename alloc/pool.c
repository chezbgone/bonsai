/*  author: samtenka
 *  create: 2020-04-28
 *  change: 2020-04-28
 *  descrp: Custom allocator to store many *small* allocations, i.e.
 *              allocations whose size is dwarfed by WORDS_PER_POOL
 *          Errors will occur if Pool used otherwise.  We also assume that 
 *          the type /long/ matches the system word size.  All sizes below, 
 *          unless otherwise specified, are in measured in system words.
 *  to use:
 */

#include <stdlib.h>
#include <stdio.h>
#include "pool.h"

long available_words(BlockHeader* b)
{
    return b->capacity - b->size;
}
BlockHeader* fst_block_of(PoolHeader* p)
{
    return (BlockHeader*) ( ((long*)p) + WORDS_PER_POOL_HEADER );
}

PoolHeader* make_pool(PoolHeader* prv)
{
    PoolHeader* p;
    p = malloc(WORDS_PER_POOL);

    p->next_pool = NULL;
    p->prev_pool = prv;

    BlockHeader* fst = fst_block_of(p); 
    fst->size = WORDS_PER_BLOCK_HEADER;
    fst->capacity = WORDS_PER_POOL - WORDS_PER_POOL_HEADER;
    fst->prev_block = NULL; 
    fst->next_block = NULL;
    fst->prev_avail = NULL;
    fst->next_avail = NULL;

    return p;
}

long* moo_alloc(PoolHeader* p, int nb_words)
{
    long requested_words = nb_words + WORDS_PER_BLOCK_HEADER; 
    BlockHeader* prv;
    BlockHeader* cur; 
    BlockHeader* nxt; 

    /* search for a sufficiently large free block */
    {
        prv = fst_block_of(p);

        while ( prv==NULL || available_words(prv) < requested_words ) {
            if ( prv == NULL ) { 
                /* go to next pool, creating it if necessary */
                if ( p->next_pool == NULL ) { p->next_pool = make_pool(p); }
                p = p->next_pool;
                prv = fst_block_of(p);
            } else {
                /* go to next block in pool */
                prv = prv->next_avail;  
            }
        }

        cur = (BlockHeader*) ( ((long*)prv) + prv->size );
        nxt = prv->next_block; 
    }

    /* update size data */
    {
        cur->size = requested_words; 
        cur->capacity = available_words(prv);
        prv->capacity = prv->size; 
    }

    /* insert /cur/ into sparse chain of free blocks */
    {
        if ( cur->size == cur->capacity) {
            /* this allocation filled up, so update /nxt/'s backward: */
            if ( nxt != NULL ) { nxt->prev_avail = prv->prev_avail; }
        } else {
            /* this allocation didn't fill up, so update /prv/'s forward: */
            prv->next_avail = cur; 
        }
    }

    /* insert /cur/ into dense chain of all blocks */
    {
        /* link /prv/ with /cur/ */
        cur->prev_block = prv;
        prv->next_block = cur; 

        /* link /nxt/ with /cur/ */
        cur->next_block = nxt;
        if ( nxt!=NULL ) { nxt->prev_block = cur; }
    }

    return ((long*)cur) + WORDS_PER_BLOCK_HEADER;
} 

long moo_free(long* allocation)
{
    BlockHeader* cur = (BlockHeader*) ( allocation - WORDS_PER_BLOCK_HEADER );
    BlockHeader* prv = cur->prev_block;
    BlockHeader* nxt = cur->next_block;

    if (prv != NULL) {
        /* make /prev/ point forward to /next/ */
        prv->next_block = nxt;
        prv->next_avail = cur->next_avail; 
        prv->capacity += cur->capacity; 
    }
    if (nxt != NULL) {
        /* make /next/ point backward to /prev/ */
        nxt->prev_block = prv;
        nxt->prev_avail = prv;
    }
}

void free_pool(PoolHeader* p)
{
    PoolHeader* next = p->next_pool;
    free((long*)p);
    if (next != NULL) { free(next); }
}

void print_pool(PoolHeader* p)
{
    int i = 0;
    BlockHeader* cur;
    while ( p != NULL ) {
        printf("  %d:", i++);
        cur = fst_block_of(p);
        while ( cur != NULL ) {
            if (cur->size != cur->capacity) {
                printf("\033[33m");
            } else {
                printf("\033[32m");
            }
            printf("[%d/%d]", cur->size, cur->capacity);
            printf("\033[36m");
            cur = cur->next_block;
        } 
        printf("\n");
        p = p->next_pool; 
    }
    printf("\n");
} 
