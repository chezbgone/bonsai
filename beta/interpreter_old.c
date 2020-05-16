/*  author: samtenka
 *  change: 2020-05-12
 *  create: 2020-05-12
 *  descrp: interface for interpretation of abstract syntax trees
 *  to use: 
 */

#include <stdbool.h>
#include "lambda.h" 
#include "interpreter.h"
#include "enumerator.h"

/* assume for now that there are no abstractions */

//ValGrid* impl_not   (ValGrid const* input, ValGrid const* args);
//ValGrid* impl_and   (ValGrid const* input, ValGrid const* args);
//ValGrid* impl_or    (ValGrid const* input, ValGrid const* args);
//ValGrid* impl_eq    (ValGrid const* input, ValGrid const* args);
//ValGrid* impl_scan  (ValGrid const* input, ValGrid const* args);
//
ValGrid* impl_sees  (ValGrid const* input, ValGrid const* args);

ValGrid* impl_offset(ValGrid const* input, ValGrid const* args);

ValGrid* impl_view  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_diff  (ValGrid const* input, ValGrid const* args);
ValGrid* impl_negate(ValGrid const* input, ValGrid const* args);
ValGrid* impl_plus  (ValGrid const* input, ValGrid const* args);

#define IN_BOUNDS(r,c,H,W) ( 0 <= (r) && (r) < (H) && 0 <= (c) && (c) < (W) ) 

Routine implementations[] = {
    &impl_not,     
    &impl_and,     
    &impl_or,      
    &impl_eq,//eq_dir
    &impl_eq,//eq_col
    &impl_eq,//eq_cell
    /**/
    NULL,//base,    
    &impl_offset,  
    &impl_scan,    
    &impl_view,    
    /**/
    NULL,//outside, 
    NULL,//black,   
    NULL,//gray,    
    NULL,//blue,    
    NULL,//brown,   
    NULL,//green,   
    NULL,//orange,  
    NULL,//purple,  
    NULL,//red,     
    NULL,//teal,    
    NULL,//yellow,  
    /**/
    &impl_diff,    
    &impl_negate,  
    &impl_plus,    
    NULL,//east,    
    NULL,//north,   
    NULL,//west,    
    NULL,//south,   
    NULL,//n_east,  
    NULL,//n_west,  
    NULL,//s_west,  
    NULL,//s_east,  
}; 

ValGrid* evaluate(LambExpr* e)
{
    ValGrid* args[MAX_NB_ARGS];
    int nb_args_seen;
    LambExpr* f; 
    while ( f->tag == EVAL ) {
        args[nb_args_seen] = GET_VAL(f->arg);
        nb_args_seen += 1;
    }
    if ( f->tag != LEAF ) { fprintf(stderr, "UH OH!\n"); return NULL; }
    if ( nb_args_seen != nb_args[f->LID] ) { return NULL; }

    return implementations[f->LID](args); 
}

ValGrid* impl_scan(ValGrid const* input, ValGrid const* args) {
    ValGrid* D = &(args[0]); 
    ValGrid* B = &(args[1]); 
    ValGrid* C = &(args[2]); 
    int H = C->height; int W = C->width;

    ValGrid* A = make_grid(H, W, TCELL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int dr = D->grid  [r * W + c];
            int dc = D->grid_b[r * W + c];
            int rr = C->grid  [r * W + c];
            int cc = C->grid_b[r * W + c];
            while ( IN_BOUNDS(rr,cc,W,H) && ! B[rr * W + cc ] && (dr||dc) ) {
                rr += dr;
                cc += dc;
            }
            A->grid  [r * W + c] = rr;
            A->grid_b[r * W + c] = cc;
        }
    }
    return A;
}

ValGrid* impl_negate(ValGrid const* input, ValGrid const* args) {
    ValGrid* Z = &(args[0]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TDRCT);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid  [r * W + c] = - Z->grid  [r * W + c];
            A->grid_b[r * W + c] = - Z->grid_b[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_diff(ValGrid const* input, ValGrid const* args) {
    ValGrid* Y = &(args[0]);  
    ValGrid* Z = &(args[1]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TDRCT);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid  [r * W + c] = Y->grid  [r * W + c] - Z->grid  [r * W + c];
            A->grid_b[r * W + c] = Y->grid_b[r * W + c] - Z->grid_b[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_view(ValGrid const* input, ValGrid const* args) {
    ValGrid* Z = &(args[0]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TCOLOR);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            int rr = Z->grid  [r * W + c]; 
            int cc = Z->grid_b[r * W + c]; 
            A->grid  [r * W + c] = (
                IN_BOUNDS(rr,cc,W,H) ? input[rr * W + cc] : OUTSIDE
            );
        }
    }
    return A;
}

ValGrid* impl_plus(ValGrid const* input, ValGrid const* args) {
    ValGrid* Y = &(args[0]);  
    ValGrid* Z = &(args[1]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TDRCT);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid  [r * W + c] = Y->grid  [r * W + c] + Z->grid  [r * W + c];
            A->grid_b[r * W + c] = Y->grid_b[r * W + c] + Z->grid_b[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_offset(ValGrid const* input, ValGrid const* args) {
    ValGrid* D = &(args[0]);  
    ValGrid* C = &(args[1]); 
    int H = C->height; int W = C->width;

    ValGrid* A = make_grid(H, W, TCELL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid  [r * W + c] = C->grid  [r * W + c] + D->grid  [r * W + c];
            A->grid_b[r * W + c] = C->grid_b[r * W + c] + D->grid_b[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_eq(ValGrid const* input, ValGrid const* args) {
    ValGrid* Y = &(args[0]); 
    ValGrid* Z = &(args[1]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TBOOL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid[r * W + c] = (Y->grid[r * W + c] == Z->grid[r * W + c]);
        }
    }
    return A;
}

ValGrid* impl_and(ValGrid const* input, ValGrid const* args) {
    ValGrid* Y = &(args[0]); 
    ValGrid* Z = &(args[1]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TBOOL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid[r * W + c] = Y->grid[r * W + c] && Z->grid[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_or(ValGrid const* input, ValGrid const* args) {
    ValGrid* Y = &(args[0]); 
    ValGrid* Z = &(args[1]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TBOOL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid[r * W + c] = Y->grid[r * W + c] || Z->grid[r * W + c];
        }
    }
    return A;
}

ValGrid* impl_not(ValGrid const* input, ValGrid const* args) {
    ValGrid* Z = &(args[0]); 
    int H = Z->height; int W = Z->width;

    ValGrid* A = make_grid(H, W, TBOOL);
    for ( int r = 0; r != H; ++r ) { 
        for ( int c = 0; c != W; ++c ) { 
            A->grid[r * W + c] = ! Z->grid[r * W + c]
        }
    }
    return A;
}

