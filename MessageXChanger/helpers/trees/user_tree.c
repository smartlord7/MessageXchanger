#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "../../structs/user_tree_node_t.h"
#include "user_tree.h"
static void write_users_(FILE * dest_file, user_tree_node_t * node, int mode);
static user_tree_node_t * find_user_(user_tree_node_t * node, char *user_name, int mode);
static user_tree_node_t * insert_user_(user_tree_node_t * node, user_t * user);
static user_t * delete_user_(user_tree_node_t * source, char user_name[LARGE_SIZE]);
static void get_user_list_(user_tree_node_t * node, int mode, char * buffer);

static user_tree_node_t * root = NULL;
static int num_existing_users = 0, num_del_users = 0, num_total_users = 0;

user_tree_node_t * get_root() {
    return root;
}

int get_num_active_users() {
    return num_existing_users;
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

void print_users(user_tree_node_t * node) {
    if (node == NULL) {
        return;
    }

    char * user_str = NULL;

    print_users(node->left);
    user_str = user_to_string(node->user);
    printf("%s", user_str);

    print_users(node->right);
}

void write_users_b(FILE * dest_file, int mode) {
    write_users_(dest_file, root, mode);
}

user_t * delete_user(char user_name[LARGE_SIZE]) {
    user_t * aux = delete_user_(root, user_name);

    if (aux != NULL) {
        num_del_users++;
        num_existing_users--;
    }

    return aux;
}

user_t * find_user(char user_name[LARGE_SIZE], int mode) {
    user_tree_node_t * aux = find_user_(root, user_name, mode);

    if (aux == NULL) {
        return NULL;
    }

    return aux->user;
}

int insert_user(user_t * user) {
    user_tree_node_t * aux = insert_user_(root, user);

    if (aux != NULL) {
        if (root == NULL) {
            root = aux;
        }

        user->id = num_existing_users++;
        num_total_users++;

        return true;
    }

    return false;
}

user_tree_node_t * insert_user_(user_tree_node_t * node, user_t * user){
    if(node == NULL){
        node = (user_tree_node_t *) malloc(sizeof(user_tree_node_t));

        node->user = user;
        node->right = NULL;
        node->left = NULL;

    } else {

        int cmp = strcmp(user->user_name, node->user->user_name);

        if(cmp > 0) {
            node->right = insert_user_(node->right, user);

        } else if(cmp < 0) {
            node->left = insert_user_(node->left, user);
        } else if (node->user->is_deleted) {
            node->user = user;
        }
    }

    return node;
}


static user_tree_node_t * find_user_(user_tree_node_t * node, char *user_name, int mode){
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

user_t * delete_user_(user_tree_node_t * source, char user_name[LARGE_SIZE]) {
    assert(source != NULL);

    user_tree_node_t * res = NULL;

    if ((res = find_user_(source, user_name, HIDE_DELETED)) == NULL) {
        return NULL;

    } else {
        res->user->is_deleted = true;

        return res->user;
    }
}

static void write_users_(FILE * dest_file, user_tree_node_t * node, int mode) {
    if (node == NULL) {
        return;
    }

    user_t user = * node->user;

    write_users_(dest_file, node->left, mode);

    if(!node->user->is_deleted || (mode == SHOW_DELETED && node->user->is_deleted == (uint) 1)) {
        user.curr_session = NULL;
        assert(fwrite(&user, sizeof(user_t), 1, dest_file) > 0);
    }

    write_users_(dest_file, node->right, mode);
}

static void get_user_list_(user_tree_node_t * node, int mode, char * buffer) {
    assert(buffer != NULL);

    if (node == NULL) {
        return;
    }

    char * aux = NULL;

    get_user_list_(node->left, mode, buffer);

    if (!node->user->is_deleted || (mode == SHOW_DELETED && node->user->is_deleted == (uint) 1)) {
        aux = user_to_string(node->user);
        strcat(buffer, aux);
    }
    get_user_list_(node->right, mode, buffer);
}

