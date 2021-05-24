//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_AVL_NODE_H
#define MESSAGEXCHANGER_AVL_NODE_H

typedef struct node_key_t node_key_t;
struct node_key_t {
    int value;
};

typedef struct node_t node_t;
struct node_t {
    int height;
    uint deleted : 1;
    node_key_t key;
    node_t * left, * right;

};

#endif //MESSAGEXCHANGER_AVL_NODE_H
