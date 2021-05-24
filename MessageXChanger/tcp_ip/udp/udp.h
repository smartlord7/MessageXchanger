//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_UDP_H
#define MESSAGEXCHANGER_UDP_H

#include "stdio.h"
#include "stdlib.h"
#include "arpa/inet.h"

#define BUFFER_SIZE

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

extern int create_socket(int ip_version, int type, int protocol);
extern int bind_socket(int socket_fd, sockaddr * address);
extern void initialize_socket(sockaddr_in * socket, int family, int port, char * ip_address);
extern int udp_receive_msg(int socket_fd, sockaddr_in * source, char * buffer, size_t msg_size);
extern int udp_send_msg(int socket_fd, sockaddr_in * destination, char * message, size_t msg_size);

#endif //MESSAGEXCHANGER_UDP_H
