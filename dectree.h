/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#ifndef DECTREE_H  
#define DECTREE_H  

#define VERBOSE_TREE 1

#define NT_PRED 1 
#define NT_LEAF 0 

#define NAY 0
#define YEA 1

#define OP_AND      0
#define OP_OR       1
#define OP_XOR      2
#define OP_IMPLIES  3

typedef struct DecTree DecTree; 

struct DecTree {
    DecTree* left; 
    DecTree* rght; 
    union {
        int didx;
        char value;
    } annotation;
    char node_type;
}; 

#include "dataset.h"
#ifdef DATASET_DONE

DecTree* train_tree(TaskView tv);
void free_tree(DecTree* dtp);
int nb_nodes(DecTree const* dtp);
int nb_leaves(DecTree const* dtp);
void print_tree(DecTree const* dtp);

float gain_from_op(TaskView tv, DecTree const* dtp, int didx_a, int didx_b, char op);

#endif//DATASET_DONE

#ifndef DECTREE_DONE 
#define DECTREE_DONE
#endif//DECTREE_DONE

#endif//DECTREE_H  
