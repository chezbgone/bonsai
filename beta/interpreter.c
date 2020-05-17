/*  author: samtenka
 *  change: 2020-05-14
 *  create: 2020-05-14
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "concept_table.h"
#include "enumerator.h"
#include "interpreter.h"
#include "lambda.h" 

/* assume for now that there are no abstractions */

ValGrid* impl_here   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_offset (PossiblyEvaldExpr const* args, CTable const* ct);

ValGrid* impl_east   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_north  (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_south  (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_west   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_plus   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_diff   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_negate (PossiblyEvaldExpr const* args, CTable const* ct);

ValGrid* impl_abyss  (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_black  (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_cobalt (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_crimson(PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_forest (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_gold   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_lead   (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_magenta(PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_salmon (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_sky    (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_tan    (PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_view   (PossiblyEvaldExpr const* args, CTable const* ct);

ValGrid* impl_has_hue(PossiblyEvaldExpr const* args, CTable const* ct);
ValGrid* impl_sees   (PossiblyEvaldExpr const* args, CTable const* ct);

#define IN_BOUNDS(r,c,H,W) ( 0 <= (r) && (r) < (H) && 0 <= (c) && (c) < (W) ) 

Routine implementations[] = {
    &impl_here   ,   &impl_offset ,   &impl_east   ,   &impl_north  ,  
    &impl_south  ,   &impl_west   ,   &impl_plus   ,   &impl_diff   ,  
    &impl_negate ,   &impl_abyss  ,   &impl_black  ,   &impl_cobalt ,  
    &impl_crimson,   &impl_forest ,   &impl_gold   ,   &impl_lead   ,  
    &impl_magenta,   &impl_salmon ,   &impl_sky    ,   &impl_tan    ,  
    &impl_view   ,   &impl_has_hue,   &impl_sees   ,  
}; 

#define MAX_NB_ARGS 4
ValGrid* evaluate(LambExpr* e, CTable* ct)
{
    PossiblyEvaldExpr args[MAX_NB_ARGS];
    int nb_args_seen;
    LambExpr* f = e; 
    while ( f->tag == EVAL ) {
        args[nb_args_seen].expr = f->ARG;
        nb_args_seen += 1;
        f = f->FUN;
        /* TODO: address fact that arguments are backward!*/
    }
    if ( f->tag != LEAF ) { fprintf(stderr, "UH OH!\n"); return NULL; }
    //if ( nb_args_seen != nb_args[f->LID] ) { return NULL; }

    return implementations[f->LID](args, ct); 
}

ValGrid* make_grid(int H, int W, EType tag)
{
    ValGrid* new = malloc(sizeof(ValGrid)); 
    switch ( tag ) {
        case tCEL: new->grid = malloc(H*W); new->grid_b = malloc(H*W); break;
        case tDIR: new->grid = malloc(H*W); new->grid_b = malloc(H*W); break;
        case tHUE: new->grid = malloc(H*W);                            break;
        case tTWO: new->grid = malloc(H*W);                            break;
    } 
    return new;
}

ValGrid* impl_here   (PossiblyEvaldExpr const* args, CTable const* ct)
{
    ValGrid* input = args[0].grid;  
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tCEL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = r;
            out->grid_b[r*W + c] = c;
        }
    }
    return out;
}

ValGrid* impl_offset (PossiblyEvaldExpr const* args, CTable const* ct)
{
    ValGrid* input = args[0].grid;  
    ValGrid* dir   = args[1].grid;  
    ValGrid* cel   = args[2].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tCEL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = cel->grid  [r*W + c] + dir->grid  [r*W + c];
            out->grid_b[r*W + c] = cel->grid_b[r*W + c] + dir->grid_b[r*W + c];
        }
    }
    return out;
}

ValGrid* impl_plus   (PossiblyEvaldExpr const* args, CTable const* ct)
{
    ValGrid* input = args[0].grid;  
    ValGrid* da    = args[1].grid;  
    ValGrid* db    = args[2].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tDIR);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = da->grid  [r*W + c] + db->grid  [r*W + c];
            out->grid_b[r*W + c] = da->grid_b[r*W + c] + db->grid_b[r*W + c];
        }
    }
    return out;
}

ValGrid* impl_diff   (PossiblyEvaldExpr const* args, CTable const* ct)
{
    ValGrid* input = args[0].grid;  
    ValGrid* ca    = args[1].grid;  
    ValGrid* cb    = args[2].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tDIR);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = ca->grid  [r*W + c] - cb->grid  [r*W + c];
            out->grid_b[r*W + c] = ca->grid_b[r*W + c] - cb->grid_b[r*W + c];
        }
    }
    return out;
}

ValGrid* impl_negate (PossiblyEvaldExpr const* args, CTable const* ct)
{
    ValGrid* input = args[0].grid; 
    ValGrid* dir   = args[1].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tDIR);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = - dir->grid  [r*W + c];
            out->grid_b[r*W + c] = - dir->grid_b[r*W + c];
        }
    }
    return out;
}

ValGrid* impl_view   (PossiblyEvaldExpr const* args, CTable const* ct) {
    ValGrid* input = args[0].grid; 
    ValGrid* cel   = args[1].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tCEL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int rr = cel->grid  [r*W + c]; 
            int cc = cel->grid_b[r*W + c]; 
            out->grid  [r*W + c] = (
                IN_BOUNDS(rr,cc,W,H) ? input->grid[rr*W + cc] : -1 /*OUTSIDE*/
            );

        }
    }
    return out;
}

ValGrid* impl_has_hue(PossiblyEvaldExpr const* args, CTable const* ct) {
    ValGrid* input = args[0].grid; 
    ValGrid* col   = args[1].grid; 
    ValGrid* cel   = args[2].grid; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int rr = cel->grid  [r*W + c]; 
            int cc = cel->grid_b[r*W + c]; 
            out->grid  [r*W + c] = (
                IN_BOUNDS(rr,cc,W,H) ? input->grid[rr*W + cc] : -1 /*OUTSIDE*/
            ) == col->grid[r*W + c];

        }
    }
    return out;
}

ValGrid* impl_sees(PossiblyEvaldExpr const* args, CTable const* ct) {
    LambExpr* here = leaf_expr(0);

    ValGrid* input = args[0].grid; 
    ValGrid* pred  = search_table(ct, eval_expr(args[1].expr, here))->cargo;
    ValGrid* dir   = args[2].grid; 
    ValGrid* start = args[3].grid; 

    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int dr = dir->grid  [r*W + c];
            int dc = dir->grid_b[r*W + c];
            int rr = start->grid  [r*W + c];
            int cc = start->grid_b[r*W + c];
            do {
                rr += dr;
                cc += dc;
            } while ( IN_BOUNDS(rr,cc,W,H) && ! pred->grid[rr*W + cc] && (dr||dc) );
            out->grid[r*W + c] = IN_BOUNDS(rr,cc,W,H) && pred->grid[rr*W + cc];
        }
    }
    return out;
}





// constants

typedef PossiblyEvaldExpr const* ARG_A; 
typedef CTable const* ARG_B; 

ValGrid* const_dir(PossiblyEvaldExpr const* args, int dr, int dc)
{
    ValGrid* input = args[0].grid;  
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tDIR);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = dr;
            out->grid_b[r*W + c] = dc;
        }
    }
    return out;
}

ValGrid* impl_east   (ARG_A args, ARG_B ct) { return const_dir(args,  0, +1); } 
ValGrid* impl_north  (ARG_A args, ARG_B ct) { return const_dir(args, -1,  0); }
ValGrid* impl_south  (ARG_A args, ARG_B ct) { return const_dir(args, +1,  0); }
ValGrid* impl_west   (ARG_A args, ARG_B ct) { return const_dir(args,  0, -1); }

ValGrid* const_col(PossiblyEvaldExpr const* args, int col)
{
    ValGrid* input = args[0].grid;  
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tHUE);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = col;
        }
    }
    return out;
}

ValGrid* impl_abyss  (ARG_A args, ARG_B ct) { return const_col(args, -1); }
ValGrid* impl_black  (ARG_A args, ARG_B ct) { return const_col(args,  0); } 
ValGrid* impl_cobalt (ARG_A args, ARG_B ct) { return const_col(args,  1); }
ValGrid* impl_crimson(ARG_A args, ARG_B ct) { return const_col(args,  2); }
ValGrid* impl_forest (ARG_A args, ARG_B ct) { return const_col(args,  3); }
ValGrid* impl_gold   (ARG_A args, ARG_B ct) { return const_col(args,  4); }
ValGrid* impl_lead   (ARG_A args, ARG_B ct) { return const_col(args,  5); }
ValGrid* impl_magenta(ARG_A args, ARG_B ct) { return const_col(args,  6); }
ValGrid* impl_salmon (ARG_A args, ARG_B ct) { return const_col(args,  7); }
ValGrid* impl_sky    (ARG_A args, ARG_B ct) { return const_col(args,  8); }
ValGrid* impl_tan    (ARG_A args, ARG_B ct) { return const_col(args,  9); }
