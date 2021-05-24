#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include "malloc.h"
#include "assert.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "tcp.h"
#include "../../global.h"

int init_tcp(int server_port, int num_max_conns) {
    sockaddr_in addr = {0};
    int socket_fd;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server_port);

    assert((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1);
    assert(bind(socket_fd, (sockaddr *) &addr, sizeof(sockaddr_in)) != -1);
    assert(listen(socket_fd, num_max_conns) != -1);

    return socket_fd;
}

char * ipv4_to_string(in_addr * addr) {
    assert(addr != NULL);

    char * buffer = (char *) malloc(INET_ADDRSTRLEN);

    assert(buffer != NULL);
    assert(inet_ntop(AF_INET, (void *) addr, buffer, INET_ADDRSTRLEN) != NULL);

    return buffer;
}

uint string_to_ipv4(const char * ipv4_str) {
    assert(ipv4_str != NULL);

    in_addr addr;

    assert(inet_pton(AF_INET, ipv4_str, (void *) &addr) != -1);

    return addr.s_addr;
}

void send_response(int fd, const char * msg, ...) {
    va_list args;
    va_start(args, msg);

    char buffer[LARGEST_SIZE];

    vsnprintf(buffer, LARGEST_SIZE, msg, args);
    assert(write(fd, (void *) buffer, strlen(buffer)) != -1);

    va_end(args);
}
