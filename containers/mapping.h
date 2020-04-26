/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include "pair.h"

#ifndef BASIC_MAPPING_H

#define ValType             int
#define KeyType             int 
#define key_compare(a,b)    ((a)<(b) ? -1 : (a)>(b) ? +1 : 0)
#define MapType             int_by_int
#define init_map            init_ibi
#define make_map            make_ibi
#define free_map            free_ibi
#define find_map            find_ibi
#define succ_map            succ_ibi
#define mini_map            mini_ibi
#define maxi_map            maxi_ibi
#define setv_map            setv_ibi
#define deln_map            deln_ibi
#define remk_map            remk_ibi
#define chgk_map            chgk_ibi
#include "mapping_generic.h"
#undef KeyType 
#undef ValType 
#undef key_compare
#undef MapType
#undef init_map
#undef make_map
#undef free_map
#undef find_map
#undef succ_map
#undef mini_map
#undef maxi_map
#undef setv_map
#undef deln_map
#undef remk_map
#undef chgk_map

#define ValType             pint
#define KeyType             int 
#define key_compare(a,b)    ((a)<(b) ? -1 : (a)>(b) ? +1 : 0)
#define MapType             pint_by_int
#define init_map            init_pibi
#define make_map            make_pibi
#define free_map            free_pibi
#define find_map            find_pibi
#define succ_map            succ_pibi
#define mini_map            mini_pibi
#define maxi_map            maxi_pibi
#define setv_map            setv_pibi
#define deln_map            deln_pibi
#define remk_map            remk_pibi
#define chgk_map            chgk_pibi
#include "mapping_generic.h"
#undef KeyType 
#undef ValType 
#undef key_compare
#undef MapType
#undef init_map
#undef make_map
#undef free_map
#undef find_map
#undef succ_map
#undef mini_map
#undef maxi_map
#undef setv_map
#undef deln_map
#undef remk_map
#undef chgk_map

#define ValType             int
#define KeyType             pint 
#define key_compare(a,b)                                \
    ((a.fst)<(b.fst) ? -1 : (a.fst)>(b.fst) ? +1 :      \
     (a.snd)<(b.snd) ? -1 : (a.snd)>(b.snd) ? +1 : 0)
#define MapType             int_by_pint
#define init_map            init_ibpi
#define make_map            make_ibpi
#define free_map            free_ibpi
#define find_map            find_ibpi
#define succ_map            succ_ibpi
#define mini_map            mini_ibpi
#define maxi_map            maxi_ibpi
#define setv_map            setv_ibpi
#define deln_map            deln_ibpi
#define remk_map            remk_ibpi
#define chgk_map            chgk_ibpi
#include "mapping_generic.h"
#undef KeyType 
#undef ValType 
#undef key_compare
#undef MapType
#undef init_map
#undef make_map
#undef free_map
#undef find_map
#undef succ_map
#undef mini_map
#undef maxi_map
#undef setv_map
#undef deln_map
#undef remk_map
#undef chgk_map

#define BASIC_MAPPING_H
#endif//BASIC_MAPPING_H
