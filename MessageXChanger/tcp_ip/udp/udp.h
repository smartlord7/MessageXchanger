//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_UDP_H
#define MESSAGEXCHANGER_UDP_H

#include "stdio.h"
#include "stdlib.h"
#include "arpa/inet.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

extern int udp_receive_msg(int socket_fd, sockaddr_in * source, char * buffer, size_t msg_size);
extern int udp_send_msg(int socket_fd, sockaddr_in * destination, char * message, size_t msg_size);
extern int init_udp_client(int port, uint ip_address, sockaddr_in * addr);
extern int init_udp_server(int port);
extern void set_udp_timeout(int fd, int num_sec);

#endif //MESSAGEXCHANGER_UDP_H
