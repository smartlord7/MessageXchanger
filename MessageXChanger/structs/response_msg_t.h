#ifndef MESSAGEXCHANGER_RESPONSE_MSG_T_H
#define MESSAGEXCHANGER_RESPONSE_MSG_T_H

typedef enum response_type {
    NA = 0,
    LOGIN_SUCCESS = 1,
    LOGIN_FAILED = 2,
    USER_NOT_FOUND = 3,
    USER_FOUND = 4,
    MULTICAST_SUCCESS = 5,
    MULTICAST_FAILED = 6,
    REGULAR = 7,
    END = 8
} response_type;

typedef struct response_msg_t {
    response_type type;
    char buffer[LARGE_SIZE],
    int ip_address, port;
} response_msg_t;

#endif //MESSAGEXCHANGER_RESPONSE_MSG_T_H
