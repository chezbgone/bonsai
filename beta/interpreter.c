/*  author: samtenka
 *  change: 2020-05-14
 *  create: 2020-05-14
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#include <stdbool.h>
#include "lambda.h" 
#include "interpreter.h"
#include "enumerator.h"

/* assume for now that there are no abstractions */

ValGrid* impl_here   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_offset (ValGrid const* input, ValGrid const* args);
ValGrid* impl_east   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_north  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_south  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_west   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_plus   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_diff   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_negate (ValGrid const* input, ValGrid const* args);
ValGrid* impl_black  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_outside(ValGrid const* input, ValGrid const* args);
ValGrid* impl_view   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_blue   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_brown  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_gray   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_green  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_orange (ValGrid const* input, ValGrid const* args);
ValGrid* impl_purple (ValGrid const* input, ValGrid const* args);
ValGrid* impl_red    (ValGrid const* input, ValGrid const* args);
ValGrid* impl_teal   (ValGrid const* input, ValGrid const* args);
ValGrid* impl_yellow (ValGrid const* input, ValGrid const* args);
ValGrid* impl_has_hue(ValGrid const* input, ValGrid const* args);
ValGrid* impl_sees   (ValGrid const* input, ValGrid const* args);

#define IN_BOUNDS(r,c,H,W) ( 0 <= (r) && (r) < (H) && 0 <= (c) && (c) < (W) ) 

Routine implementations[] = {
    &impl_here   ,   &impl_offset ,   &impl_east   ,   &impl_north  ,  
    &impl_south  ,   &impl_west   ,   &impl_plus   ,   &impl_diff   ,  
    &impl_negate ,   &impl_black  ,   &impl_outside,   &impl_view   ,  
    &impl_blue   ,   &impl_brown  ,   &impl_gray   ,   &impl_green  ,  
    &impl_orange ,   &impl_purple ,   &impl_red    ,   &impl_teal   ,  
    &impl_yellow ,   &impl_has_hue,   &impl_sees   ,  
}; 

ValGrid* evaluate(LambExpr* e)
{
    ScoredLamb* args[MAX_NB_ARGS];
    int nb_args_seen;
    LambExpr* f; 
    while ( f->tag == EVAL ) {
        args[nb_args_seen] = f->arg;
        nb_args_seen += 1;
    }
    if ( f->tag != LEAF ) { fprintf(stderr, "UH OH!\n"); return NULL; }
    if ( nb_args_seen != nb_args[f->LID] ) { return NULL; }

    return implementations[f->LID](args); 
}

ValGrid* impl_sees(ValGrid const* input, ScoredLamb const* args) {
    ValGrid* pred  = args[0].val_grid; 
    ValGrid* dir   = args[1].val_grid; 
    ValGrid* start = args[2].val_grid; 

    int H = start->height; int W = start->width;

    ValGrid* out = make_grid(H, W, tTWO);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int dr = dir->grid  [r * W + c];
            int dc = dir->grid_b[r * W + c];
            int rr = start->grid  [r * W + c];
            int cc = start->grid_b[r * W + c];
            while ( IN_BOUNDS(rr,cc,W,H) && ! pred[rr * W + cc ] && (dr||dc) ) {
                rr += dr;
                cc += dc;
            }
            A->grid  [r * W + c] = rr;
            A->grid_b[r * W + c] = cc;
        }
    }
    return A;
}

//ValGrid* impl_negate(ValGrid const* input, ValGrid const* args) {
//    ValGrid* Z = &(args[0]); 
//    int H = Z->height; int W = Z->width;
//
//    ValGrid* A = make_grid(H, W, TDRCT);
//    for ( int r = 0; r != H; ++r ) { 
//        for ( int c = 0; c != W; ++c ) { 
//            A->grid  [r * W + c] = - Z->grid  [r * W + c];
//            A->grid_b[r * W + c] = - Z->grid_b[r * W + c];
//        }
//    }
//    return A;
//}
//
//ValGrid* impl_diff(ValGrid const* input, ValGrid const* args) {
//    ValGrid* Y = &(args[0]);  
//    ValGrid* Z = &(args[1]); 
//    int H = Z->height; int W = Z->width;
//
//    ValGrid* A = make_grid(H, W, TDRCT);
//    for ( int r = 0; r != H; ++r ) { 
//        for ( int c = 0; c != W; ++c ) { 
//            A->grid  [r * W + c] = Y->grid  [r * W + c] - Z->grid  [r * W + c];
//            A->grid_b[r * W + c] = Y->grid_b[r * W + c] - Z->grid_b[r * W + c];
//        }
//    }
//    return A;
//}
//
//ValGrid* impl_view(ValGrid const* input, ValGrid const* args) {
//    ValGrid* Z = &(args[0]); 
//    int H = Z->height; int W = Z->width;
//
//    ValGrid* A = make_grid(H, W, TCOLOR);
//    for ( int r = 0; r != H; ++r ) { 
//        for ( int c = 0; c != W; ++c ) { 
//            int rr = Z->grid  [r * W + c]; 
//            int cc = Z->grid_b[r * W + c]; 
//            A->grid  [r * W + c] = (
//                IN_BOUNDS(rr,cc,W,H) ? input[rr * W + cc] : OUTSIDE
//            );
//        }
//    }
//    return A;
//}
//
//ValGrid* impl_plus(ValGrid const* input, ValGrid const* args) {
//    ValGrid* Y = &(args[0]);  
//    ValGrid* Z = &(args[1]); 
//    int H = Z->height; int W = Z->width;
//
//    ValGrid* A = make_grid(H, W, TDRCT);
//    for ( int r = 0; r != H; ++r ) { 
//        for ( int c = 0; c != W; ++c ) { 
//            A->grid  [r * W + c] = Y->grid  [r * W + c] + Z->grid  [r * W + c];
//            A->grid_b[r * W + c] = Y->grid_b[r * W + c] + Z->grid_b[r * W + c];
//        }
//    }
//    return A;
//}
//
//ValGrid* impl_offset(ValGrid const* input, ValGrid const* args) {
//    ValGrid* D = &(args[0]);  
//    ValGrid* C = &(args[1]); 
//    int H = C->height; int W = C->width;
//
//    ValGrid* A = make_grid(H, W, TCELL);
//    for ( int r = 0; r != H; ++r ) { 
//        for ( int c = 0; c != W; ++c ) { 
//            A->grid  [r * W + c] = C->grid  [r * W + c] + D->grid  [r * W + c];
//            A->grid_b[r * W + c] = C->grid_b[r * W + c] + D->grid_b[r * W + c];
//        }
//    }
//    return A;
//}
//
