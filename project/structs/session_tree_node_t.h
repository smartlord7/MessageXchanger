#ifndef MESSAGEXCHANGER_SESSION_TREE_NODE_T_H
#define MESSAGEXCHANGER_SESSION_TREE_NODE_T_H

typedef struct user_session_t user_session_t;
typedef struct session_tree_node_t session_tree_node_t;

typedef struct session_tree_node_t {
    user_session_t * session;
    session_tree_node_t * left, * right;
} session_tree_node_t;

#endif //MESSAGEXCHANGER_SESSION_TREE_NODE_T_H
