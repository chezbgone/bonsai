/*  author: samtenka
 *  change: 2020-03-08
 *  create: 2020-03-08
 *  descrp: 
 *  to use: 
 */

#include <stdlib.h>
#include "verbose.h"

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

void free_map(MapType* mp)
{
    if ( mp->is_leaf ) { return; }
    free_map(mp->left);
    free_map(mp->rght);
    free(mp->left);
    free(mp->rght);
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
        mp->is_leaf = 0;
    }
    mp->value = value; 
}

void deln_map(MapType** mpp, MapType* node)
{
    // { found!=NULL && !found->is_leaf }
    char has_left = !node->left->is_leaf;
    char has_rght = !node->rght->is_leaf;
    MapType* newn;

    // attach to node's children 
    if ( !has_left && !has_rght ) {
        newn = node->rght; 
        free(node->left);
    } else if ( !has_left &&  has_rght ) {
        newn = node->rght;
        free(node->left);
    } else if (  has_left && !has_rght ) {
        newn = node->left;
        free(node->rght);
    } else {
        newn = succ_map(node);
        // { newn!=NULL && !newn->is_leaf && newn->prnt!=NULL }
        free(newn->left);
        if ( newn != node->rght ) {
            // between node and newn is a distinct node newn->prnt
            newn->prnt->left = newn->rght; newn->rght->prnt = newn->prnt;
            newn->left = node->left; newn->left->prnt = newn;
            newn->rght = node->rght; newn->rght->prnt = newn;
        } else {
            newn->left = node->left; newn->left->prnt = newn;
        }
    }

    // attach to node's parent
    newn->prnt = node->prnt;
    if ( node->prnt != NULL ) {
        if ( node->prnt->left == node ) { node->prnt->left = newn; }
        else                            { node->prnt->rght = newn; }
        free(node);
    } else {
        /* root changed! */
        free(node);
        *mpp = newn;
    }
}

void remk_map(MapType** mpp, KeyType key)
{
    MapType* found = find_map(*mpp, key); 
    deln_map(mpp, found);
}

void chgk_map(MapType** mpp, KeyType old_key, KeyType new_key)
{
    MapType* found = find_map(*mpp, old_key); 
    ValType value = found->value;
    deln_map(mpp, found);
    setv_map(*mpp, new_key, value);
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
