#ifndef MESSAGEXCHANGER_USER_SESSION_H
#define MESSAGEXCHANGER_USER_SESSION_H

#include "sys/types.h"

typedef struct user_t user_t;

typedef struct user_session_t {
    user_t * user;
    uint host_ip, port, logged_in : 1;
} user_session_t;

#endif //MESSAGEXCHANGER_USER_SESSION_H
