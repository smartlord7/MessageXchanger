#ifndef MESSAGEXCHANGER_REQUEST_MSG_H
#define MESSAGEXCHANGER_REQUEST_MSG_H

typedef enum request_method {
    NA,
    LOGIN,
    SEND,
    GET_USER,
    LIST_USERS,
    MULTICAST,
    END,
    REQ_MEDIATED,
    REQ_NON_MEDIATED
} request_method;

typedef struct request_msg_t {
    request_method method;
    char user_name[LARGE_SIZE],
         message[LARGEST_SIZE];
    char hash[SMALL_SIZE];

} request_msg_t;

#endif //MESSAGEXCHANGER_REQUEST_MSG_H
