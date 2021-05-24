#ifndef MESSAGEXCHANGER_USER_H
#define MESSAGEXCHANGER_USER_H

typedef struct user_t{
    char user_name[LARGE_SIZE];
    int host_ip;
    char password_hash[XLARGE_SIZE];
    uint8 is_client_server : 1, is_p2p : 1, has_group : 1;
} user_t;

#endif //MESSAGEXCHANGER_USER_H
