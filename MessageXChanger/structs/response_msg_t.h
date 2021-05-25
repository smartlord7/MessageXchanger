#ifndef MESSAGEXCHANGER_RESPONSE_MSG_T_H
#define MESSAGEXCHANGER_RESPONSE_MSG_T_H

typedef enum response_type {
    RESP_NA,
    RESP_LOGIN_SUCCESS,
    RESP_WRONG_PASSWORD,
    RESP_NO_SESSION,
    RESP_IP_NOT_ALLOWED,
    RESP_USER_NOT_FOUND,
    RESP_USER_FOUND,
    RESP_MULTICAST_SUCCESS,
    RESP_MULTICAST_FAILED,
    RESP_REGULAR,
    RESP_END
} response_type;

typedef struct response_msg_t {
    response_type type;
    char buffer[LARGE_SIZE],
    uint ip_address, port;
} response_msg_t;

#endif //MESSAGEXCHANGER_RESPONSE_MSG_T_H
