#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "../../structs/avl_node.h"
#include "users_avl.h"

static node_t * left_rotation(node_t * node);
static node_t * right_rotation(node_t * node);
static int max(int num1, int num2);
static int calculate_height(node_t * node);
static int calculate_balance(node_t * node);
static void write_users_avl_(FILE * dest_file, node_t * node, int mode);
static node_t * find_user_(node_t * node, char *user_name, int mode);
static node_t * insert_user_(node_t * node, user_t * user);
static user_t * delete_user_(node_t * source, char user_name[LARGE_SIZE]);

static node_t * root = NULL;
static int num_active_users = 0, num_del_users = 0;

node_t * get_root() {
    return root;
}

int get_num_active_users() {
    return num_active_users;
}

int get_num_del_users() {
    return num_del_users;
}

void write_users(FILE * dest_file, int mode) {
    write_users_avl_(dest_file, root, mode);
}

user_t * delete_user(char user_name[LARGE_SIZE]) {
    return delete_user_(root, user_name);
}

user_t * find_user(char user_name[LARGE_SIZE], int mode) {
    return find_user_(root, user_name, mode)->user;
}

void insert_user(user_t * user) {
    root = insert_user_(root, user);
}

node_t * insert_user_(node_t * node, user_t * user){
    assert(user != NULL);

    int balancing_factor;

    if(node == NULL){
        node = (node_t *) malloc(sizeof(node_t));
        assert(node != NULL);

        node->user = user;
        node->deleted = false;

        return node;
    } else {

        if(strcmp(user->user_name, node->user->user_name) > 0) {
            node->right = insert_user_(node->right, user);

        } else if(strcmp(user->user_name, node->user->user_name) < 0) {
            node->left = insert_user_(node->left, user);
        } else {
            return NULL;
        }

        balancing_factor = calculate_balance(node);

        if(balancing_factor > 1){
            if(strcmp(user->user_name, node->user->user_name) > 0) node->left = left_rotation(node->left);

            return right_rotation(node);

        } else if(balancing_factor < -1){
            if(strcmp(user->user_name, node->user->user_name) > 0) node->right = right_rotation(node->right);

            return left_rotation(node);
        }

        return node;
    }
}


static node_t * find_user_(node_t * node, char *user_name, int mode){
    int cmp;

    while(true){

        if(node == NULL) {
            return NULL;
        }

        cmp = strcmp(user_name, node->user->user_name);

        if (cmp == 0) {
            if (node->deleted == (uint) 1 && mode == HIDE_DELETED) {
                return NULL;
            }

            return node;
        }

        if(cmp > 0) {
            node = node->right;
        } else {
            node = node->left;
        }
    }
}

user_t * delete_user_(node_t * source, char user_name[LARGE_SIZE]) {
    assert(source != NULL);

    node_t * res = NULL;

    if ((res = find_user_(source, user_name, HIDE_DELETED)) == NULL) {
        return NULL;

    } else {
        res->deleted = true;
        num_active_users--;
        num_del_users++;

        return res->user;
    }
}

static int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}

static int calculate_height(node_t * node){
    assert(node != NULL);

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

static int calculate_balance(node_t * node){
    assert(node != NULL);

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

static node_t * left_rotation(node_t * node){

    node_t * aux = node->right;
    node->right = aux->left;
    node->height = calculate_height(node);
    aux->left = node;
    aux->height = calculate_height(aux);

    return aux;
}

static node_t * right_rotation(node_t * node){

    node_t * aux = node->left;
    node->left = aux->right;
    node->height = calculate_height(node);
    aux->right = node;
    aux->height = calculate_height(aux);

    return aux;
}

static void write_users_avl_(FILE * dest_file, node_t * node, int mode) {
    if (node == NULL) {
        return;
    }

    user_t user = * node->user;

    write_users_avl_(dest_file, node->left, mode);

    if(!node->deleted) {
        assert(fwrite(&user, sizeof(user_t), 1, dest_file) > 0);
    } else if (mode == SHOW_DELETED && node->deleted == (uint) 1) {
        assert(fwrite(&user, sizeof(user_t), 1, dest_file) > 0);
    }

    write_users_avl_(dest_file, node->right, mode);
}

void print_users(node_t * node) {
    if (node == NULL) {
        return;
    }

    user_t * user = node->user;

    print_users(node->left);

    if (node->deleted) {
        printf("USER: %s (deleted)\n", user->user_name);
    } else {
        printf("USER: %s\n", user->user_name);
    }

    print_users(node->right);
}

