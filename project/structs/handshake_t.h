#ifndef MESSAGEXCHANGER_HANDSHAKE_T_H
#define MESSAGEXCHANGER_HANDSHAKE_T_H

typedef struct handshake_t {
    long client_id;
    sockaddr_in client_addr;
    request_msg_t msg;
} handshake_t;

#endif //MESSAGEXCHANGER_HANDSHAKE_T_H
