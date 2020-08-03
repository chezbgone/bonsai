/*  author: samtenka
 *  change: 2020-05-14
 *  create: 2020-05-02
 *  descrp: 
 *  to use: 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/colors.h"
#include "lambda.h"
#include "arc_type.h"

bool is_func[NB_TYPES] = {
    false           , 1             , 1             , 1             ,
    false           , 1             , 1             , 1             ,
    1               , false         , 1             , false         ,
    1               , 1             , 1             , 1             ,
    1               , 1             , 1             , 
};

EType arg_type[NB_TYPES] = { 
    -1              , tCEL          , tDIR          , tTWOCEL       ,
    -1              , tCEL          , tCEL          , tDIR          ,
    tDIR            , -1            , tCEL          , -1            ,
    tCEL            , tDIR          , tTWOCEL       , tHUE          ,
    tTWO            , tTWO          , tTWOCEL
};

EType out_type[NB_TYPES] = { 
    -1              , tCEL          , tCEL_CEL      , tCEL_CEL_DIR  ,
    -1              , tDIR          , tDIR_CEL      , tDIR          ,
    tDIR_DIR        , -1            , tHUE          , -1            ,
    tTWO            , tTWOCEL       , tTWOCEL_DIR   , tTWOCEL       ,
    tTWO            , tTWOTWO       , tTWO
};

//Type t_bool          = {.tag=BASE, .data={.base=BOOL                        }};
//Type t_numb          = {.tag=BASE, .data={.base=NUMB                        }};
//Type t_bool_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_bool}}};
//Type t_numb_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_numb}}};
//Type t_bool_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_bool}}};
//Type t_numb_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_numb}}};
//
//Type t_bools         = {.tag=MSET, .data={.mset={.elt=&t_bool              }}};
//Type t_numbs         = {.tag=MSET, .data={.mset={.elt=&t_numb              }}};

