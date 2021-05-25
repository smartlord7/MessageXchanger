#include <netinet/in.h>
#include "stdlib.h"
#include "malloc.h"
#include "assert.h"
#include "string.h"
#include "user.h"
#include "../util/strings/strings.h"
#include "../tcp_ip/tcp/tcp.h"

user_t * new_user(char user_name[LARGE_SIZE], char password_hash[SMALL_SIZE], uint host_ip, uint has_client_server_conn, uint has_p2p_conn, uint has_group) {
    user_t * user = (user_t *) malloc(sizeof(user_t));

    assert(user != NULL);

    strcpy(user->user_name, user_name);
    strcpy(user->password_hash, password_hash);
    user->host_ip = host_ip;
    user->has_client_server_conn = has_client_server_conn;
    user->has_p2p_conn = has_p2p_conn;
    user->has_group = has_group;
    user->is_deleted = false;
    user->curr_session = NULL;

    return user;
}

char * user_to_string(user_t * user) {
    assert(user != NULL);

    char * buffer = NULL, * ipv4 = NULL;

    in_addr addr;
    addr.s_addr = user->host_ip;

    ipv4 = ipv4_to_string(&addr);
    buffer = (char *) malloc(XLARGE_SIZE * sizeof(char));

    assert(buffer != NULL);

    snprintf(buffer, XLARGE_SIZE, "-ID: %d, NAME: %s, IP: %s, PASSWORD HASH: %s "
                                 "CLIENT-SERVER: %u, P2P: %u, GROUP: %u, DELETED: %u\n",
                                  user->id, user->user_name, ipv4, user->password_hash,
                                  user->has_client_server_conn, user->has_p2p_conn, user->has_group, user->is_deleted);

    free(ipv4);

    return buffer;
}

user_t * parse_user(char buffer[LARGEST_SIZE]) {
    char * token = NULL,
            user_name[LARGE_SIZE],
            password_hash[SMALL_SIZE];
    int host_ip;
    uint has_client_server_conn, has_p2p_conn, has_group;
    user_t * user = NULL;

    token = trim_string(strtok(buffer, FIELD_DELIM));
    assert(token != NULL);
    strcpy(user_name, token);

    token = trim_string(strtok(NULL, FIELD_DELIM));
    assert(token != NULL);
    strcpy(password_hash, token);

    token = trim_string(strtok(NULL, FIELD_DELIM));
    assert(token != NULL);
    host_ip = atoi(token);

    token = trim_string(strtok(NULL, FIELD_DELIM));
    assert(token != NULL);
    has_client_server_conn = atoi(token);

    token = trim_string(strtok(NULL, FIELD_DELIM));
    assert(token != NULL);
    has_p2p_conn = atoi(token);

    token = trim_string(strtok(NULL, FIELD_DELIM));
    assert(token != NULL);
    has_group = atoi(token);

    user = new_user(user_name, password_hash, host_ip, has_client_server_conn, has_p2p_conn, has_group);

    return user;
}
