/*  author: samtenka
 *  change: 2020-05-28
 *  create: 2020-05-08
 *  descrp: interface for abstract syntax trees
 *  to use: 
 */

#ifndef EXTRACT_H 
#define EXTRACT_H 

#include <stdbool.h>

#include "lambda.h"
#include "concept_table.h"

/*===========================================================================*/
/*====  0. LAMBDA EXTRACTION  ===============================================*/
/*===========================================================================*/

#define DEPTH 3 

void extract_to(LambExpr* e, CTable* ct);
LambExpr* rewrite_given(LambExpr* e, LambExpr* concept, LambExpr* name);

/*===========================================================================*/
/*====  1. LOGIC CANONICAL FORM  ============================================*/
/*===========================================================================*/

LambExpr* canonicalize_logic(LambExpr* e, int logic_op);  

#endif//EXTRACT_H
