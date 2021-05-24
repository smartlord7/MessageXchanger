#include <malloc.h>
#include "stddef.h"
#include "../../structs/user_session.h"
#include "../../structs/session_tree_node_t.h"
#include "session_tree.h"

static session_tree_node_t * insert_session_(session_tree_node_t * node, user_session_t * session);
static session_tree_node_t * find_session_(session_tree_node_t * node, uint host_ip, uint port);

static session_tree_node_t * root = NULL;

user_session_t * find_session(int ip, int port) {
    session_tree_node_t * aux = find_session_(root, ip, port);

    if (aux == NULL) {
        return NULL;
    }

    return aux->session;
}

user_session_t * insert_session(user_session_t * session) {
    root = insert_session_(root, session);

    return session;
}

static session_tree_node_t * insert_session_(session_tree_node_t * node, user_session_t * session){
    if(node == NULL){

        node = (session_tree_node_t *) malloc(sizeof(session_tree_node_t));
        node->session = session;

        return node;
    } else {
        uint cmp1, cmp2;
        cmp1 = session->host_ip + session->port;
        cmp2 = node->session->host_ip + node->session->port;

        if(cmp1 > cmp2) {
            node->right = insert_session_(node->right, session);

        } else if(cmp1 > cmp2) {
            node->left = insert_session_(node->left, session);
        }
        return node;
    }
}

static session_tree_node_t * find_session_(session_tree_node_t * node, uint host_ip, uint port){
    while(true){

        if(node == NULL) {
            return NULL;
        }

        uint cmp1, cmp2;
        cmp1 = host_ip + port;
        cmp2 = node->session->host_ip + node->session->port;

        if (cmp1 == cmp2) {
            return node;
        }

        if(cmp1 > cmp2) {
            node = node->right;
        } else {
            node = node->left;
        }
    }
}