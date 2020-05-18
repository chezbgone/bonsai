/*  author: samtenka
 *  create: 2020-05-08
 *  change: 2020-04-28
 *  descrp: Custom allocator to improve cache efficiency when storing many
 *          *small* allocations (i.e. allocations whose size is dwarfed by
 *          WORDS_PER_POOL == 4096 words), especially when all these
 *          allocations will be freed at once.  
 *
 *  to use: Create, use, and destroy a memory pool as shown below.  We do *not* 
 *          need /moo_free/ all allocated variables, but we *do* need to call
 *          /free_pool/ after use.  Below, /moo_alloc(p, 23)/ returns a
 *          (word-aligned) allocation 23 words in size.  
 *
 *              PoolHeader* p = make_pool(NULL); 
 *              {
 *                  long* a = moo_alloc(p, 23); 
 *                  long* b = moo_alloc(p, 100); 
 *                  long* c = moo_alloc(p, 1950);
 *                  long* d = moo_alloc(p, 1);
 *
 *                  moo_free(c);
 *                  moo_free(b);
 *              }
 *              free_pool(p); 
 *
 *          Our implementation assumes that the type /long/ matches the system
 *          word size.  All sizes below, unless otherwise specified, are in
 *          measured in system words.  As noted in the description, requested
 *          allocations should be much smaller than WORDS_PER_POOL (4000 words
 *          is safe); in fact, errors will occur if Pool is used otherwise.
 *          Read and run test_pool.c to learn more about block size and
 *          internal behavior.
 */

#include <stdbool.h>

#define WORDS_PER_POOL (4096)

#define LSIZE(t) ((sizeof(t) + sizeof(long)-1) / sizeof(long)) 
#define MOO_ALLOC(pool, t, nb) ((void*)moo_alloc(pool, LSIZE(t) * nb))
#define MOO_FREE(ptr) (moo_free((long*)ptr))

typedef struct PoolHeader PoolHeader;
typedef struct BlockHeader BlockHeader;

#define WORDS_PER_POOL_HEADER 4
struct PoolHeader {
    PoolHeader* next_pool;  
    PoolHeader* prev_pool;  
    BlockHeader* active;
    bool is_full;
};

#define WORDS_PER_BLOCK_HEADER 7
struct BlockHeader {
    /* The following sizes include the size of the block header, not just    * 
     * the size of the data.                                                 */ 
    long size;
    long capacity;
    PoolHeader* owner;
    BlockHeader* prev_block; 
    BlockHeader* next_block;
    BlockHeader* prev_avail; /* pointer to block with unused space */ 
    BlockHeader* next_avail; /* pointer to block with unused space */
};

PoolHeader* make_pool(PoolHeader* prv);
long* moo_alloc(PoolHeader* p, int nb_words);
void moo_free(long* allocation);
void free_pool(PoolHeader* p);

void print_pool(PoolHeader* p); 
bool check_pool(PoolHeader* p); 
