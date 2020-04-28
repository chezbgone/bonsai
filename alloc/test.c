#include <stdio.h>
#include "pool.h"

void main() 
{
    printf("hi!\n\n");
    PoolHeader* p = make_pool(NULL); 

    printf("allocating small blocks...\n");
    long* a = moo_alloc(p, 23); 
    long* b = moo_alloc(p, 100); 
    long* c = moo_alloc(p, 1);
    print_pool(p);

    printf("freeing a middle block, creating a gap...\n");
    moo_free(b);
    print_pool(p);

    printf("allocating big blocks, necessitating new pool...\n");
    long* d = moo_alloc(p, 2900);
    long* e = moo_alloc(p, 1950);
    long* f = moo_alloc(p, 1950);
    print_pool(p);

    printf("freeing a big block, creating a gap...\n");
    moo_free(e);
    print_pool(p);

    printf("allocating two small blocks, filling gap and end...\n");
    long* g = moo_alloc(p, 80); 
    long* h = moo_alloc(p, 80); 
    print_pool(p);

    printf("freeing remaining big blocks...\n");
    moo_free(d);
    moo_free(f);
    print_pool(p);

    printf("freeing remaining small blocks...\n");
    moo_free(a);
    moo_free(c);
    moo_free(g);
    moo_free(h);
    print_pool(p);

    free_pool(p); 
    printf("bye!\n");
}
