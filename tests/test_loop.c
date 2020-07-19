/*  author: samtenka
 *  change: 2020-07-01
 *  create: 2020-06-30
 *  descrp: 
 *  to use: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "../containers/count_heap.h" 
#include "../containers/fixpoint.h" 
#include "../containers/mapping.h" 
#include "../data_scripts/toy_data.h"
#include "../dsl/arc_dsl.c"
#include "../dsl/enumerator.h"
#include "../dsl/interpreter.h"
#include "../lambda/concept_table.h"
#include "../lambda/extract.h"
#include "../lambda/lambda.h"
#include "../utils/colors.h"
#include "../utils/verbose.h"

void handle_interrupt(int sig);
void init_lamb_list(Grammar const* G, LambList* ll, float nats);
void init_valuations(CTable* ct, ValGrid const* input, LambList* ll, Grammar const* G);
NewDim nds[100];
LambExpr* expr_from_nd(int i, LambList const* ll); 
LambExpr* canonical_expr_from_nd(int i, LambList const* ll);
void print_tree_legend(DecTree const* dt, LambList const* ll);
void extract_concepts_from_tree(DecTree const* dt, CTable* ct, LambList const* ll);
LambExpr* extract_concepts_from_trees(Trees const* dts, LambList const* ll); 

void main()
{
    lava(); printf("hello!\n"); defc();

    init_lamb_expr_pool();
    signal(SIGINT, handle_interrupt);
    {
        Grammar G;              alloc_grammar(&G, NB_PRIMITIVES, NB_TYPES);
                                scores_from_counts(&G, my_prims, 0.0);

        add_concept(
            abst_expr(
                eval_expr(eval_expr(leaf_expr(24), 
                    eval_expr(vrbl_expr( 0),
                              eval_expr(eval_expr(leaf_expr( 1), leaf_expr( 0)), leaf_expr( 3)))),
                    eval_expr(vrbl_expr( 0),
                              eval_expr(eval_expr(leaf_expr( 1), leaf_expr( 0)), leaf_expr( 4)))
                )
            ),
            tTWO_TWOCEL,
            &G
        ); 

        LambList ll;            init_lamb_list(&G, &ll, 9.0);
        Tasks tasks;            init_tasks(&tasks, NB_EASY_TASKS);

        for ( int pi = 0; pi != ll.len; ++pi ) {
            printf("%4d : ", pi);
            lava(); printf("%8.4f ", ll.arr[pi].score); defc();
            print_expr(ll.arr[pi].e, my_leaf_names);
            printf("\n");
        }

        int const nb_dims = ll.len; 
        int const nb_colors = 10;

        for ( int tidx = 0; tidx != NB_EASY_TASKS; ++tidx ) {
            Task t = {make_charss(0), make_charss(0), nb_dims};
            push_tasks(&tasks, t);

            for ( int s = 0; s != easy_pairs[tidx].nb_pairs; ++s ) {
                CTable ct;
                init_table(&ct, CARGO_VALUED);
                ValGrid const* input = easy_pairs[tidx].pairs[s].x;
                init_valuations(&ct, input, &ll, &G);

                int H = input->height;
                int W = input->width;

                for ( int r = 0; r != H; ++r ) {
                    for ( int c = 0; c != W; ++c ) {
                        int actual_out = easy_pairs[tidx].pairs[s].y->grid[r*W + c];

                        for ( int di = 0; di != nb_dims; ++ di ) {
                            /* dim @ sample*/
                            ValGrid const* features = search_table(&ct, ll.arr[di].e)->cargo;
                            char f = features->grid[r*W + c]; 

                            char label = actual_out;
                            charss* points = label ? &(tasks.data[tidx].pospoints) :
                                                     &(tasks.data[tidx].negpoints)  ; 
                            if ( di == 0 ) {
                                push_charss(points, make_chars(nb_dims));
                            }
                            push_chars(&points->data[points->len-1], f);
                        }

                    }
                }

                wipe_table(&ct);
            }
        }


        for ( int it = 0; it != 0; ++it ) {
            printf("\n\n");
            lava();
            for ( int j = 0; j != 60; ++j ) { printf("=-"); }
            defc();
            printf("\n\n\n");

            Trees trees;            init_trees(&trees, 1);

            for ( int tidx = 0; tidx != NB_EASY_TASKS; ++tidx ) {
                DecTree dt;             init_tree(&dt);
                TaskView tv;            cons_taskview(&tv, &(tasks.data[tidx]));
                train_tree(&tv, &dt);
                push_trees(&trees, dt);
                // print:
                lime(); printf("%s\n", easy_pair_descrps[tidx]);
                defc();
                print_tree(&dt);
                print_tree_legend(&dt, &ll);
                printf("\n");
            }

            float score;
            NewDim nd = best_new_dim(&tasks, &trees, 10, nb_dims+it, &score); 
            nds[ll.len + it] = nd;
            add_new_dim(&tasks, &nd);

            lime(); printf("Found predicate number ");
            purp(); printf("%d ", nb_dims + it);
            lime(); printf("(score ");
            purp(); printf("%.1f", score);
            lime(); printf("): \n    ");
            print_expr(canonical_expr_from_nd(ll.len+it, &ll), my_leaf_names);
            defc(); printf("\n");
            
            LambExpr* best = extract_concepts_from_trees(&trees, &ll); 
            lime(); printf("Extracted a concept: \n    ");
            print_expr(best, my_leaf_names);
            printf("\n");

            free_trees(&trees);
        }

        free(ll.arr);
        free_grammar(&G);
    }
    handle_interrupt(0);
}

void handle_interrupt(int sig)
{
    free_lamb_expr_pool();
    lava(); printf("cleaned lambda pool!\n"); defc();
    exit(0);
}

void init_lamb_list(Grammar const* G, LambList* ll, float nats)
{
    clock_t start, end;
    float diff;

    start = clock();
    {
        *ll = enumerate(G, -nats, tTWO); 
    }
    end = clock();
    diff = ((float)(end-start))/CLOCKS_PER_SEC;

    lime(); printf("enumerated "); defc();
    lava(); printf("%d ", ll->len); defc();
    printf("programs in ");
    lava(); printf("%.3f ", 1000*diff); defc();
    printf("ms, or ");
    lava(); printf("%.3f ", 1000*diff/(ll->len)); defc();
    printf("ms per program \n");
}

void init_valuations(CTable* ct, ValGrid const* input, LambList* ll, Grammar const* G)
{
    clock_t start, end;
    float diff;

    start = clock();
    {
        init_table(ct, CARGO_VALUED);
        for ( int pi = 0; pi != ll->len; ++pi ) {
            ValGrid const* v = evaluate(input, ll->arr[pi].e, ct, G);
        }
    }
    end = clock();
    diff = ((float)(end-start))/CLOCKS_PER_SEC;

    lime(); printf("evaluated "); defc();
    lava(); printf("%d ", ll->len); defc();
    printf("programs in ");
    lava(); printf("%.3f ", 1000*diff); defc();
    printf("ms, or ");
    lava(); printf("%.3f ", 1000*diff/(ll->len)); defc();
    printf("ms per program \n");
}


LambExpr* expr_from_nd(int i, LambList const* ll) 
{
    if ( i < ll->len ) { return ll->arr[i].e; }

    switch ( nds[i].op ) {
        case OP_AND    : return eval_expr(eval_expr(leaf_expr(25),
                                expr_from_nd(nds[i].didx_a, ll)),
                                expr_from_nd(nds[i].didx_b, ll)); 
        case OP_OR     : return eval_expr(eval_expr(leaf_expr(24),
                                expr_from_nd(nds[i].didx_a, ll)),
                                expr_from_nd(nds[i].didx_b, ll)); 
        case OP_XOR    : return NULL; 
        case OP_IMPLIES: return NULL;
    }
}

LambExpr* canonical_expr_from_nd(int i, LambList const* ll)
{ 
    LambExpr* sortable = expr_from_nd(i, ll);
    if ( i < ll->len ) { return sortable; } 
    return canonicalize_logic(
        sortable,
        nds[i].op == OP_AND ? 25 :
        nds[i].op == OP_OR  ? 24 : -1
    ); 
}

void print_tree_legend(DecTree const* dt, LambList const* ll)
{
    switch ( dt->node_type ) {
        case NT_LEAF: return;
        case NT_PRED: printf("%d : ", dt->annotation.didx);
                      print_expr(canonical_expr_from_nd(dt->annotation.didx, ll), my_leaf_names);
                      printf("\n");
                      print_tree_legend(dt->left, ll);
                      print_tree_legend(dt->rght, ll);
                      return;
    }
}

void extract_concepts_from_tree(DecTree const* dt, CTable* ct, LambList const* ll)
{
    // TODO: weight by number of children
    switch ( dt->node_type ) {
        case NT_LEAF: return;
        case NT_PRED: extract_to(canonical_expr_from_nd(dt->annotation.didx, ll), ct);
                      extract_concepts_from_tree(dt->left, ct, ll);
                      extract_concepts_from_tree(dt->rght, ct, ll);
                      return;
    }
}

LambExpr* extract_concepts_from_trees(Trees const* dts, LambList const* ll) 
{
    CTable ct;
    init_table(&ct, COUNT_VALUED);
    DecTree* dt;
    for each (dt, *dts) {
        extract_concepts_from_tree(dt, &ct, ll);
    }
    //print_table(&ct, NULL);
    LambExpr* best = best_concept(&ct);
    wipe_table(&ct);
    return best;
}


