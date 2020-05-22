#include <stdio.h>
#include "../alloc/pool.h"

void basic_test(PoolHeader* p);
void big_test(PoolHeader* p);

void main() 
{
    printf("hi!\n\n");

    PoolHeader* p = make_pool(NULL); 
    PoolHeader* q = make_pool(NULL); 

    basic_test(p);
    big_test(q);

    free_pool(q); 
    free_pool(p); 

    printf("bye!\n");
}

void big_test(PoolHeader* p)
{
    printf("allocating many many blocks...\n");
    for ( int i = 0; i != 10000; ++ i) {
        moo_alloc(p, i%100);
    }
    printf("done!\n");
}

void basic_test(PoolHeader* p)
{
    printf("allocating small blocks...\n");
    long* a = moo_alloc(p, 23); 
    long* b = moo_alloc(p, 100); 
    long* c = moo_alloc(p, 1);
    print_pool(p);

    printf("freeing a middle block, creating a gap...\n");
    moo_free(b);
    print_pool(p);

    printf("add middle block again...\n");
    b = moo_alloc(p, 100); 
    print_pool(p);

    printf("free middle block again...\n");
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
}
