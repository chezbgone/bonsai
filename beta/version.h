/*  author: samtenka
 *  change: 2020-04-28
 *  create: 2020-04-28
 *  descrp: dsl version spaces
 *  to use: 
 */

#ifndef VERSION_H 
#define VERSION_H 

#include "lambda.h"

/*=============================================================================
======  2. VERSION SPACES  ====================================================
=============================================================================*/

typedef struct VersionSpace VersionSpace;
struct VersionSpace {
    union {
        int leaf_idx; 
        int vrbl_idx;
        struct {
            VersionSpace* body;
        } abst; 
        struct {
            VersionSpace* fun;
            VersionSpace* arg;
        } eval; 
        /**/
        struct {
            int cap;
            int len;
            VersionSpace** elts; 
        } disj;
    } data;
    enum {
        VS_LEAF = 0, 
        VS_VRBL = 1, 
        VS_ABST = 2, 
        VS_EVAL = 3, 
        /**/
        VS_DISJ = 4, 
    } tag;
}; 

VersionSpace* rewrite(LambdaExpr* e);
void print_vs(VersionSpace* vs, char leaf_names[][16]);
void free_vs(VersionSpace* vs);

#endif//VERSION_H
