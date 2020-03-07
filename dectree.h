/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-04
 * descrp: 
 * to use: 
 */

#ifndef DECTREE_H  
#define DECTREE_H  

#include "vector.h"
#include "dataset.h"

#define VERBOSE_TREE 1

#define NT_PRED 1 
#define NT_LEAF 0 

#define NAY 0
#define YEA 1

typedef struct DecTree DecTree; 

struct DecTree {
    DecTree* left; 
    DecTree* rght; 
    char node_type;
    union {
        char didx;
        char value;
    } annotation;
}; 

DecTree* train_tree(TaskView tv);
void free_tree(DecTree* dtp);
int nb_nodes(DecTree const* dtp);
int nb_leaves(DecTree const* dtp);
void print_tree(DecTree const* dtp);

#endif//DECTREE_H  
