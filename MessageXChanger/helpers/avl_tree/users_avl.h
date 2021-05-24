#ifndef MESSAGEXCHANGER_USERS_AVL_H
#define MESSAGEXCHANGER_USERS_AVL_H

#include "stdio.h"
#include "../../structs/avl_node.h"

#define true 1
#define false 0

#define HIDE_DELETED 0
#define SHOW_DELETED 1

extern node_t * get_root();
extern int get_num_active_users();
extern int get_num_del_users();
extern void print_users(node_t * node);
extern void write_users(FILE * dest_file, int mode);
extern void insert_user(user_t * user);
extern user_t * find_user(char user_name[LARGE_SIZE], int mode);
extern user_t * delete_user(char user_name[LARGE_SIZE]);

#endif //MESSAGEXCHANGER_USERS_AVL_H
