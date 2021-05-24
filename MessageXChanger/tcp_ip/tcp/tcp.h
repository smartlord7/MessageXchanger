#ifndef MESSAGEXCHANGER_TCP_H
#define MESSAGEXCHANGER_TCP_H

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

extern int init_tcp(int server_port, int num_max_conns);
extern char * ipv4_to_string(in_addr * addr);
extern uint string_to_ipv4(const char * ipv4_str);
extern void send_response(int fd, const char * msg, ...);

#endif //MESSAGEXCHANGER_TCP_H
