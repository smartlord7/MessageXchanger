#include "string.h"
#include "udp.h"
#include "assert.h"

int udp_receive_msg(int socket_fd, sockaddr_in * source, char * buffer, size_t msg_size) {
    socklen_t size  = sizeof(* source);

    if(recvfrom(socket_fd, buffer, msg_size * sizeof(char), 0, (sockaddr *) source, (socklen_t *) &size) < 0) {
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

int init_udp_client(int port, uint ip_address, sockaddr_in * addr) {
    int socket_fd;

    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(ip_address);
    addr->sin_port = htons(port);

    assert((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);

    return socket_fd;
}

int init_udp_server(int port) {
    int socket_fd;
    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    assert((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    assert(bind(socket_fd, (sockaddr *) &addr, sizeof(sockaddr_in)) != -1);

    return socket_fd;
}

void set_udp_timeout(int fd, int num_sec) {
    assert(fd > 0 && num_sec > 0);

    struct timeval read_timeout;
    read_timeout.tv_sec = num_sec;
    read_timeout.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
}
