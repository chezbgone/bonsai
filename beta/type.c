/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-04
 *  descrp: 
 *  to use: 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "type.h"
#include "lambda.h"

Type t_bool          = {.tag=BASE, .data={.base=BOOL                        }};
Type t_numb          = {.tag=BASE, .data={.base=NUMB                        }};
Type t_bool_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_bool}}};
Type t_numb_pred     = {.tag=FROM, .data={.from={.out=&t_bool, .arg=&t_numb}}};
Type t_bool_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_bool}}};
Type t_numb_size     = {.tag=FROM, .data={.from={.out=&t_numb, .arg=&t_numb}}};

Type t_bools         = {.tag=MSET, .data={.mset={.elt=&t_bool              }}};
Type t_numbs         = {.tag=MSET, .data={.mset={.elt=&t_numb              }}};

