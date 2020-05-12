/*  author: samtenka
 *  change: 2020-05-10
 *  create: 2020-03-04
 *  descrp: interface for decision trees 
 *  to use: 
 */

#ifndef TREE  

#define NAY 0
#define YEA 1

typedef struct DecTree DecTree; 
struct DecTree {
    DecTree* left; 
    DecTree* rght; 
    union {
        int pred_idx;
        char value;
    } label;
    enum {
        NT_LEAF = 0,
        NT_PRED = 1 
    } node_type;
}; 

float info_of(TaskView const* tv);

void init_tree(DecTree* dt);
void train_tree(TaskView const* tv, DecTree* dt);
void free_tree(DecTree* dt);

int nb_nodes(DecTree const* dt);
int nb_leaves(DecTree const* dt);
void print_tree(DecTree const* dt);

#endif//TREE
