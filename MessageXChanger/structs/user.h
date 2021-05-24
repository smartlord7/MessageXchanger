#ifndef MESSAGEXCHANGER_USER_H
#define MESSAGEXCHANGER_USER_H

#include "sys/types.h"
#include "../global.h"

typedef struct user_t{
    char user_name[LARGE_SIZE];
    char password_hash[XLARGE_SIZE];
    int host_ip;
    uint has_client_server_conn : 1, has_p2p_conn : 1, has_group : 1, is_deleted : 1;
} user_t;

extern user_t * new_user(char user_name[LARGE_SIZE],  char password_hash[XLARGE_SIZE], int host_ip, uint has_client_server_conn, uint has_p2p_conn, uint has_group);

#endif //MESSAGEXCHANGER_USER_H
