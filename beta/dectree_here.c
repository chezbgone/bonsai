/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include <stdio.h>

DecTree* malloc_tree();
void grow_tree(DecTree* dtp);
float entropy_extensive(int A, int B);

float info_of_split(TaskView const* tvp, int didx);
void split_at(TaskView const* tvp, int didx, TaskView* left, TaskView* right);
void train_subtree(DecTree* dtp, TaskView const* t, int depth);
void print_tree_row(DecTree const* dtp, int row);

int nb_leaves(DecTree const* dtp)
{
    switch ( dtp->node_type ) {
        case NT_LEAF: return 1;
        case NT_PRED: return nb_leaves(dtp->left) + 
                             nb_leaves(dtp->rght);
    }
}

int nb_nodes(DecTree const* dtp)
{
    switch ( dtp->node_type ) {
        case NT_LEAF: return 1;
        case NT_PRED: return 1 + nb_nodes(dtp->left)
                               + nb_nodes(dtp->rght);
    }
}

int depth(DecTree const* dtp)
{
    int dleft, dright;
    switch ( dtp->node_type ) {
        case NT_LEAF:
            return 0;
        case NT_PRED:
            dleft  = depth(dtp->left);
            dright = depth(dtp->rght);
            return (dleft<dright ? dright : dleft) + 1;
    }
}

void print_tree(DecTree const* dtp)
{
    int j;
    for ( j = 0; j != nb_nodes(dtp); ++j ) { printf("-"); }
    printf("\n");
    for ( int r = 0; r != depth(dtp)+1; ++r ) {
        print_tree_row(dtp, r);
        printf("\n");
    }
    for ( j = 0; j != nb_nodes(dtp); ++j ) { printf("-"); }
    printf("\n");
}

void print_tree_row(DecTree const* dtp, int row)
{
    if ( row ) {
        switch ( dtp->node_type ) {
            case NT_LEAF:
                printf(" ");
                return;
            case NT_PRED:
                print_tree_row(dtp->left, row-1);
                printf("\033[30m|\033[36m");
                print_tree_row(dtp->rght, row-1);
                return;
        }
    } else {
        int j;
        switch ( dtp->node_type ) {
            case NT_LEAF:
                printf("%s",
                    dtp->annotation.value==+1 ?
                    "\033[32m+\033[36m" :
                    "\033[31m-\033[36m"
                );
                return;
            case NT_PRED:
                for ( j = 0; j != nb_nodes(dtp->left)-1; ++j ) { printf(" "); }
                int didx = dtp->annotation.didx;
                if      ( didx <  10 ) { printf(" %d ", didx); }
                else if ( didx < 100 ) { printf("%d ",  didx); }
                else                   { printf("%d",   didx); }
                for ( j = 0; j != nb_nodes(dtp->rght)-1; ++j ) { printf(" "); }
                return;
        }
    }
}

DecTree* malloc_tree()
{
    DecTree* dtp = malloc(sizeof(DecTree));
    dtp->node_type = NT_LEAF; 
    return dtp;
}

void grow_tree(DecTree* dtp)
{
    if ( dtp->node_type == NT_LEAF ) { 
        dtp->left = malloc_tree(); 
        dtp->rght = malloc_tree(); 
        dtp->node_type = NT_PRED; 
    } else {
        BARK(VERBOSE_DECTREE_MEM, "attempted to grow a non-leaf!\n");
    }
}

float entropy_extensive(int A, int B)
{
    if ( A+B == 0 ) { return 0.0; }
    float ratio = ((float)A)/(A+B);
    float quad = ratio * (1.0-ratio);
    /* good approximation of shannon entropy times (A+B): */
    return (1.7 + 2.0 * quad) * quad * (A+B);
}

float info_of(TaskView const* tvp)
{
    return entropy_extensive(tvp->negpoints.len,
                             tvp->pospoints.len);
}
