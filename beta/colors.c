/*  author: samtenka
 *  change: 2020-04-28
 *  create: 2020-04-28
 *  descrp: dsl type system
 *  to use: 
 */

#ifndef COLORS_H 
#define COLORS_H 

#include <stdio.h>

/*=============================================================================
======  0. COLORS  ============================================================
=============================================================================*/

void skyb() { printf("\033[36m"); }
void crim() { printf("\033[31m"); }
void lime() { printf("\033[32m"); }
void defc() { skyb();             }

#endif//COLORS_H
