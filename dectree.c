/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include <stdio.h>

#include "verbose.h" 
#include "fixpoint.h"

DecTree* malloc_tree();
void grow_tree(DecTree* dtp);
float entropy_extensive(int A, int B);

float info_of_split(TaskView const* tvp, int didx);
void split_at(TaskView const* tvp, int didx, TaskView* left, TaskView* right);
void train_subtree(DecTree* dtp, TaskView const* t, int depth);
void print_tree_row(DecTree const* dtp, int row);

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
                for (j=0; j!=nb_nodes(dtp->left)-1; ++j) {
                    printf(" ");
                }
                int didx = dtp->annotation.didx;
                if (didx < 10) {
                    printf(" %d ", didx);
                } else if (didx < 100) {
                    printf("%d ", didx);
                } else {
                    printf("%d", didx);
                }
                for (j=0; j!=nb_nodes(dtp->rght)-1; ++j) {
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
    if (A+B == 0) { return 0.0; }
    float ratio = ((float)A)/(A+B);
    float quad = ratio * (1.0-ratio);
    /* good approximation of shannon entropy times (A+B): */
    return (1.7 + 2.0 * quad) * quad * (A+B);
}

float info_of(TaskView const* tvp)
{
    return entropy_extensive(
        tvp->negpoints.len,
        tvp->pospoints.len
    );
}

float gain_from_op(TaskView const* tvp, DecTree const* dtp, NewDim const* new_dim)
{
    if (dtp->node_type == NT_LEAF) { 
        return 0.0;
    }
    
    int didx = dtp->annotation.didx;
    int didx_a = new_dim->didx_a; 
    int didx_b = new_dim->didx_b; 

    if (didx != didx_a && didx != didx_b) {
        TaskView left;
        TaskView rght;
        split_at(tvp, didx, &left, &rght);
        float gain = (
            gain_from_op(&left, dtp->left, new_dim) + 
            gain_from_op(&rght, dtp->rght, new_dim)   
        );
        wipe_taskview(&left);
        wipe_taskview(&rght);
        return gain;
    } else {
        int nay_neg=0;
        int nay_pos=0;
        int yea_neg=0;
        int yea_pos=0;

        chars* point; 
        for each(point, tvp->negpoints) {
            char* data = point->data;
            char val;
            switch ( new_dim->op ) {
                case OP_AND:    val =    data[didx_a]  & data[didx_b]; break;
                case OP_OR:     val =    data[didx_a]  | data[didx_b]; break;
                case OP_XOR:    val =    data[didx_a]  ^ data[didx_b]; break;
                case OP_IMPLIES:val = (1^data[didx_a]) | data[didx_b]; break;
            }
            switch ( val ) {
                case NAY : nay_neg += 1; break; 
                case YEA : yea_neg += 1; break; 
            }
        }
        for each(point, tvp->pospoints) {
            char* data = point->data;
            char val;
            switch ( new_dim->op ) {
                case OP_AND:    val =    data[didx_a]  & data[didx_b]; break;
                case OP_OR:     val =    data[didx_a]  | data[didx_b]; break;
                case OP_XOR:    val =    data[didx_a]  ^ data[didx_b]; break;
                case OP_IMPLIES:val = (1^data[didx_a]) | data[didx_b]; break;
            }
            switch ( val ) {
                case NAY : nay_pos += 1; break; 
                case YEA : yea_pos += 1; break; 
            }
        }

        //float baseline_a = info_of_split(tvp, didx_a);
        //float baseline_b = info_of_split(tvp, didx_b);
        //float min_baseline = baseline_a < baseline_b ? baseline_a : baseline_b;

        float baseline = info_of_split(tvp, didx);
        return baseline - (
            entropy_extensive(yea_neg, yea_pos) + 
            entropy_extensive(nay_neg, nay_pos)   
        );
    }
}

float info_of_split(TaskView const* tvp, int didx)
{
    int nay_neg=0;
    int nay_pos=0;
    int yea_neg=0;
    int yea_pos=0;

    chars* point; 
    for each(point, tvp->negpoints) {
        switch ( point->data[didx] ) {
            case NAY : nay_neg += 1; break; 
            case YEA : yea_neg += 1; break; 
        }
    }
    for each(point, tvp->pospoints) {
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

void split_at(TaskView const* tvp, int didx, TaskView* left, TaskView* rght)
{
    empt_taskview(left, tvp->pt_dim, (2*len_taskview(tvp))/3);
    empt_taskview(rght, tvp->pt_dim, (2*len_taskview(tvp))/3);

    chars* point; 
    for each(point, tvp->negpoints) {
        switch ( point->data[didx] ) {
            case NAY : push_charss(&(left->negpoints), *point); break; 
            case YEA : push_charss(&(rght->negpoints), *point); break;
        }
    }
    for each(point, tvp->pospoints) {
        switch ( point->data[didx] ) {
            case NAY : push_charss(&(left->pospoints), *point); break; 
            case YEA : push_charss(&(rght->pospoints), *point); break;
        }
    }
}

void init_tree(DecTree* dtp)
{
    dtp->node_type = NT_LEAF;
}

void train_tree(TaskView const* tvp, DecTree* dtp)
{
    train_subtree(dtp, tvp, 0);
}

void train_subtree(DecTree* dtp, TaskView const* tvp, int depth)
{
    /* mask.len bounds info from above  */

    float best_info = info_of(tvp);
    if (best_info==0.0) {
        dtp->annotation.value = tvp->pospoints.len ? +1 : -1;
        return;
    }
    int best_didx = depth % tvp->pt_dim; 

    for (int didx=0; didx!=tvp->pt_dim; ++didx) {
        float info = info_of_split(tvp, didx);
        if (!(info < best_info)) { continue; }
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
    split_at(tvp, best_didx, &left, &rght);
    {
        grow_tree(dtp);
        dtp->annotation.didx = best_didx;
        train_subtree(dtp->left, &left, depth+1);
        train_subtree(dtp->rght, &rght, depth+1);
    }
    wipe_taskview(&left);
    wipe_taskview(&rght);

    for (int j=0; j!=depth; ++j) {
        BARK(VERBOSE_DECTREE_TRAIN, "| ");
    }
    BARK(VERBOSE_DECTREE_TRAIN,
        "%d leaves for %d el.s\n", nb_leaves(dtp), len_taskview(tvp) 
    );
}

void free_tree(DecTree* dtp)
{
    if (dtp->node_type == NT_PRED) {
        free_tree(dtp->left);
        free(dtp->left);
        free_tree(dtp->rght);
        free(dtp->rght); 
    }
    BARK(VERBOSE_DECTREE_MEM, "freed tree!\n");
}

NewDim best_new_dim(Tasks const* tasks, Trees const* trees, int pt_dim, float* score)
{
    float best_gain = -1000000.0;
    NewDim best_nd;

    for (char op=0; op!=4; ++op) {
        for (int didx_a=0; didx_a!=pt_dim; ++didx_a) {
            for (int didx_b=didx_a+1; didx_b!=pt_dim; ++didx_b) {

                NewDim nd = {didx_a, didx_b, op};
                float total_gain = 0.0;
                for (int i=0; i!=tasks->len; ++i) {
                    TaskView tv;
                    cons_taskview(&tv, &(tasks->data[i]));
                    float gain = gain_from_op(&tv, &(trees->data[i]), &nd);
                    if (0.0 < gain) {
                        total_gain += gain;
                    }
                    wipe_taskview(&tv);
                }
                if (total_gain <= best_gain) { continue; }

                best_gain = total_gain;
                best_nd = nd;

            }
        }
    }
    *score = best_gain;
    return best_nd; 
} 
