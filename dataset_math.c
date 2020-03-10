/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include "verbose.h"
#include "fixpoint.h"

int is_square(int n);
int gcd(int a, int b);

void populate_math(Tasks* tsp)
{
    const int pt_dim = 7;    
    const int nb_samples = 128; 
    const int nb_tasks = 24;

    init_tasks(tsp, nb_tasks);

    for (int ti=0; ti!=nb_tasks; ++ti) {
        Task t = {
            make_charss(nb_samples/2),
            make_charss(nb_samples/2),
            pt_dim
        };
        push_tasks(tsp, t);

        for (int si=0; si!=nb_samples; ++si) {
            char label = (
                //ti <  6 ? ((((si>>ti)%2) ^ ((si>>((ti+5)%6))%2)) ? 1 : 0) :
                  ti < 24 ? (si%(3)==0 ? 1 : 0) : 0
                //ti < 12 ? (si%(3*ti+6)==0 ? 1 : 0) :
                //          ((si*ti)%9<=1 ? 1 : 0)  
                //ti < 24 ? (si%3==0 ? 1 : 0) : 0
                //ti < 18 ? (is_square(si+(ti%6))||is_square(si-(ti%6)) ? 1 : 0) :
                //ti < 24 ? (gcd(ti, si)==1 ? 1 : 0) :
                //          (gcd(ti, si)!=2 ? 1 : 0)
            );
            charss* points = (
                label ? &(tsp->data[ti].pospoints) :
                        &(tsp->data[ti].negpoints)
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

void print_math_feature(TaskView const* tvp, int didx)
{
    char labels[128];
    int i;
    for (i=0; i!=128; ++i) {
        labels[i] = 0;
    } 

    chars* point;
    i=0;
    for each (point, tvp->negpoints) {
        int pt_val = 0;
        for (int d=0; d!=7; ++d) {
            pt_val += (point->data[d] ? 1 : 0) << d;
        }
        labels[pt_val] = point->data[didx]==1 ? +1 : -1;
    } 
    for each (point, tvp->pospoints) {
        int pt_val = 0;
        for (int d=0; d!=7; ++d) {
            pt_val += (point->data[d] ? 1 : 0) << d;
        }
        labels[pt_val] = point->data[didx]==1 ? +1 : -1;
    } 
    printf("%3d[", didx);
    for (i=0; i!=128; ++i) {
        printf("%s", (
            (labels[i] == -1) ? "\033[31m." :
            (labels[i] == +1) ? "\033[32m@" :
                                "\033[36m "
        )); 
    }
    printf("\033[36m]");
}

void print_math(TaskView const* tvp)
{
    char labels[128];
    int i;
    for (i=0; i!=128; ++i) {
        labels[i] = 0;
    } 

    chars* point;
    i=0;
    for each (point, tvp->negpoints) {
        int pt_val = 0;
        for (int didx=0; didx!=7; ++didx) {
            pt_val += (point->data[didx] ? 1 : 0) << didx;
        }
        labels[pt_val] = -1;
    } 
    for each (point, tvp->pospoints) {
        int pt_val = 0;
        for (int didx=0; didx!=7; ++didx) {
            pt_val += (point->data[didx] ? 1 : 0) << didx;
        }
        labels[pt_val] = +1;
    } 
    printf("[");
    for (i=0; i!=128; ++i) {
        printf("%s", (
            (labels[i] == -1) ? "\033[31m." :
            (labels[i] == +1) ? "\033[32m@" :
                                "\033[36m "
        )); 
    }
    printf("\033[36m]");
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
