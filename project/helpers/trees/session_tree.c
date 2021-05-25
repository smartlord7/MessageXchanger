#include <malloc.h>
#include "stddef.h"
#include "../../structs/user_session.h"
#include "../../structs/session_tree_node_t.h"
#include "session_tree.h"

static session_tree_node_t * insert_session_(session_tree_node_t * node, user_session_t * session);
static session_tree_node_t *find_session_(session_tree_node_t *node, uint host_ip, uint port);
static session_tree_node_t *delete_session_(session_tree_node_t *node, uint ip);
static session_tree_node_t * min_value_node(session_tree_node_t * node);

static session_tree_node_t * root = NULL;

user_session_t * insert_session(user_session_t * session) {
    root = insert_session_(root, session);

    return session;
}

user_session_t * find_session(uint ip, uint port) {
    session_tree_node_t * aux = find_session_(root, ip, port);

    if (aux == NULL) {
        return NULL;
    }

    return aux->session;
}

user_session_t * delete_session(uint host_ip) {
    root = delete_session_(root, host_ip);

    return root->session;
}

static session_tree_node_t * insert_session_(session_tree_node_t * node, user_session_t * session){
    if(node == NULL){

        node = (session_tree_node_t *) malloc(sizeof(session_tree_node_t));
        node->session = session;

        return node;
    } else {
        if(session->host_ip > node->session->host_ip) {
            node->right = insert_session_(node->right, session);

        } else if(session->host_ip < node->session->host_ip) {
            node->left = insert_session_(node->left, session);
        } else {
            if (session->port > node->session->port) {
                node->right = insert_session_(node->right, session);
            } else {
                node->left = insert_session_(node->left, session);
            }
        }

        return node;
    }
}

static session_tree_node_t * find_session_(session_tree_node_t *node, uint host_ip, uint port) {
    while(node != NULL){
        if(host_ip > node->session->host_ip) {
            node = node->right;
        } else  if (host_ip < node->session->host_ip){
            node = node->left;
        } else {
            if (port > node->session->port) {
                node = node->right;
            } else if (port < node->session->port) {
                node = node->left;
            } else {
                return node;
            }
        }
    }

    return NULL;
}

session_tree_node_t * delete_session_(session_tree_node_t *node, uint ip) {
    if (root == NULL)
        return root;

    int cmp1 = ip, cmp2 = node->session->host_ip;

    if (cmp1 < cmp2) {
        root->left = delete_session_(root->left, ip);
    } else if (cmp1 > cmp2) {
        root->right = delete_session_(root->right, ip);
    } else {
        if (root->left == NULL) {
            session_tree_node_t * temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            session_tree_node_t * temp = root->left;
            free(root);
            return temp;
        }

        session_tree_node_t * temp = min_value_node(root->right);

        root->session = temp->session;

        root->right = delete_session_(root->right, ip);
    }
    return root;
}

static session_tree_node_t * min_value_node(session_tree_node_t * node) {
    session_tree_node_t * current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}
