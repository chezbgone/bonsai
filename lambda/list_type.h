/*
 */

#ifndef LIST_TYPE_H
#define LIST_TYPE_H

struct Type {
    union {
        int num;
        struct {
            Type* t; // element types
        } list;
        struct {
            Type* d; // domain
            Type* c; // codomain
        } func;
    } data;
    enum {
        INT = 0,
        LIST = 1,
        FUNC = 2,
    } tag;
};

#endif // LIST_TYPE_H
