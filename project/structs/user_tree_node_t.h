#ifndef MESSAGEXCHANGER_USER_TREE_NODE_T_H
#define MESSAGEXCHANGER_USER_TREE_NODE_T_H

#include "user.h"

typedef struct user_tree_node_t user_tree_node_t;

typedef struct user_tree_node_t {
    user_t * user;
    user_tree_node_t * left, * right;
} user_tree_node_t;

#endif //MESSAGEXCHANGER_USER_TREE_NODE_T_H
