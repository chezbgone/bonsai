/*  author: samtenka
 *  change: 2020-06-30
 *  create: 2020-05-14
 *  descrp: interpretation of abstract syntax trees
 *  to use: 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lambda/concept_table.h"
#include "../lambda/lambda.h" 
#include "../utils/colors.h"
#include "enumerator.h"
#include "interpreter.h"

void print_grid(ValGrid const* vg)
{
    int H = vg->height;
    int W = vg->width;
    switch ( vg->tag ) {
        case tTWO: 
            for ( int r = 0; r != H; ++r ) { 
                printf(">");
                for ( int c = 0; c != W; ++c ) { 
                    printf("%s", vg->grid[r*W + c] ? "[] " : "   ");
                }
                printf("<\n");
            } break;
        case tHUE: 
            for ( int r = 0; r != H; ++r ) { 
                printf(">");
                for ( int c = 0; c != W; ++c ) { 
                    printf("%3d", vg->grid[r*W + c]);
                }
                printf("<\n");
            } break;
        case tCEL:
            for ( int r = 0; r != H; ++r ) { 
                printf(">");
                for ( int c = 0; c != W; ++c ) { 
                    printf("%d%d ", vg->grid[r*W + c], vg->grid_b[r*W + c]);
                }
                printf("<\n");
            } break;
        case tDIR: printf("cannot print grids of dircts \n"); break;
    }

} 

/* assume for now that there are no abstractions */

ValGrid const* impl_here   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_offset (ValGrid const* input, LambExpr* const* args, CTable const* ct);

ValGrid const* impl_east   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_north  (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_south  (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_west   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_plus   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_diff   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_negate (ValGrid const* input, LambExpr* const* args, CTable const* ct);

ValGrid const* impl_view   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_abyss  (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_black  (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_cobalt (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_crimson(ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_forest (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_gold   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_lead   (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_magenta(ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_salmon (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_sky    (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_tan    (ValGrid const* input, LambExpr* const* args, CTable const* ct);

ValGrid const* impl_has_hue(ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_sees   (ValGrid const* input, LambExpr* const* args, CTable const* ct);

ValGrid const* impl_not    (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_or     (ValGrid const* input, LambExpr* const* args, CTable const* ct);
ValGrid const* impl_and    (ValGrid const* input, LambExpr* const* args, CTable const* ct);

#define IN_BOUNDS(r,c,H,W) ( 0<=(r) && (r)<(H) && 0<=(c) && (c)<(W) ) 

Routine implementations[] = {
    &impl_here   ,   &impl_offset ,   &impl_east   ,   &impl_north  ,  
    &impl_south  ,   &impl_west   ,   &impl_plus   ,   &impl_diff   ,  
    &impl_negate ,   &impl_view   ,   &impl_abyss  ,   &impl_black  ,
    &impl_cobalt ,   &impl_crimson,   &impl_forest ,   &impl_gold   ,   
    &impl_lead   ,   &impl_magenta,   &impl_salmon ,   &impl_sky    , 
    &impl_tan    ,   &impl_has_hue,   &impl_sees   ,   &impl_not    ,
    &impl_or     ,   &impl_and    , 
}; 

#define MAX_NB_ARGS 4
ValGrid const* evaluate(ValGrid const* input, LambExpr* e, CTable* ct, Grammar const* G)
{
    /* memoized */
    CTableValue const* val = search_table(ct, e);
    if ( val != NULL ) { return val->cargo; }

    /* evaluate kids */
    switch ( e-> tag ) {
        case LEAF: break; 
        case VRBL: break; 
        case ABST: evaluate(input, e->BOD, ct, G); break; 
        case EVAL: evaluate(input, e->FUN, ct, G);
                   evaluate(input, e->ARG, ct, G); break; 
    }

    /* collect arg values */
    LambExpr* args[MAX_NB_ARGS];
    int nb_args_seen = 0;
    LambExpr* f = e; 
    while ( f->tag == EVAL ) {
        args[nb_args_seen] = f->ARG;
        nb_args_seen += 1;
        f = f->FUN;
    }

    /* abort if not in expected form */
    if ( f->tag != LEAF ) {
        fprintf(stderr, "EVAL HEAD ISN'T LEAF!\n"); return NULL;
    } else if ( nb_args_seen != G->primitives.elts[f->LID].arity ) {
        fprintf(stderr, "WRONG NUMBER OF ARGS!\n"); return NULL;
    }

    /* address fact that arguments are backward */
    for ( int a = 0; a != nb_args_seen/2; ++a ) {
        LambExpr* temp = args[a];
        args[a] = args[nb_args_seen-1-a]; 
        args[nb_args_seen-1-a] = temp; 
    }

    /* run head function */
    ValGrid const* out = implementations[f->LID](input, args, ct);
    update_table(ct, e, (CTableValue){.cargo=out});
    return out; 
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
    new->height = H;
    new->width = W;
    new->tag = tag;
    return new;
}

ValGrid const* impl_here   (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
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

ValGrid const* impl_offset (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* dir   = search_table(ct, args[0])->cargo;  
    ValGrid const* cel   = search_table(ct, args[1])->cargo; 
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

ValGrid const* impl_plus   (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* da    = search_table(ct, args[0])->cargo;  
    ValGrid const* db    = search_table(ct, args[1])->cargo; 
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

ValGrid const* impl_diff   (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* ca    = search_table(ct, args[0])->cargo;  
    ValGrid const* cb    = search_table(ct, args[1])->cargo; 
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

ValGrid const* impl_negate (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* dir   = search_table(ct, args[0])->cargo; 
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

ValGrid const* impl_view   (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* cel   = search_table(ct, args[0])->cargo; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tHUE);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int rr = cel->grid  [r*W + c]; 
            int cc = cel->grid_b[r*W + c]; 
            out->grid  [r*W + c] = (
                IN_BOUNDS(rr,cc,H,W) ? input->grid[rr*W + cc] : -1 /*OUTSIDE*/
            );
        }
    }
    return out;
}

ValGrid const* impl_has_hue(ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* col   = search_table(ct, args[0])->cargo; 
    ValGrid const* cel   = search_table(ct, args[1])->cargo; 
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int rr = cel->grid  [r*W + c]; 
            int cc = cel->grid_b[r*W + c]; 
            out->grid  [r*W + c] = (
                IN_BOUNDS(rr,cc,H,W) ? input->grid[rr*W + cc] : -1 /*OUTSIDE*/
            ) == col->grid[r*W + c] ? 1 : 0;
        }
    }
    return out;
}

ValGrid const* impl_sees   (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    LambExpr* here = leaf_expr(0);

    ValGrid const* pred  = search_table(ct, eval_expr(args[0], here))->cargo;
    ValGrid const* dir   = search_table(ct, args[1])->cargo; 
    ValGrid const* start = search_table(ct, args[2])->cargo; 

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
            } while ( IN_BOUNDS(rr,cc,H,W) && ! pred->grid[rr*W + cc] && (dr||dc) );
            out->grid[r*W + c] = IN_BOUNDS(rr,cc,H,W) && pred->grid[rr*W + cc];
        }
    }
    return out;
}

ValGrid const* impl_not    (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* arg = search_table(ct, args[0])->cargo; 

    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid[r*W + c] = 0;//! arg->grid[r*W + c];
        }
    }
    return out;
}

ValGrid const* impl_or     (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* arg_a = search_table(ct, args[0])->cargo; 
    ValGrid const* arg_b = search_table(ct, args[1])->cargo; 

    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid[r*W + c] = 0;//arg_a->grid[r*W + c] ||
                                 //arg_b->grid[r*W + c];
        }
    }
    return out;
}

ValGrid const* impl_and    (ValGrid const* input, LambExpr* const* args, CTable const* ct)
{
    ValGrid const* arg_a = search_table(ct, args[0])->cargo; 
    ValGrid const* arg_b = search_table(ct, args[1])->cargo; 

    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid[r*W + c] = 0;//arg_a->grid[r*W + c] &&
                                 //arg_b->grid[r*W + c];
        }
    }
    return out;
}



// constants

typedef ValGrid const* VG; 
typedef LambExpr* const* LE; 
typedef CTable const* CT; 

ValGrid* const_dir(ValGrid const* input, LambExpr* const* args, int dr, int dc)
{
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

VG impl_east   (VG x, LE args, CT ct) { return const_dir(x, args,  0, +1); } 
VG impl_north  (VG x, LE args, CT ct) { return const_dir(x, args, -1,  0); }
VG impl_south  (VG x, LE args, CT ct) { return const_dir(x, args, +1,  0); }
VG impl_west   (VG x, LE args, CT ct) { return const_dir(x, args,  0, -1); }

ValGrid* const_col(ValGrid const* input, LambExpr* const* args, int col)
{
    int H = input->height; int W = input->width;

    ValGrid* out = make_grid(H, W, tHUE);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            out->grid  [r*W + c] = col;
        }
    }
    return out;
}

VG impl_abyss  (VG x, LE args, CT ct) { return const_col(x, args, -1); }
VG impl_black  (VG x, LE args, CT ct) { return const_col(x, args,  0); } 
VG impl_cobalt (VG x, LE args, CT ct) { return const_col(x, args,  1); }
VG impl_crimson(VG x, LE args, CT ct) { return const_col(x, args,  2); }
VG impl_forest (VG x, LE args, CT ct) { return const_col(x, args,  3); }
VG impl_gold   (VG x, LE args, CT ct) { return const_col(x, args,  4); }
VG impl_lead   (VG x, LE args, CT ct) { return const_col(x, args,  5); }
VG impl_magenta(VG x, LE args, CT ct) { return const_col(x, args,  6); }
VG impl_salmon (VG x, LE args, CT ct) { return const_col(x, args,  7); }
VG impl_sky    (VG x, LE args, CT ct) { return const_col(x, args,  8); }
VG impl_tan    (VG x, LE args, CT ct) { return const_col(x, args,  9); }
