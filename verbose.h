/* author: samtenka
 * change: 2020-03-06
 * create: 2020-03-06
 * descrp: 
 * to use: 
 */

#ifndef VERBOSE_H 
#define VERBOSE_H 

/*=============================================================================
======  0. VERBOSITY FLAGS  ===================================================
=============================================================================*/

#define VERBOSE_VECTOR_MEM      0 
#define VERBOSE_DECTREE_MEM     0 
#define VERBOSE_DECTREE_TRAIN   0

#define BARK(COND, ...) {if (COND) {printf(__VA_ARGS__);}}

char _c_;
#define WAIT_FOR_ENTER() {scanf("%c", &_c_);} 

#endif//VERBOSE_H
