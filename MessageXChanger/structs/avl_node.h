#ifndef MESSAGEXCHANGER_AVL_NODE_H
#define MESSAGEXCHANGER_AVL_NODE_H

#include "user.h"

typedef struct node_t node_t;

typedef struct node_t {
    int height;
    uint deleted : 1;
    user_t * user;
    node_t * left, * right;

} node_t;

#endif //MESSAGEXCHANGER_AVL_NODE_H
