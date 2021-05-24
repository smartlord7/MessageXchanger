#include "malloc.h"
#include "assert.h"
#include "string.h"
#include "user.h"

user_t * new_user(char user_name[LARGE_SIZE],  char password_hash[XLARGE_SIZE], int host_ip, uint has_client_server_conn, uint has_p2p_conn, uint has_group) {
    user_t * user = (user_t *) malloc(sizeof(user_t));

    assert(user != NULL);

    strcpy(user->user_name, user_name);
    strcpy(user->password_hash, password_hash);
    user->host_ip = host_ip;
    user->has_client_server_conn = has_client_server_conn;
    user->has_p2p_conn = has_p2p_conn;
    user->has_group = has_group;
    user->is_deleted = false;

    return user;
}
