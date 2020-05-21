/*  author: samtenka
 *  change: 2020-05-18
 *  create: 2020-05-17
 *  descrp: Toy ARC-style dataset capturing the notion of ``neighbor'':
 *            (a) picks black cells that neighbor non-black cells 
 *            (b) picks non-black cells as well as their neighbors
 *            (c) picks cells that do not neighbor black cells 
 *            (d) picks magenta cells that neighbor gold cells 
 *            (e) picks non-black cells' neighbors not neighboring green cells
 *            (f) picks non-black cells that neighbor cells of the same color
 *
 *            (g) picks neighbors of neighbors of non-black cells            
 *            (h) picks neighbors of {non-black cells right below black cells}
 *            (i) picks  
 *            (j) picks 
 *            (k) picks 
 *            (l) picks 
 *  to use: 
 */

#include <stdbool.h>

#include "interpreter.h" 
#include "toy_data.h" 
#include "type.h" 

#define SIDE 10
#define _ 0

char a0xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,2,_,_, 
                        _,2,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,2,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char a0yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,1,_,_, 
                        _,1,_,_,_,_,1,_,1,_, 
                        1,_,1,_,_,_,_,1,_,_, 
                        _,1,_,1,_,_,_,_,_,_, 
                        _,_,1,_,1,_,_,_,_,_, 
                        _,_,_,1,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char a1xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,4,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        4,4,4,_,_,_,_,_,3,3, 
                        _,_,_,_,3,3,3,_,_,_, 
                        _,_,_,_,3,3,3,3,_,_, 
                        _,_,_,_,3,3,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char a1yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,1,_,_,_,_,_,_,_, 
                        _,1,_,1,_,_,_,_,_,_, 
                        1,1,1,_,_,_,_,_,1,1, 
                        _,_,_,1,1,1,1,1,_,_, 
                        1,1,1,1,_,_,_,1,1,1, 
                        _,_,_,1,_,_,_,_,1,_, 
                        _,_,_,1,_,_,1,1,_,_, 
                        _,_,_,_,1,1,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

ValGrid a0x = {.grid=a0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid a0y = {.grid=a0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid a1x = {.grid=a1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid a1y = {.grid=a1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

char b0xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,6,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,4,_,_,_,_,_,_,_, 
                        _,4,4,4,4,_,_,_,_,_, 
                        _,4,4,4,4,_,_,_,_,_, 
                        _,_,4,4,4,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char b0yg[SIDE*SIDE] = {_,_,_,_,_,_,1,1,1,_, 
                        _,_,_,_,_,_,1,1,1,_, 
                        _,_,_,_,_,_,1,1,1,_, 
                        _,1,1,1,_,_,_,_,_,_, 
                        1,1,1,1,1,1,_,_,_,_, 
                        1,1,1,1,1,1,_,_,_,_, 
                        1,1,1,1,1,1,_,_,_,_, 
                        1,1,1,1,1,1,_,_,_,_, 
                        _,1,1,1,1,1,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char b1xg[SIDE*SIDE] = {_,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        5,5,5,5,5,5,5,5,5,5, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_, 
                        _,_,_,5,5,_,_,_,_,_,}; 

char b1yg[SIDE*SIDE] = {_,_,1,1,1,1,_,_,_,_,  
                        1,1,1,1,1,1,1,1,1,1,  
                        1,1,1,1,1,1,1,1,1,1,  
                        1,1,1,1,1,1,1,1,1,1,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,}; 

ValGrid b0x = {.grid=b0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid b0y = {.grid=b0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid b1x = {.grid=b1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid b1y = {.grid=b1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

char c0xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,6,6,6,_,_,_,_, 
                        _,_,6,6,6,6,6,_,_,_, 
                        _,_,6,6,6,6,6,_,_,_, 
                        _,_,_,6,6,6,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,6,6,_,_, 
                        _,_,_,_,_,6,6,6,_,_, 
                        _,_,_,_,_,6,_,6,_,_, 
                        _,_,_,_,_,_,6,_,_,_,}; 

char c0yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,1,1,1,_,_,_,_,  
                        _,_,_,1,1,1,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

char c1xg[SIDE*SIDE] = {_,_,_,_,_,7,7,_,_,_, 
                        _,_,_,7,7,7,7,7,_,_, 
                        _,_,7,7,7,7,7,7,7,_, 
                        _,7,7,7,_,_,_,7,7,7, 
                        _,7,7,7,_,_,7,7,7,_, 
                        _,7,7,7,_,_,_,7,7,7, 
                        _,7,7,7,_,_,_,7,7,7, 
                        _,_,7,7,7,_,7,7,7,_, 
                        _,_,7,7,7,7,7,7,_,_, 
                        _,_,_,7,7,7,7,_,_,_,}; 

char c1yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,1,1,_,_,_,  
                        _,_,_,1,_,_,_,1,_,_,  
                        _,_,1,_,_,_,_,_,1,_,  
                        _,_,1,_,_,_,_,1,_,_,  
                        _,_,1,_,_,_,_,_,1,_,  
                        _,_,1,_,_,_,_,_,1,_,  
                        _,_,_,1,_,_,_,1,_,_,  
                        _,_,_,1,1,_,1,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

ValGrid c0x = {.grid=c0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid c0y = {.grid=c0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid c1x = {.grid=c1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid c1y = {.grid=c1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

char d0xg[SIDE*SIDE] = {_,_,_,2,_,_,_,_,_,_, 
                        _,_,_,_,_,_,6,4,6,6, 
                        _,_,6,4,8,_,6,6,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,6,6,6,6,4,2,_,8,_, 
                        _,6,_,_,_,6,_,_,_,_, 
                        _,6,4,_,_,_,6,_,_,_, 
                        _,_,6,_,8,_,_,4,_,_, 
                        _,_,_,_,2,_,8,4,2,_, 
                        _,_,_,_,_,_,_,4,_,_,}; 

char d0yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,1,_,1,_,  
                        _,_,1,_,_,_,_,1,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,1,_,_,_,_,_,  
                        _,_,_,_,_,1,_,_,_,_,  
                        _,1,_,_,_,_,_,_,_,_,  
                        _,_,1,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

char d1xg[SIDE*SIDE] = {_,_,_,_,_,_,_,4,_,4, 
                        6,1,6,1,6,1,6,1,4,_, 
                        _,1,1,1,1,1,1,4,1,4, 
                        _,6,1,4,1,6,1,6,1,6, 
                        _,1,4,4,4,1,1,1,1,_, 
                        6,1,6,4,6,1,4,1,6,_, 
                        _,1,1,1,1,1,4,4,1,_, 
                        _,6,1,6,1,6,1,4,4,6, 
                        _,1,1,1,1,1,1,1,4,_, 
                        6,_,6,_,6,_,6,_,6,_,}; 

char d1yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,1,_,1,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,1,_,1,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,1,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,1,_,}; 

ValGrid d0x = {.grid=d0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid d0y = {.grid=d0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid d1x = {.grid=d1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid d1y = {.grid=d1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

char e0xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,6,_,_,_,_,_,_, 
                        _,7,_,_,_,5,_,_,_,_, 
                        _,_,9,8,_,_,4,_,_,_, 
                        _,_,_,1,7,6,_,9,4,_, 
                        _,8,_,_,_,_,3,_,_,_, 
                        _,_,_,_,2,5,_,5,_,3, 
                        _,1,_,_,_,4,_,_,_,8, 
                        _,_,_,9,3,_,1,_,7,_, 
                        _,_,2,_,_,_,6,_,2,_,}; 

char e0yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,1,1,_,_,_,_,_,_,  
                        _,_,_,1,1,1,_,1,1,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,1,1,_,_,_,1,  
                        _,_,_,_,_,1,_,_,_,_,  
                        _,_,_,_,1,_,1,_,1,_,  
                        _,_,_,_,_,_,1,_,1,_,}; 

char e1xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,7,_,_,_,_,_,_,_,8, 
                        _,_,_,2,2,3,3,3,_,_, 
                        _,_,_,2,2,3,3,3,_,_, 
                        _,1,1,2,2,_,_,4,4,_, 
                        _,1,1,_,_,_,_,4,4,_, 
                        _,1,1,6,6,_,_,4,4,_, 
                        _,_,_,6,6,5,5,5,_,_, 
                        _,_,_,6,6,5,5,5,_,_, 
                        _,_,_,_,_,_,_,_,_,9,}; 

char e1yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,2,_,_,_,_,_,_,  
                        _,_,_,2,_,_,_,_,_,_,  
                        _,1,1,2,2,_,_,_,4,_,  
                        _,1,1,_,_,_,_,4,4,_,  
                        _,1,1,6,6,_,_,4,4,_,  
                        _,_,_,6,6,5,5,5,_,_,  
                        _,_,_,6,6,5,5,5,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

ValGrid e0x = {.grid=e0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid e0y = {.grid=e0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid e1x = {.grid=e1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid e1y = {.grid=e1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

char f0xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,4,_,_,_,_,_, 
                        _,4,_,4,6,4,_,_,_,_, 
                        _,_,6,6,6,_,_,_,_,_, 
                        _,1,_,4,_,4,_,_,_,_, 
                        1,_,1,_,_,_,3,_,_,_, 
                        _,1,_,_,_,3,6,3,_,_, 
                        _,_,_,_,4,4,3,3,_,_, 
                        _,_,_,4,3,4,_,_,_,_, 
                        _,_,_,_,4,_,_,_,_,_,}; 

char f0yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,1,_,_,_,_,_,  
                        _,_,1,1,1,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,1,_,_,  
                        _,_,_,_,1,1,1,1,_,_,  
                        _,_,_,_,_,1,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

char f1xg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        _,4,6,6,4,4,6,4,_,_, 
                        _,6,3,2,3,2,2,6,_,_, 
                        _,6,1,1,3,1,3,4,_,_, 
                        _,4,3,2,3,2,2,6,_,_, 
                        _,6,1,1,3,1,3,6,_,_, 
                        _,4,6,4,4,6,6,4,_,_, 
                        _,_,_,_,_,_,_,_,_,_,}; 

char f1yg[SIDE*SIDE] = {_,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,  
                        _,_,1,1,1,1,_,_,_,_,  
                        _,1,_,_,1,1,1,_,_,_,  
                        _,1,1,1,1,_,_,_,_,_,  
                        _,_,_,_,1,1,1,1,_,_,  
                        _,_,1,1,1,_,_,1,_,_,  
                        _,_,_,1,1,1,1,_,_,_,  
                        _,_,_,_,_,_,_,_,_,_,}; 

ValGrid f0x = {.grid=f0xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid f0y = {.grid=f0yg, .height=SIDE, .width=SIDE, .tag=tTWO};
ValGrid f1x = {.grid=f1xg, .height=SIDE, .width=SIDE, .tag=tHUE};
ValGrid f1y = {.grid=f1yg, .height=SIDE, .width=SIDE, .tag=tTWO};  

ArcPair a_pairs[A_NB_PAIRS] = {{.x=&a0x, .y=&a0y}, {.x=&a1x, .y=&a1y}};
ArcPair b_pairs[B_NB_PAIRS] = {{.x=&b0x, .y=&b0y}, {.x=&b1x, .y=&b1y}};
ArcPair c_pairs[C_NB_PAIRS] = {{.x=&c0x, .y=&c0y}, {.x=&c1x, .y=&c1y}};
ArcPair d_pairs[D_NB_PAIRS] = {{.x=&d0x, .y=&d0y}, {.x=&d1x, .y=&d1y}};
ArcPair e_pairs[D_NB_PAIRS] = {{.x=&e0x, .y=&e0y}, {.x=&e1x, .y=&e1y}};
ArcPair f_pairs[D_NB_PAIRS] = {{.x=&f0x, .y=&f0y}, {.x=&f1x, .y=&f1y}};
