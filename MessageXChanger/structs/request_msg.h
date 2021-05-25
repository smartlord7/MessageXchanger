#ifndef MESSAGEXCHANGER_REQUEST_MSG_H
#define MESSAGEXCHANGER_REQUEST_MSG_H

typedef enum request_method {
    REQ_NA,
    REQ_LOGIN,
    REQ_SEND,
    REQ_GET_USER,
    REQ_LIST_USERS,
    REQ_MULTICAST,
    REQ_END,
    REQ_MEDIATED,
    REQ_NON_MEDIATED,
} request_method;

typedef struct request_msg_t {
    request_method method;
    char user_name[LARGE_SIZE],
         message[LARGEST_SIZE],
         hash[SMALL_SIZE];
    int flags : 3;

} request_msg_t;

#endif //MESSAGEXCHANGER_REQUEST_MSG_H
