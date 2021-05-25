#ifndef MESSAGEXCHANGER_REQUEST_MSG_H
#define MESSAGEXCHANGER_REQUEST_MSG_H

typedef enum request_method {
    NA = 0,
    LOGIN = 1,
    SEND = 2,
    GET_USER = 3,
    LIST_USERS = 4,
    MULTICAST = 5,
    END = 6
} request_method;

typedef struct request_msg_t {
    request_method method;
    char user_name[LARGE_SIZE],
         message[LARGEST_SIZE];
    char hash[SMALL_SIZE];

} request_msg_t;

#endif //MESSAGEXCHANGER_REQUEST_MSG_H
