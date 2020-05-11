/*  author: samtenka
 *  create: 2020-05-10
 *  change: 2020-04-28
 *  descrp: Custom allocator to improve cache efficiency.
 *
 *          We store many *small* allocations, i.e. allocations whose size is
 *          dwarfed by WORDS_PER_POOL == 4096 words. Errors will occur if Pool
 *          is used otherwise.  
 *
 *          We assume that the type /long/ matches the system word size.  All
 *          sizes below, unless otherwise specified, are in measured in system
 *          words.
 *
 *  to use:
 */

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "pool.h"

long available_words(BlockHeader* b);
BlockHeader* fst_block_of(PoolHeader* p);

/*===========================================================================*/
/*====  0. (DE)ALLOCATION OF POOLS  =========================================*/
/*===========================================================================*/

PoolHeader* make_pool(PoolHeader* prv)
{
    PoolHeader* p;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.0. Allocate and Initializely Pool  ~~~~~~~~~~~~~~~~~~~~~~~~*/
    {
        p = malloc(sizeof(long) * WORDS_PER_POOL);
        p->next_pool = NULL;
        p->prev_pool = prv;
    }

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  0.1. Initialize the First Block in that Pool  ~~~~~~~~~~~~~~~*/
    {
        p->active = fst_block_of(p); 
        p->active->size = WORDS_PER_BLOCK_HEADER;
        p->active->capacity = WORDS_PER_POOL - WORDS_PER_POOL_HEADER;
        p->active->owner = p; 
        p->active->prev_block = NULL; 
        p->active->next_block = NULL;
        p->active->prev_avail = NULL;
        p->active->next_avail = NULL;
    }

    return p;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  0.2. Free Linked List of Pools given Head Pool  ~~~~~~~~~~~~~*/

void free_pool(PoolHeader* p)
{
    PoolHeader* next = p->next_pool;
    free((long*)p);
    if (next != NULL) { free(next); }
}

/*===========================================================================*/
/*====  1. (DE)ALLOCATION OF BLOCKS WITHIN A POOL  ==========================*/
/*===========================================================================*/

long* moo_alloc(PoolHeader* p, int nb_words)
{
    fprintf(stderr, "{{");
    long requested_words = nb_words + WORDS_PER_BLOCK_HEADER; 
    BlockHeader* prv;
    BlockHeader* cur; 
    BlockHeader* nxt; 

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  1.0. Search for a Sufficiently Large Free Block  ~~~~~~~~~~~~*/
    {
        bool found = false; 
        prv = p->active;

        while ( prv == NULL || available_words(prv) < requested_words ) {
            if ( prv == NULL ) { 
                fprintf(stderr, "A");
                /*----  1.0.0. go to next pool, creating it if necessary  ---*/
                if ( p->next_pool == NULL ) { p->next_pool = make_pool(p); }
                p = p->next_pool;
                found = false; 
                prv = p->active;
            } else {
                fprintf(stderr, "B");
                if ( !found && available_words(prv) ) {
                    found = true;
                    p->active = prv;
                }

                /*----  1.0.1. go to next block in pool  --------------------*/
                prv = prv->next_avail;  
            }
            fprintf(stderr, "?");
            if ( prv == NULL ) { fprintf(stderr, "*"); }
            else { int s = available_words(prv); }
            fprintf(stderr, "! ");
        }

        cur = (BlockHeader*) ( ((long*)prv) + prv->size );
        nxt = prv->next_block; 
    }

    fprintf(stderr, ":");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  1.1. Update Size Data  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    {
        cur->size = requested_words; 
        cur->capacity = available_words(prv);
        cur->owner = p;  

        prv->capacity = prv->size; 
    }
    
    fprintf(stderr, "#");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  1.2. Insert /cur/ into the Sparse Chain of Free Blocks  ~~~~~*/
    {
        if ( cur->size == cur->capacity) {
            /*--------  1.2.0. /cur/ is full, so update /nxt/'s backward  ---*/
            if ( nxt != NULL ) { nxt->prev_avail = prv->prev_avail; }
        } else {
            /*--------  1.2.1. /cur/ isn't full, so update /prv/'s forward  -*/
            prv->next_avail = cur; 
            cur->prev_avail = prv->prev_avail; 
        }
    }

    fprintf(stderr, "&");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~  1.3. Insert /cur/ into the Dense Chain of All Blocks  ~~~~~~~*/
    {
        /*------------  1.3.0. link /prv/ with /cur/  -----------------------*/
        cur->prev_block = prv;
        prv->next_block = cur; 

        /*------------  1.3.0. link /nxt/ with /cur/  -----------------------*/
        cur->next_block = nxt;
        if ( nxt == NULL ) {
            cur->next_avail = NULL;
        } else {
            nxt->prev_block = cur;
            cur->next_avail = nxt; 
        }
    }

    fprintf(stderr, "}} ");
    return ((long*)cur) + WORDS_PER_BLOCK_HEADER;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  1.4. Free a Block  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void moo_free(long* allocation)
{
    BlockHeader* cur = (BlockHeader*) ( allocation - WORDS_PER_BLOCK_HEADER );
    BlockHeader* prv = cur->prev_block;
    BlockHeader* nxt = cur->next_block;

    if ( allocation < (long*)(cur->owner->active) ) {
        cur->owner->active = prv;
    }

    if (prv != NULL) {
        /*------------  1.4.0. make /prv/ point forward to /nxt/  -----------*/
        prv->next_block = nxt;
        prv->next_avail = cur->next_avail; 
        prv->capacity += cur->capacity; 
    }
    if (nxt != NULL) {
        /*------------  1.4.1. make /nxt/ point backward to /prv/  ----------*/
        nxt->prev_block = prv;
        nxt->prev_avail = prv;
    }
}

/*===========================================================================*/
/*====  2. TESTING  =========================================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.0. Check Pool List for Well-Formedness  ~~~~~~~~~~~~~~~~~~~*/

bool check_pool(PoolHeader* p)
{
    BlockHeader* cur;
    BlockHeader* nxt ;
    while ( p != NULL ) {
        cur = fst_block_of(p);
        while ( cur != NULL ) {
            nxt = cur->next_block;
            if ( nxt != NULL ) {

                /*----  2.0.0. linked list order invariant  -----------------*/
                if ( nxt <= cur ) { return false; }

                /*----  2.0.1. size invariant  ------------------------------*/
                if ( cur->capacity < cur->size) { return false; }

                /*----  2.0.2. capacity invariant ---------------------------*/
                if ( ((long*)nxt) - ((long*)cur) != cur->capacity ) {
                    return false;
                }

            }
            cur = nxt;
        } 
        p = p->next_pool; 
    }
    return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~  2.1. Print Each Pool's Blocks on One Line  ~~~~~~~~~~~~~~~~~~*/

void print_pool(PoolHeader* p)
{
    if ( !check_pool(p) ) {
        printf("\033[31m");
        printf("cannot print ill-formed pool!");
        return;
    }
    int i = 0;
    BlockHeader* cur;
    while ( p != NULL ) {
        printf("  %d:", i++);
        cur = fst_block_of(p);
        while ( cur != NULL ) {

            if ( cur == p->active ) { lava(); printf("*"); }
            /*--------  2.1.0. print block, colored by whether it is full  --*/
            if ( cur->size == cur->capacity )  { lime(); } else { gold(); }
            printf("[%d/%d]", (int) cur->size, (int) cur->capacity);
            cur = cur->next_block;

        } 
        defc();
        printf("\n");
        p = p->next_pool; 
    }
    printf("\n");
} 

/*===========================================================================*/
/*====  3. DEFINE HELPERS  ==================================================*/
/*===========================================================================*/

long available_words(BlockHeader* b)
{
    return b->capacity - b->size;
}

BlockHeader* fst_block_of(PoolHeader* p)
{
    return (BlockHeader*) ( ((long*)p) + WORDS_PER_POOL_HEADER );
}


