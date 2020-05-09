/*  author: samtenka
 *  change: 2020-05-08
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
/*====  0. TREE STRUCTURE  ==================================================*/
/*===========================================================================*/

#define DEPTH 3 

void extract_to(LambExpr const* e, CTable* ct);
LambExpr const* rewrite_given(LambExpr const* e, LambExpr const* concept);

#endif//EXTRACT_H
