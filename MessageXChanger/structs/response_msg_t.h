#ifndef MESSAGEXCHANGER_RESPONSE_MSG_T_H
#define MESSAGEXCHANGER_RESPONSE_MSG_T_H

typedef enum response_type {
    RESP_NA,
    RESP_LOGIN_SUCCESS,
    RESP_ALREADY_LOGGED_IN,
    RESP_WRONG_PASSWORD,
    RESP_NO_SESSION,
    RESP_IP_NOT_ALLOWED,
    RESP_USER_NOT_FOUND,
    RESP_MEDIATED,
    RESP_MED_FAILED,
    RESP_NON_MEDIATED,
    RESP_NON_MEDIATED_FAILED,
    RESP_MULTICAST,
    RESP_MULTICAST_FAILED,
    USER_NOT_FOUND,
    USER_FOUND,
    RESP_END
} response_type;

typedef struct response_msg_t {
    response_type type;
    uint ip_address, port, permissions;
    char buffer[LARGE_SIZE];
    char username[SMALL_SIZE];
} response_msg_t;

#endif //MESSAGEXCHANGER_RESPONSE_MSG_T_H
