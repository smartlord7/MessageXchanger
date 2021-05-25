//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_CLIENT_H
#define MESSAGEXCHANGER_CLIENT_H

#include "tcp_ip/udp/udp.h"
#include "util/strings/strings.h"
#include "util/numbers/numbers.h"
#include "tcp_ip/tcp/tcp.h"
#include "structs/request_msg.h"
#include "structs/response_msg_t.h"

#define IP "0.0.0.0"
#define PORT 4500
#define NUM_PERMITS 3
#define PASSWORD_HASH_OPT "DZ"
#define UDP_TIMEOUT_SEC 120
#define CHECK_TIMEOUT_SEC 1

#define USERNAME "USERNAME"
#define PASSWORD "PASSWORD"
#define MESSAGE "MESSAGE"
#define CONTACT_USER "USER TO BE CONTACTED"

#define TYPES_CHAT "\n---- TYPES OF CHAT ----\n\n"
#define MEDIATED_CHAT "MEDIATED CHAT - OPTION %d\n"
#define NON_MEDIATED_CHAT "NON MEDIATED CHAT - OPTION %d\n"
#define GROUP_CHAT "GROUP CHAT - OPTION %d\n"
#define GET_CHAT_OPTION "\nPRESS ANY OTHER OPTION TO LEAVE\nOPTION: "
#define INSERT_EXIT "INSERT 'EXIT' TO LEAVE THE CHAT\n"
#define FAILED_REC_CLOSE "FAILED TO RECEIVE A RESPONSE! CLOSING CHAT!\n"
#define RECEIVED_MSG "RECEIVED MESSAGE FROM USER %s!\n CONTENT: %s\n"
#define CLOSE_CHAT "CLOSING CHAT!\n"
#define USER_NOT_FOUND CLOSE_CHAT "COULDN'T SEND MESSAGE TO USER %s\n"

#define EXIT "EXIT"

#define LOGIN_SUCCESS "LOGIN SUCCESSFUL\n"
#define LOGIN_FAILURE "LOGIN FAILED\n"

#define true 1
#define false 0

#define REJECTED "000"

enum permits {
    SERVER_COMMS,
    P2P,
    GROUP_COMMS,
};

#endif //MESSAGEXCHANGER_CLIENT_H
