/*  author: samtenka
 *  change: 2020-05-14
 *  create: 2020-05-02 
 *  descrp: dsl type system
 *  to use: 
 */

#ifndef TYPE_H 
#define TYPE_H 

// TODO: replace by commented struct below
//
#define NB_TYPES 19

typedef enum EType EType;
enum EType {
    tCEL                =  0,  // CEL 
    tCEL_CEL            =  1,  // CEL <- CEL
    tCEL_CEL_DIR        =  2,  // (CEL <- CEL) <- DIR
    tCEL_CEL_DIR_TWOCEL =  3,  // (CEL <- CEL) <- DIR <- (TWO <- CEL)

    tDIR                =  4,
    tDIR_CEL            =  5,
    tDIR_CEL_CEL        =  6,
    tDIR_DIR            =  7,

    tDIR_DIR_DIR        =  8,
    tHUE                =  9,
    tHUE_CEL            = 10,
    tTWO                = 11,

    tTWOCEL             = 12,
    tTWOCEL_DIR         = 13,
    tTWOCEL_DIR_TWOCEL  = 14,
    tTWOCEL_HUE         = 15,

    tTWOTWO             = 16,
    tTWOTWO_TWO         = 17,
    tTWO_TWOCEL         = 18,
};                       

extern bool is_func[NB_TYPES];   
extern EType arg_type[NB_TYPES];
extern EType out_type[NB_TYPES];

///*=============================================================================
//======  0. TYPES  =============================================================
//=============================================================================*/
//
//typedef struct Type Type;
//struct Type {
//    union {
//        enum {
//            BOOL = 0,
//            NUMB = 1,
//        } base;
//        struct {
//            Type* out;
//            Type* arg;
//        } from; 
//        struct {
//            Type* elt;
//        } mset; 
//    } data;
//    enum {
//        BASE = 0,
//        FROM = 1,
//        MSET = 2,
//    } tag;
//};  
//
//extern Type t_bool;
//extern Type t_numb;
//extern Type t_bool_pred;
//extern Type t_numb_pred;
//extern Type t_bool_size;
//extern Type t_numb_size;
//extern Type t_bools;
//extern Type t_numbs;   

#endif//TYPE_H
