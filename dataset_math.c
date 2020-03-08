/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include "dataset.h"

int is_square(int n);
int gcd(int a, int b);

void populate_math(Tasks* tsp)
{
    const int pt_dim = 6;    
    const int nb_samples = 64; 
    const int nb_tasks = 24;

    tsp->pt_dim = pt_dim;
    tsp->nb_tasks = nb_tasks;

    init_charsss(&(tsp->negpointss), nb_tasks);
    init_charsss(&(tsp->pospointss), nb_tasks);

    for (int ti=0; ti!=nb_tasks; ++ti) {
        push_charsss(&(tsp->negpointss), make_charss(nb_samples/2));
        push_charsss(&(tsp->pospointss), make_charss(nb_samples/2));

        for (int si=0; si!=nb_samples; ++si) {
            char label = (
                ti <  6 ? ((((si>>ti)%2) ^ ((si>>((ti+5)%6))%2)) ? 1 : 0) :
                ti < 12 ? (si%ti==0 ? 1 : 0) :
                ti < 18 ? (is_square(si+(ti%6))||is_square(si-(ti%6)) ? 1 : 0) :
                          (gcd(ti, si)==1 ? 1 : 0)
            );
            charss* points = (
                label ? &(tsp->pospointss.data[ti]) :
                        &(tsp->negpointss.data[ti])
            );
            push_charss(points, make_chars(pt_dim));

            for (int di=0; di!=pt_dim; ++di) {
                push_chars(&(points->data[points->len-1]),
                    ((si & (1<<di)) ?  1 : 0)
                );
            } 
        }
    }
}

void print_math(TaskView tv)
{
    char labels[64+1];
    for (int i=0; i!=64; ++i) {
        labels[i] = '?';
    } 
    labels[64] = '\0';

    chars* point;
    for each(point, tv.negpoints) {
        int pt_val = 0;
        char* x;
        for each(x, *point) {
            pt_val += ((*x) ? 1 : 0)<<(x-point->data); 
        }
        labels[pt_val] = ' ';
    } 
    for each(point, tv.pospoints) {
        int pt_val = 0;
        char* x;
        for each(x, *point) {
            pt_val += ((*x) ? 1 : 0)<<(x-point->data); 
        }
        labels[pt_val] = '#';
    } 
    printf("[%s]", labels); 
}

int is_square(int n)
{
    int s = n/2 + 1;
    while (s*s > n) { --s; }
    return (s*s==n) ? 1 : 0;
}

int gcd(int a, int b)
{
    while (a && b) {
        if (a<b) {b %= a;}
        else     {a %= b;}
    }
    return a+b;
}


