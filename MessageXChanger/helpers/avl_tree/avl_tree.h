//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_AVL_TREE_H
#define MESSAGEXCHANGER_AVL_TREE_H

#include "../../structs/avl_node.h"

#define true 1
#define false 0

#define HIDE_DELETED 0
#define SHOW_DELETED 1
#define BUFFER_SIZE 64

extern node_t * insert(node_t * node, node_key_t key);
extern int delete(node_t * source, int value);
extern void print_tree(FILE * destination, node_t *node, int mode);
extern node_t * find_node(node_t * node, int value, int mode);

#endif //MESSAGEXCHANGER_AVL_TREE_H
