/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>

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

void init_map(MapType* mp)
{
    mp->is_leaf = 1;
    mp->prnt = NULL;
}

MapType* make_map(MapType* prnt)
{
    MapType* mp = malloc(sizeof(MapType)); 
    mp->is_leaf = 1;
    mp->prnt = prnt;
    return mp;
}

MapType* find_map(MapType const* mp, KeyType key)
{
    /* assume (mp!=NULL) */
    while ( ! mp->is_leaf ) {
        switch ( key_compare(key, mp->key) ) {
            case -1: mp = mp->left; break;
            case +1: mp = mp->rght; break;
            case  0: return (MapType*) mp; 
        }
    }
    return (MapType*) mp;
}

MapType* succ_map(MapType const* mp)
{
    /* assume (mp!=NULL && !mp->is_leaf && !mp->rght->is_leaf) */
    return mini_map(mp->rght);
}

MapType* mini_map(MapType const* mp)
{
    /* assume (mp!=NULL && !mp->is_leaf ) */
    while ( !mp->left->is_leaf ) { mp = mp->left; }
    return (MapType*) mp;
}

MapType* maxi_map(MapType const* mp)
{
    /* assume (mp!=NULL && !mp->is_leaf ) */
    while ( !mp->rght->is_leaf ) { mp = mp->rght; }
    return (MapType*) mp;
}

void setv_map(MapType* mp, KeyType key, ValType value)
{
    mp = find_map(mp, key); 
    if ( mp->is_leaf ) {
        mp->left = make_map(mp);
        mp->rght = make_map(mp);
        mp->key = key; 
    }
    mp->value = value; 
}

void free_map(MapType* mp)
{
    if ( !mp->is_leaf ) {
        free_map(mp->left);
        free_map(mp->rght);
    }
    free(mp->left);
    free(mp->rght);
}

void deln_map(MapType* mp, MapType* node)
{
    // PRE: (found!=NULL && !found->is_leaf)
    char has_left = !node->left->is_leaf;
    char has_rght = !node->rght->is_leaf;
    MapType* newn;

    if ( !has_left && !has_rght ) {
        newn = node->rght; 
        free(node->left);
    } else if ( !has_left &&  has_rght ) {
        newn = node->rght;
    } else if (  has_left && !has_rght ) {
        newn = node->left;
    } else {
        newn = succ_map(node);
        // POST: assume (newn!=NULL && !newn->is_leaf ) */

        newn->rght->prnt = newn->prnt;
        newn->prnt->left = newn->rght;
    }

    if ( node->prnt != NULL ) {
        if ( node->prnt->left == node ) { node->prnt->left = newn; }
        else                              { node->prnt->rght = newn; }
    } else {
        /* root changed! */
        *mp = *newn;
    }

    if ( newn != NULL ) {
        newn->prnt = node->prnt;
        newn->rght = node->rght;
        newn->left = node->left;
    }
    free(node);
}

void remk_map(MapType* mp, KeyType key)
{
    MapType* found = find_map(mp, key); 
    deln_map(mp, found);
}

void chgk_map(MapType* mp, KeyType old_key, KeyType new_key)
{
    MapType* found = find_map(mp, old_key); 
    ValType value = found->value;
    deln_map(mp, found);
    setv_map(mp, new_key, value);
}

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
