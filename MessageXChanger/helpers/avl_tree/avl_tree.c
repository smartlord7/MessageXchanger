//
// Created by joao on 24/05/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "../../structs/avl_node.h"
#include "avl_tree.h"

node_t * root;

node_t * left_rotation(node_t * node);
node_t * right_rotation(node_t * node);
int max(int num1, int num2);
int calculate_height(node_t * node);
int calculate_balance(node_t * node);

int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}

node_t * insert(node_t * node, node_key_t key){
    int balancing_factor;

    if(node == NULL){

        node = (node_t *) malloc(sizeof(node_t));
        node->key = key;
        node->deleted = false;

        return node;
    } else {

        if(key.value > node->key.value) {
            node->right = insert(node->right, key);

        } else if(key.value <= node->key.value) {
            node->left = insert(node->left, key);
        }

        balancing_factor = calculate_balance(node);

        if(balancing_factor > 1){
            if(key.value > node->key.value) node->left = left_rotation(node->left);
            return right_rotation(node);

        } else if(balancing_factor < -1){
            if(key.value > node->key.value) node->right = right_rotation(node->right);
            return left_rotation(node);
        }

        return node;
    }
}

int calculate_height(node_t * node){

    if(node->right == NULL && node->left == NULL) {
        return 0;
    }
    else if(node->right == NULL) {
        return node->left->height + 1;
    }
    else if(node->left == NULL) {
        return node->right->height + 1;
    }
    else {
        return max(node->right->height, node->left->height) + 1;
    }
}

int calculate_balance(node_t * node){
    int left, right;

    if(node->left == NULL) {
        left = 0;
    } else {
        left = node->left->height;
    }

    if(node->right == NULL) {
        right = 0;
    } else {
        right = node->right->height;
    }

    return left - right;
}

node_t * left_rotation(node_t * node){

    node_t * aux = node->right;
    node->right = aux->left;
    node->height = calculate_height(node);
    aux->left = node;
    aux->height = calculate_height(aux);

    return aux;
}

node_t * right_rotation(node_t * node){

    node_t * aux = node->left;
    node->left = aux->right;
    node->height = calculate_height(node);
    aux->right = node;
    aux->height = calculate_height(aux);

    return aux;
}

node_t * find_node(node_t * node, int value, int mode){
    node_key_t aux;

    while(true){

        if(node == NULL) {
            return NULL;
        }

        if(node->deleted && mode == HIDE_DELETED) {
            return NULL;
        }

        if(node->key.value == value) {
            return node;
        }

        if(value > node->key.value) {
            node = node->right;
        }
        else {
            node = node->left;
        }
    }
}

void print_tree(FILE * destination, node_t *node, int mode) {
    char buffer[BUFFER_SIZE];

    if (node == NULL) {
        return;
    }

    print_tree(destination, node->left, mode);

    if(!node->deleted) {
        snprintf(buffer, BUFFER_SIZE, "key: %d\n", node->key.value);
        fwrite(buffer, 1, sizeof(buffer), destination);
    } else if (mode == SHOW_DELETED && node->deleted) {
        snprintf(buffer, BUFFER_SIZE, "(deleted) key: %d\n", node->key.value);
        fwrite(buffer, 1, sizeof(buffer), destination);
    }

    print_tree(destination, node->right, mode);
}

int delete(node_t * source, int value) {
    node_t * node;

    if((node = find_node(source, value, HIDE_DELETED)) == NULL) {
        printf("node not found\n");
        return false;

    } else {
        printf("node found and deleted\n");
        node->deleted = true;
        return true;
    }

}
