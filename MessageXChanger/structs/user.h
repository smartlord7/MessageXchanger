#ifndef MESSAGEXCHANGER_USER_H
#define MESSAGEXCHANGER_USER_H

#include "sys/types.h"
#include "../util/strings/strings.h"
#include "user_session.h"

#define FIELD_DELIM ","

typedef struct user_t{
    char user_name[LARGE_SIZE], password_hash[SMALL_SIZE];
    user_session_t * curr_session;
    int id;
    uint host_ip,
         has_client_server_conn : 1,
         has_p2p_conn : 1,
         has_group : 1,
         is_deleted : 1,
         is_logged : 1;
} user_t;

extern user_t * new_user(char user_name[LARGE_SIZE], char password_hash[SMALL_SIZE], uint host_ip, uint has_client_server_conn, uint has_p2p_conn, uint has_group);
extern user_t * parse_user(char buffer[LARGEST_SIZE]);
extern char * user_to_string(user_t * user);

#endif //MESSAGEXCHANGER_USER_H
