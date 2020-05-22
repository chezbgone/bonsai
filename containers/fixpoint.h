/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: Converges the set of user defined types to a fixpoint.  The idea of
 *          convergence is relevant because, for example:
 *
 *              "dataset.h" defines Task in terms of "vector.h"'s charss, but
 *              "vector.h" defines Tasks in terms of "dataset.h"'s Task.   
 *
 *          Hence, we guard Task and Tasks (and etc) with separate preprocessor
 *          conditionals, then include "vector.h" (for charss) before including
 *          "dataset.h" (for Task) before including "vector.h" (for Tasks).
 *          By these means, we keep conceptually similar types in the same file
 *          (e.g. charss and Tasks), even when those types are distant in the
 *          DAG of dependencies.
 *
 *  to use: If encounter linker errors, try appending another chunk to the
 *          end of this file:
 *
 *              #include "vector.h" 
 *              #include "mapping.h" 
 *              #include "count_heap.h" 
 *              #include "../dectree/dataset.h" 
 *              #include "../dectree/dectree.h" 
 */

#include "pair.h"

#include "vector.h" 
#include "mapping.h" 
#include "count_heap.h" 
#include "../dectree/dataset.h" 
#include "../dectree/dectree.h" 

#include "vector.h" 
#include "mapping.h" 
#include "count_heap.h" 
#include "../dectree/dataset.h" 
#include "../dectree/dectree.h" 

#include "vector.h" 
#include "mapping.h" 
#include "count_heap.h" 
#include "../dectree/dataset.h" 
#include "../dectree/dectree.h" 

#include "vector.h" 
#include "mapping.h" 
#include "count_heap.h" 
#include "../dectree/dataset.h" 
#include "../dectree/dectree.h" 

#include "vector.h" 
#include "mapping.h" 
#include "count_heap.h" 
#include "../dectree/dataset.h" 
#include "../dectree/dectree.h" 
