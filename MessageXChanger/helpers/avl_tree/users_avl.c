#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "../../structs/avl_node.h"
#include "users_avl.h"
static void write_users_avl_b(FILE * dest_file, node_t * node, int mode);
static node_t * find_user_(node_t * node, char *user_name, int mode);
static node_t * insert_user_(node_t * node, user_t * user);
static user_t * delete_user_(node_t * source, char user_name[LARGE_SIZE]);
static void get_user_list_(node_t * node, int mode, char * buffer);

static node_t * root = NULL;
static int num_active_users = 0, num_del_users = 0, num_total_users = 0;

node_t * get_root() {
    return root;
}

int get_num_active_users() {
    return num_active_users;
}

int get_num_del_users() {
    return num_del_users;
}

char * get_user_list_as_str(int mode) {
    char * buffer = (char *) malloc(LARGEST_SIZE * sizeof(char));

    assert(buffer != NULL);

    get_user_list_(root, mode, buffer);

    return buffer;
}

void print_users(node_t * node) {
    if (node == NULL) {
        return;
    }

    char * user_str = NULL;

    print_users(node->left);
    user_str = user_to_string(node->user);
    printf("%s", user_str);
    free(user_str);

    print_users(node->right);
}

void write_users_b(FILE * dest_file, int mode) {
    write_users_avl_b(dest_file, root, mode);
}

user_t * delete_user(char user_name[LARGE_SIZE]) {
    user_t * aux = delete_user_(root, user_name);

    if (aux != NULL) {
        num_del_users++;
        num_active_users--;
    }

    return aux;
}

user_t * find_user(char user_name[LARGE_SIZE], int mode) {
    node_t * aux = find_user_(root, user_name, mode);

    if (aux == NULL) {
        return NULL;
    }

    return aux->user;
}

user_t * insert_user(user_t * user) {
    root = insert_user_(root, user);

    user->id = num_active_users++;
    num_total_users++;

    return user;
}

node_t * insert_user_(node_t * node, user_t * user){
    int balancing_factor;

    if(node == NULL){

        node = (node_t *) malloc(sizeof(node_t));
        node->user = user;

        return node;
    } else {

        int cmp = strcmp(user->user_name, node->user->user_name);

        if(cmp > 0) {
            node->right = insert_user_(node->right, user);

        } else if(cmp < 0) {
            node->left = insert_user_(node->left, user);
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
            if (node->user->is_deleted == (uint) 1 && mode == HIDE_DELETED) {
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
        res->user->is_deleted = true;

        return res->user;
    }
}

static void write_users_avl_b(FILE * dest_file, node_t * node, int mode) {
    if (node == NULL) {
        return;
    }

    user_t user = * node->user;

    write_users_avl_b(dest_file, node->left, mode);

    if(!node->user->is_deleted) {
        assert(fwrite(&user, sizeof(user_t), 1, dest_file) > 0);
    } else if (mode == SHOW_DELETED && node->user->is_deleted == (uint) 1) {
        assert(fwrite(&user, sizeof(user_t), 1, dest_file) > 0);
    }

    write_users_avl_b(dest_file, node->right, mode);
}

static void get_user_list_(node_t * node, int mode, char * buffer) {
    assert(buffer != NULL);

    if (node == NULL) {
        return;
    }

    char * aux = NULL;

    get_user_list_(node->left, mode, buffer);
    aux = user_to_string(node->user);
    strcat(buffer, aux);
    free(aux);
    get_user_list_(node->right, mode, buffer);
}

