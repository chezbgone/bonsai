/*  author: samtenka
 *  change: 2020-04-25
 *  create: 2020-03-08
 *  descrp: associative map from an ordered type of keys to a type of values
 *  to use:
 */

#ifdef ValType 
#ifdef KeyType 
#ifdef key_compare

#ifdef MapType

#ifdef init_map 
#ifdef make_map
#ifdef free_map 

#ifdef find_map
#ifdef succ_map
#ifdef mini_map
#ifdef maxi_map

#ifdef setv_map
#ifdef deln_map 
#ifdef remk_map 
#ifdef chgk_map

typedef struct MapType MapType;

struct MapType {
    MapType* left; 
    MapType* rght;  
    MapType* prnt;  
    KeyType key;
    ValType value;
    char is_leaf;
};

void init_map(MapType* mp);
MapType* make_map(MapType* prnt);
void free_map(MapType* mp);

MapType* find_map(MapType const* mp, KeyType key);
MapType* succ_map(MapType const* mp);
MapType* mini_map(MapType const* mp);
MapType* maxi_map(MapType const* mp);

/* modify the tree structure but not the root */
void setv_map(MapType* mp, KeyType key, ValType value);

/* modify the tree structure and potentially the root */
void deln_map(MapType** mpp, MapType* node);
void remk_map(MapType** mpp, KeyType key);
void chgk_map(MapType** mpp, KeyType old_key, KeyType new_key);

#endif
#endif
#endif
#endif

#endif
#endif
#endif
#endif

#endif
#endif
#endif

#endif

#endif
#endif
#endif
