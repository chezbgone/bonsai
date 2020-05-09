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

void pink() { printf("\033[38;5;212m"); }
void lava() { printf("\033[38;5;202m"); }
void gold() { printf("\033[38;5;220m"); }
void lime() { printf("\033[38;5;34m"); }
void teal() { printf("\033[38;5;44m"); }
void purp() { printf("\033[38;5;91m"); }

void defc() { teal();             }

#endif//COLORS_H
