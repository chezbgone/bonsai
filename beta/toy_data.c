/*  author: samtenka
 *  change: 2020-05-17
 *  create: 2020-05-17
 *  descrp: toy ARC-style dataset capturing the notion of ``neighbor''
 *  to use: 
 */

#include <stdbool.h>

#include "interpreter.h" 
#include "toy_data.h" 
#include "type.h" 

#define SIDE 10
//ValGrid b0x, b0y, b1x, b1y;  
//ValGrid c0x, c0y, c1x, c1y;  

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
                        _,_,2,_,_,_,_,_,_,_, 
                        _,_,_,_,_,_,_,_,_,_, 
                        2,2,2,_,_,_,_,_,2,2, 
                        _,_,_,_,2,2,2,_,_,_, 
                        _,_,_,_,2,2,2,2,_,_, 
                        _,_,_,_,2,2,_,_,_,_, 
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

ArcPair a_pairs[A_NB_PAIRS] = {{.x=&a0x, .y=&a0y}, {.x=&a1x, .y=&a1y}};
//ArcPair b_pairs[A_NB_PAIRS] = {{.x=&b0x, .y=&b0y}, {.x=&b1x, .y=&b1y}};
//ArcPair c_pairs[A_NB_PAIRS] = {{.x=&c0x, .y=&c0y}, {.x=&c1x, .y=&c1y}};
