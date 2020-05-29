/*  author: samtenka
 *  change: 2020-05-27
 *  create: 2020-05-17
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
#include "../dsl/enumerator.h"
#include "../dsl/interpreter.h"
#include "../utils/colors.h"
#include "../utils/verbose.h"
#include "concept_table.h"
#include "lambda.h"
#include "extract.h"

const float exp_thouth = 1.001000500;
const float exp_hundth = 1.010050167;
const float exp_tenth  = 1.105170918;
const float exp_one    = 2.718281828;

float plog(float p)
{
    if ( p == 0.0 ) { return -9999.0; }
    float bulk = 0.0;
    while ( p < 1.0 ) { p *= exp_one   ; bulk -= 1.   ; }
    while ( 1.0 < p ) { p /= exp_tenth ; bulk +=  .1  ; }
    while ( p < 1.0 ) { p *= exp_hundth; bulk -=  .01 ; } 
    while ( 1.0 < p ) { p /= exp_thouth; bulk +=  .001; }

    float d = 1.0-p; 
    float remainder = d*(1/1.0 + d*(1/2.0 + d/3.0));
    return bulk - remainder; 
}

typedef struct Primitive Primitive;
struct Primitive {
    char name[16];
    int nb_args;
    EType type;
    float weight;
    bool is_const;
    bool needs_nonconst;
    bool commutes;
    bool needs_unequal;
    bool absorbs_self;
};

const float ABST_PROB = 0.0;

#define NB_PRIMITIVES 26//23//24
Primitive my_prims[] = {                        /*const  nnonc  comm   uneq   absbs*/
    {"here"    , 0, tCEL               ,      64 , false, 1    , 0    , 0    , 0    }, 
    {"offset"  , 2, tCEL_CEL_DIR       ,   16    , 1    , 1    , 0    , 0    , true }, 
                    
    {"east"    , 0, tDIR               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"north"   , 0, tDIR               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"south"   , 0, tDIR               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"west"    , 0, tDIR               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"plus"    , 2, tDIR_DIR_DIR       ,      64 , 1    , false, true , 0    , 0    },
    {"diff"    , 2, tDIR_CEL_CEL       , 4       , 1    , 1    , 0    , true , 0    },
    {"negate"  , 1, tDIR_DIR           , 4       , 1    , 1    , 0    , 0    , 0/**/},
                    
    //{"query"   , 0, tHUE               ,      64 , false, 1    , 0    , 0    , 0    },
    {"view"    , 1, tHUE_CEL           ,      64 , 1    , 1    , 0    , 0    , 0    }, 
    {"abyss"   , 0, tHUE               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"black"   , 0, tHUE               ,      64 , 1    , 1    , 0    , 0    , 0    },
    {"cobalt"  , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"crimson" , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"forest"  , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"gold"    , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"lead"    , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"magenta" , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"salmon"  , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"sky"     , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
    {"tan"     , 0, tHUE               , 4       , 1    , 1    , 0    , 0    , 0    },
                    
    {"has_hue" , 2, tTWOCEL_HUE        ,      64 , 1    , 1    , 0    , 0    , 0    }, 
    {"sees"    , 3, tTWOCEL_DIR_TWOCEL , 4       , 1    , 1    , 0    , 0    , 0    }, 

    {"not"     , 1, tTWOTWO            ,       1 , 1    , 1    , 0    , 0    , 0    }, 
    {"or"      , 2, tTWOTWO_TWO        ,       1 , 1    , 1    , 1    , 1    , 0    }, 
    {"and"     , 2, tTWOTWO_TWO        ,       1 , 1    , 1    , 1    , 1    , 0    }, 
};
float eval_scores[NB_TYPES];
float leaf_scores[NB_PRIMITIVES];
EType leaf_types[NB_PRIMITIVES];

char leaf_names[NB_PRIMITIVES][16];
int  nb_args[NB_PRIMITIVES];
bool is_const[NB_PRIMITIVES];
bool needs_nonconst[NB_PRIMITIVES];
bool commutes[NB_PRIMITIVES];
bool needs_unequal[NB_PRIMITIVES];
bool absorbs_self[NB_PRIMITIVES];

void initialize_primitive_scores()
{
    float weight_to[NB_TYPES];

    fprintf(stderr, "hi!\n");
    for ( int t = 0; t != NB_TYPES; ++t ) { weight_to[t] = 0.0; }

    fprintf(stderr, "hi!\n");
    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive p = my_prims[l];
        EType t = p.type; 
        while ( t != -1 ) { 
            weight_to[t] += p.weight;
            t = out_type[t];
        }; 
    }

    fprintf(stderr, "hi!\n");
    for ( int l = 0; l != NB_PRIMITIVES; ++l ) {
        Primitive* p = &(my_prims[l]);
        EType t = p->type;
        leaf_types[l] = t;
        leaf_scores[l] = plog( p->weight / weight_to[t] );

        strcpy(leaf_names[l], p->name); 
        nb_args[l] = p->nb_args;
        is_const[l] = p->is_const;
        needs_nonconst[l] = p->needs_nonconst;
        commutes[l] = p->commutes;
        needs_unequal[l] = p->needs_unequal;
        absorbs_self[l] = p->absorbs_self;
    }

    fprintf(stderr, "hi!\n");
    for ( int t = 0; t != NB_TYPES; ++t) {
        eval_scores[t] = plog(
            /*  ( ( t == TTWOCEL ) ? (1-ABST_PROB) : 1.0 ) *   */
            ( is_func[t] ? ( weight_to[t] / weight_to[out_type[t]]) : 0.0 )
        );
    }
}

void handle_interrupt(int sig)
{
    free_lamb_expr_pool();
    lava(); printf("cleaned lambda pool!\n"); defc();
    exit(0);
}

void init_grammar(Grammar* G)
{
    *G = (Grammar){
        .nb_leaves      = NB_PRIMITIVES   ,
        .nb_args        = nb_args         ,
        .leaf_scores    = leaf_scores     ,
        .leaf_types     = leaf_types      ,
        .eval_score     = eval_scores     ,
        .is_const       = is_const        ,
        .needs_nonconst = needs_nonconst  ,
        .commutes       = commutes        ,
        .needs_unequal  = needs_unequal   ,
        .absorbs_self   = absorbs_self    ,
        .abst_score     = plog(ABST_PROB) ,
    };
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

void init_valuations(CTable* ct, ValGrid const* input, LambList* ll)
{
    clock_t start, end;
    float diff;

    start = clock();
    {
        init_table(ct, CARGO_VALUED);
        for ( int pi = 0; pi != ll->len; ++pi ) {
            ValGrid const* v = evaluate(input, ll->arr[pi].e, ct, nb_args);
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

NewDim nds[100];

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
                      print_expr(canonical_expr_from_nd(dt->annotation.didx, ll), leaf_names);
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

void main()
{
    initialize_primitive_scores();

    lava(); printf("hello!\n"); defc();

    init_lamb_expr_pool();
    signal(SIGINT, handle_interrupt);
    {
        Grammar G;              init_grammar(&G);
        LambList ll;            init_lamb_list(&G, &ll, 8.0);
        Tasks tasks;            init_tasks(&tasks, NB_EASY_TASKS);

        for ( int pi = 0; pi != ll.len; ++pi ) {
            printf("%4d : ", pi);
            lava(); printf("%8.4f ", ll.arr[pi].score); defc();
            print_expr(ll.arr[pi].e, leaf_names);
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
                init_valuations(&ct, input, &ll);

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


        for ( int it = 0; it != 12; ++it ) {
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
            print_expr(canonical_expr_from_nd(ll.len+it, &ll), leaf_names);
            defc(); printf("\n");
            
            LambExpr* best = extract_concepts_from_trees(&trees, &ll); 
            lime(); printf("Extracted a concept: \n    ");
            print_expr(best, leaf_names);
            printf("\n");

            free_trees(&trees);
        }

        free(ll.arr);
    }
    handle_interrupt(0);
}




