/*  author: samtenka
 *  change: 2020-05-06
 *  create: 2020-05-06
 *  descrp: implement hash table storing (lambda:score) records
 *  to use:  
 */

#include <stdlib.h>

#include "lambda.h"
#include "concept_table.h"

/*===========================================================================*/
/*====  0. DECLARE LIST METHODS (PRIVATE TO THIS FILE)  =====================*/
/*===========================================================================*/

void init_list(CList* cl);
void wipe_list(CList* cl); /* shallow */
void free_list(CList* cl); /* deep */
CRecord* insert_into_list(CList* cl, LambExpr* bod);
CRecord* find_in_list(CList* cl, LambExpr* bod);

/*===========================================================================*/
/*====  1. TABLE METHODS  ===================================================*/
/*===========================================================================*/

void insert_into_table(CTable* ct, LambExpr* bod);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.0. Constructor and Destructor  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void init_table(CTable* ct)
{
    ct->nb_bins = 256;
    ct->arr = malloc(sizeof(CList) * ct->nb_bins);
}

void free_table(CTable* ct)
{
    for ( int i=0; i != ct->nb_bins; ++i ) { free_list(&(ct->arr[i])); }
    free(ct->arr);
    *ct = (CTable){.arr = NULL, .nb_bins = 0, .nb_elts = 0};
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.1. Extract  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

LambExpr* best_concept(CTable* ct)
{
    CRecord best = {.bod=NULL, .score=-1};
    for ( int i=0; i != ct->nb_bins; ++i ) {
        CList cl = ct->arr[i];
        for ( int j=0; j != cl.len; ++j ) {
            CRecord* cr = &(cl.arr[i]);
            if ( cr->score <= best.score ) { continue; }
            best = *cr;
        }
    }
    return best.bod;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  1.2. Update  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void expand_table(CTable* ct);

/*----------------  1.2.0. update  ------------------------------------------*/

void update_table(CTable* ct, LambExpr* bod, int d_score)
{
    expand_table(ct);

    CList* cl = &(ct->arr[ bod->hash % ct->nb_bins ]);
    CRecord* cr = find_in_list(cl, bod); 
    if ( cr == NULL ) { cr = insert_into_list(cl, bod); }
    cr->score += d_score;
}

/*----------------  1.2.0. private insertion helper  ------------------------*/

void expand_table(CTable* ct)
{
    if ( ! ( 3 * ct->nb_elts < ct->nb_bins ) ) { return; }

    int new_nb_bins = 3*ct->nb_bins + 1;
    CList* new_arr = malloc(sizeof(CList) * new_nb_bins);
    for ( int i=0; i != new_nb_bins; ++i ) {
        init_list(&(new_arr[i]));
    }
    for ( int i=0; i != ct->nb_bins; ++i ) {
        CList* cl = &(ct->arr[i]);
        for ( int j=0; j != cl->len; ++j ) {
            CRecord* cr = &(cl->arr[j]); 
            CList* new_cl = &(new_arr[ cr->bod->hash % new_nb_bins ]);
            CRecord* new_cr = find_in_list(new_cl, cr->bod); 
            if ( new_cr == NULL ) {
                new_cr = insert_into_list(new_cl, cr->bod);
            }
            new_cr->score = cr->score;
        }
        wipe_list(cl);
    }
    free(ct->arr);
    *ct = (CTable){.arr = new_arr, .nb_bins = new_nb_bins};
}

/*===========================================================================*/
/*====  2. IMPLEMENT LIST METHODS  ==========================================*/
/*===========================================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~  2.0. Search  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

CRecord* find_in_list(CList* cl, LambExpr* bod)
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

CRecord* insert_into_list(CList* cl, LambExpr* bod)
{
    while ( ! ( cl->len < cl->cap ) ) {
        int new_cap = (3*cl->cap)/2 + 1;
        CRecord* new_arr = malloc(sizeof(CRecord) * new_cap);
        for ( int i=0; i != cl->len; ++i ) { new_arr[i] = cl->arr[i]; }
        free(cl->arr); cl->arr = new_arr; 
        cl->cap = new_cap;
    } 
    cl->arr[cl->len] = (CRecord){.bod = bod, .score=0};
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

void free_list(CList* cl)
{
    for ( int i=0; i != cl->len; ++i ) { free_expr(cl->arr[i].bod); }
    wipe_list(cl);
}


