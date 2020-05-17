/*  author: samtenka
 *  change: 2020-05-17
 *  create: 2020-05-06
 *  descrp: Implement hash tables storing (lambda:value) records.
 *  to use:  
 */

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "concept_table.h"
#include "lambda.h"

/*  On /MOD/: for positive /b/, /a % b/ can be negative but our /MOD(a, b)/
    will always be non-negative.  Thus, we use /MOD/ for hashing.
*/
#define MOD(a, b) ( (((a)%(b))+(b)) % (b) ) 

#define START_NB_BINS 256

/*===========================================================================*/
/*====  0. DECLARE LIST METHODS (PRIVATE TO THIS FILE)  =====================*/
/*===========================================================================*/

void init_list(CList* cl);
void wipe_list(CList* cl); /* shallow */
CRecord* insert_into_list(CList* cl, LambExpr const* bod, CTableType tag);
CRecord* find_in_list(CList const* cl, LambExpr const* bod);

/*===========================================================================*/
/*====  1. TABLE METHODS  ===================================================*/
/*===========================================================================*/

void insert_into_table(CTable* ct, LambExpr* bod);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.0. Constructor and Destructor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void init_table(CTable* ct, CTableType tag)
{
    ct->nb_bins = START_NB_BINS;
    ct->nb_elts = 0;
    ct->arr = malloc(sizeof(CList) * ct->nb_bins);
    ct->tag = tag; 
    for ( int i=0; i != ct->nb_bins; ++i ) { init_list(&(ct->arr[i])); }
}

void wipe_table(CTable* ct)
{
    for ( int i=0; i != ct->nb_bins; ++i ) { wipe_list(&(ct->arr[i])); }
    free(ct->arr);
    *ct = (CTable){.arr = NULL, .nb_bins = 0, .nb_elts = 0};
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.1. Query  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*----------------  1.1.0. extract program with highest count  --------------*/

LambExpr const* best_concept(CTable const* ct)
{
    CRecord best = {.bod=NULL, .data={.count=-1}};
    for ( int i=0; i != ct->nb_bins; ++i ) {
        CList cl = ct->arr[i];
        for ( int j=0; j != cl.len; ++j ) {
            CRecord* cr = &(cl.arr[j]);
            if ( cr->COUNT <= best.COUNT ) { continue; }
            best = *cr;
        }
    }
    return best.bod;
}

CTableValue const* search_table(CTable const* ct, LambExpr const* bod)
{
    CList* cl = &(ct->arr[ MOD(bod->hash, ct->nb_bins) ]);
    CRecord* cr = find_in_list(cl, bod); 
    if ( cr == NULL ) { return NULL; }

    return &(cr->data);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.2. Update  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void expand_table(CTable* ct);

/*----------------  1.2.0. update  ------------------------------------------*/

void update_table(CTable* ct, LambExpr const* bod, CTableValue diff)
{
    expand_table(ct);

    CList* cl = &(ct->arr[ MOD(bod->hash, ct->nb_bins) ]);

    CRecord* cr = find_in_list(cl, bod); 
    if ( cr == NULL ) { /* not found */
        cr = insert_into_list(cl, bod, ct->tag);
        ct->nb_elts += 1;
    }

    switch ( ct->tag ) {
        case COUNT_VALUED:  cr->COUNT += diff.count;    break;
        case CARGO_VALUED:  cr->CARGO = diff.cargo;     break;
    }
}

/*----------------  1.2.0. private insertion helper  ------------------------*/

void expand_table(CTable* ct)
{
    if ( 3 * ct->nb_elts < ct->nb_bins ) { return; }

    int new_nb_bins = 3*ct->nb_bins + 1;

    lava(); printf("expand: "); defc();
    lime(); printf("%3d ", ct->nb_bins); defc();
    printf("bins -> ");
    lime(); printf("%3d ", new_nb_bins); defc();
    printf("bins...\n");

    CList* new_arr = malloc(sizeof(CList) * new_nb_bins);
    for ( int i=0; i != new_nb_bins; ++i ) {
        init_list(&(new_arr[i]));
    }
    for ( int i=0; i != ct->nb_bins; ++i ) {
        CList* cl = &(ct->arr[i]);
        for ( int j=0; j != cl->len; ++j ) {
            CRecord* cr = &(cl->arr[j]); 
            CList* new_cl = &(new_arr[ MOD(cr->bod->hash, new_nb_bins) ]);
            CRecord* new_cr = insert_into_list(new_cl, cr->bod, ct->tag);
            new_cr->data = cr->data;
        }
        wipe_list(cl);
    }
    free(ct->arr);
    ct->arr = new_arr;
    ct->nb_bins = new_nb_bins;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.3. Display  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void print_table(CTable const* ct, char leaf_names[][16])
{
    for ( int i=0; i != ct->nb_bins; ++i ) {
        CList cl = ct->arr[i];
        for ( int j=0; j != cl.len; ++j ) {
            CRecord* cr = &(cl.arr[j]);
            if ( 0 < cr->COUNT ) { lava(); } 
            printf("%+3d", cr->COUNT);
            defc();
            printf(" : ");
            print_expr(cr->bod, leaf_names);
            printf("\n");
        }
    }
}

/*===========================================================================*/
/*====  2. IMPLEMENT LIST METHODS  ==========================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.0. Search  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

CRecord* find_in_list(CList const* cl, LambExpr const* bod)
{
    for ( int i=0; i != cl->len; ++i ) {
        if ( same_expr(bod, cl->arr[i].bod) ) { return &(cl->arr[i]); }
    }
    return NULL;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.1. Construction  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void init_list(CList* cl)
{
    *cl = (CList){.arr=NULL, .cap=0, .len=0};
}

CRecord* insert_into_list(CList* cl, LambExpr const* bod, CTableType tag)
{
    while ( ! ( cl->len < cl->cap ) ) {
        int new_cap = (3*cl->cap)/2 + 1;
        CRecord* new_arr = malloc(sizeof(CRecord) * new_cap);
        for ( int i=0; i != cl->len; ++i ) { new_arr[i] = cl->arr[i]; }
        free(cl->arr); cl->arr = new_arr; 
        cl->cap = new_cap;
    } 
    cl->arr[cl->len].bod = bod;
    switch ( tag ) {
        case CARGO_VALUED:   cl->arr[cl->len].CARGO = NULL;             break;
        case COUNT_VALUED:   cl->arr[cl->len].COUNT = -(1+bod->weight); break;
    } 
    cl->len += 1; 
    return &(cl->arr[cl->len-1]); 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.2. Destruction  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void wipe_list(CList* cl)
{
    free(cl->arr);
    *cl = (CList){.arr = NULL, .cap=0, .len=0};
}
