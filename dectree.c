/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-04
 * descrp: 
 * to use: 
 */

#include <stdlib.h>
#include <stdio.h>
#include "vector.h" 
#include "dectree.h" 
#include "verbose.h"

DecTree* malloc_tree();
void grow_tree(DecTree* dtp);
float entropy_extensive(int A, int B);
float info_of(TaskView tv);
float info_of_split(TaskView tv, int didx);
void split_at(TaskView tv, int didx, TaskView* left, TaskView* right);
void train_subtree(DecTree* dtp, TaskView t, int depth);
void print_tree_row(DecTree const* dtp, int row);
//DecTree* train_tree(TaskView t);
//void free_tree(DecTree* dtp);

int nb_leaves(DecTree const* dtp)
{
    switch (dtp->node_type) {
        case NT_LEAF :
            return 1;
        case NT_PRED :
            return (
                nb_leaves(dtp->left) +
                nb_leaves(dtp->rght)
            );
    }
}
int nb_nodes(DecTree const* dtp)
{
    switch (dtp->node_type) {
        case NT_LEAF :
            return 1;
        case NT_PRED :
            return (
                nb_nodes(dtp->left) + 1 +
                nb_nodes(dtp->rght)
            );
    }
}

int depth(DecTree const* dtp)
{
    int dleft, dright;
    switch (dtp->node_type) {
        case NT_LEAF :
            return 0;
        case NT_PRED :
            dleft  = depth(dtp->left);
            dright = depth(dtp->rght);
            return (dleft<dright ? dright : dleft) + 1;
    }
}

void print_tree(DecTree const* dtp)
{
    int j;
    for (j=0; j!=nb_nodes(dtp); ++j) {
        printf("-");
    }
    printf("\n");
    for (int r=0; r!=depth(dtp)+1; ++r) {
        print_tree_row(dtp, r);
        printf("\n");
    }
    for (j=0; j!=nb_nodes(dtp); ++j) {
        printf("-");
    }
    printf("\n");
}

void print_tree_row(DecTree const* dtp, int row)
{
    if (row) {
        switch (dtp->node_type) {
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
        switch (dtp->node_type) {
            case NT_LEAF :
                printf("%s",
                    dtp->annotation.value==+1 ?
                    "\033[32m+\033[36m" :
                    "\033[31m-\033[36m"
                );
                return;
            case NT_PRED :
                for (j=0; j!=nb_nodes(dtp->left); ++j) {
                    printf(" ");
                }
                printf("%d", dtp->annotation.didx);
                for (j=0; j!=nb_nodes(dtp->rght); ++j) {
                    printf(" ");
                }
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
    if (dtp->node_type == NT_LEAF) { 
        dtp->left = malloc_tree(); 
        dtp->rght = malloc_tree(); 
        dtp->node_type = NT_PRED; 
    } else {
        BARK(VERBOSE_DECTREE_MEM, "attempted to grow a non-leaf!\n");
    }
}

float entropy_extensive(int A, int B)
{
    float ratio = ((float)A)/(A+B);
    float quad = ratio * (1.0-ratio);
    /* good approximation of shannon entropy times (A+B): */
    return (1.7 + 2.0 * quad) * quad * (A+B);
}

float info_of(TaskView tv)
{
    return entropy_extensive(
        tv.negpoints.len,
        tv.pospoints.len
    );
}

float info_of_split(TaskView tv, int didx)
{
    int nay_neg=0;
    int nay_pos=0;
    int yea_neg=0;
    int yea_pos=0;

    chars* point; 
    for each(point, tv.negpoints) {
        switch ( point->data[didx] ) {
            case NAY : nay_neg += 1; break; 
            case YEA : yea_neg += 1; break; 
        }
    }
    for each(point, tv.pospoints) {
        switch ( point->data[didx] ) {
            case NAY : nay_pos += 1; break; 
            case YEA : yea_pos += 1; break; 
        }
    }

    return (
        entropy_extensive(yea_neg, yea_pos) + 
        entropy_extensive(nay_neg, nay_pos)   
    );
}

void split_at(TaskView tv, int didx, TaskView* left, TaskView* rght)
{
    empt_taskview(left, tv.pt_dim, (2*len_taskview(tv))/3);
    empt_taskview(rght, tv.pt_dim, (2*len_taskview(tv))/3);

    chars* point; 
    for each(point, tv.negpoints) {
        switch ( point->data[didx] ) {
            case NAY : push_charss(&(left->negpoints), *point); break; 
            case YEA : push_charss(&(rght->negpoints), *point); break;
        }
    }
    for each(point, tv.pospoints) {
        switch ( point->data[didx] ) {
            case NAY : push_charss(&(left->pospoints), *point); break; 
            case YEA : push_charss(&(rght->pospoints), *point); break;
        }
    }
}

DecTree* train_tree(TaskView tv)
{
    DecTree* dtp = malloc_tree();
    train_subtree(dtp, tv, 0);
    return dtp;
}

void train_subtree(DecTree* dtp, TaskView tv, int depth)
{
    /* mask.len bounds info from above  */

    float best_info = info_of(tv);
    if (best_info==0.0) {
        dtp->annotation.value = tv.pospoints.len ? +1 : -1;
        return;
    }
    int best_didx; 

    for (int didx=0; didx!=tv.pt_dim; ++didx) {
        float info = info_of_split(tv, didx);
        if (!(info <= best_info)) { continue; }
        best_info = info;
        best_didx = didx;
    }

    for (int j=0; j!=depth; ++j) {
        BARK(VERBOSE_DECTREE_TRAIN, "| ");
    }
    BARK(VERBOSE_DECTREE_TRAIN,
        "split at dim %d to reduce info to %.1f\n", best_didx, best_info
    );

    TaskView left;
    TaskView rght;
    split_at(tv, best_didx, &left, &rght);
    {
        grow_tree(dtp);
        dtp->annotation.didx = best_didx;
        train_subtree(dtp->left, left, depth+1);
        train_subtree(dtp->rght, rght, depth+1);
    }
    wipe_taskview(&left);
    wipe_taskview(&rght);

    for (int j=0; j!=depth; ++j) {
        BARK(VERBOSE_DECTREE_TRAIN, "| ");
    }
    BARK(VERBOSE_DECTREE_TRAIN,
        "%d leaves for %d el.s\n", nb_leaves(dtp), len_taskview(tv) 
    );
}

void free_tree(DecTree* dtp)
{
    if (dtp->node_type == NT_PRED) {
        free_tree(dtp->left);
        free_tree(dtp->rght);
    }
    BARK(VERBOSE_DECTREE_MEM, "freed tree!\n");
    free(dtp);
}
