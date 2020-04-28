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

#define WORDS_PER_POOL 4096 

typedef struct PoolHeader PoolHeader;
typedef struct BlockHeader BlockHeader;

#define WORDS_PER_POOL_HEADER 3
struct PoolHeader {
    PoolHeader* next_pool;  
    PoolHeader* prev_pool;  
};

#define WORDS_PER_BLOCK_HEADER 6
struct BlockHeader {
    /* The following sizes include the size of the block header, not just    * 
     * the size of the data.                                                 */ 
    long size;
    long capacity;
    BlockHeader* prev_block; 
    BlockHeader* next_block;
    BlockHeader* prev_avail; /* pointer to block with unused space */ 
    BlockHeader* next_avail; /* pointer to block with unused space */
};

PoolHeader* make_pool(PoolHeader* prv);
long* moo_alloc(PoolHeader* p, int nb_words);
long moo_free(long* allocation);
void free_pool(PoolHeader* p);

void print_pool(PoolHeader* p); 
