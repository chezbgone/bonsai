/*  author: samtenka
 *  change: 2020-03-06
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#ifndef TREE  

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

typedef struct DimPair DimPair; 
struct DimPair {
    int didx_a;  
    int didx_b;  
}; 

typedef struct NewDim NewDim; 
struct NewDim {
    int didx_a;  
    int didx_b;  
    char op;
}; 

#define TREE
#endif//TREE

#ifndef TREE_METHODS
#ifdef TREE
#ifdef TASK

float info_of(TaskView const* tvp);
void init_tree(DecTree* dtp);
void train_tree(TaskView const* tvp, DecTree* dtp);
void free_tree(DecTree* dtp);
int nb_nodes(DecTree const* dtp);
int nb_leaves(DecTree const* dtp);
void print_tree(DecTree const* dtp);

#define TREE_METHODS
#endif//TASK
#endif//TREE
#endif//TREE_METHODS

#ifndef ADVANCED_TREE_METHODS
#ifdef TREES
#ifdef TASKS

void most_popular_pairs(Trees const* tsp, DimPairs* psp, int max_len, int pt_dim);
float gain_from_op(TaskView const* tvp, DecTree const* dtp, NewDim const* new_dim);
NewDim best_new_dim(Tasks const* tasks, Trees const* trees, int max_len, int pt_dim, float* score); 

#define ADVANCED_TREE_METHODS
#endif//TASKS
#endif//TREES
#endif//ADVANCED_TREE_METHODS


