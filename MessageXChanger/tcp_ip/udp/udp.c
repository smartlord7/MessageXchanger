
#include <string.h>
#include "udp.h"

int create_socket(int ip_version, int type, int protocol) {
    int socket_fd;

    if((socket_fd = socket(ip_version, type, protocol)) < 0) {
        perror("Couldn't create socket\n");
        return EXIT_FAILURE;
    }

    return socket_fd;
}

int bind_socket(int socket_fd, sockaddr * address) {

    if(bind(socket_fd, address, sizeof(*address)) < 0) {
        perror("Couldn't bind socket\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void initialize_socket(sockaddr_in * socket, int family, int port, char * ip_address) {
    bzero((void *) socket, sizeof(*socket));

    socket->sin_family = family;
    socket->sin_port = htons(port);
    inet_aton(ip_address, (struct in_addr *) &socket->sin_addr.s_addr);

}

int udp_receive_msg(int socket_fd, sockaddr_in * source, char * buffer, size_t msg_size) {
    socklen_t size  = sizeof(*source);

    if(recvfrom(socket_fd, buffer, msg_size * sizeof(char), 0, (sockaddr *) source, (socklen_t *)&size) < 0) {
        perror("Couldn't received udp message\n");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int udp_send_msg(int socket_fd, sockaddr_in * destination, char * message, size_t msg_size) {
    socklen_t size = sizeof(*destination);

    if(sendto(socket_fd, message, msg_size * sizeof(char), 0, (sockaddr *) destination, size) < 0) {
        perror("Couldn't send udp message\n");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}