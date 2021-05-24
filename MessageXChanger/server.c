#include "string.h"
#include "netinet/in.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "pthread.h"
#include "sys/wait.h"
#include "helpers/client_reg_file_manager/client_reg_file_manager.h"
#include "helpers/avl_tree/users_avl.h"
#include "tcp_ip/tcp/tcp.h"
#include "server.h"
#include "util/strings/strings.h"

_Noreturn static void * tcp_worker(void * data);
static void * udp_worker(void * data);
static void handle_admin();
static void signal_handler(int signum);
static user_t * validate_user(char buffer[LARGEST_SIZE]);

in_addr admin_addr_in;
int server_fd, admin_fd;


int main(void) {
    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);

    pthread_t tcp_handler_thread = {0}, udp_handler_thread = {0};

    client_reg_reader_init(CLIENT_REG_FILE_PATH, CLIENT_REG_FILE_PATH_B);
    read_client_regs();

    pthread_create(&tcp_handler_thread, NULL, tcp_worker, NULL);
    pthread_create(&tcp_handler_thread, NULL, udp_worker, NULL);

    pthread_join(tcp_handler_thread, NULL);
    pthread_join(udp_handler_thread, NULL);
}

_Noreturn static void * tcp_worker(void * data) {
    sockaddr_in admin_addr = {0};
    int admin_addr_size = sizeof(admin_addr);

    server_fd = init_tcp(SERVER_TCP_PORT, NUM_MAX_TCP_CONNECTIONS);

    while (true) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        admin_fd = accept(server_fd, (sockaddr *) &admin_addr, (socklen_t *) &admin_addr_size);

        if (admin_fd > 0) {
            handle_admin();
        }
    }
}

static void * udp_worker(void * data) {

    while (true) {

    }
}

static void handle_admin() {
    int n_read, list_mode = HIDE_DELETED;
    char buffer[LARGEST_SIZE], * aux = NULL, * admin_ip;
    user_t * user = NULL;

    admin_ip = ipv4_to_string(&admin_addr_in);;

    printf(ADMIN_CONNECT, admin_ip);

    while (true) {
        do {
            aux = &buffer[0];
            n_read = (int) read(admin_fd, buffer, LARGEST_SIZE - 1);
            buffer[n_read - 1] = '\0';

            if (starts_with_ignore_case(buffer, CMD_ADD)) {
                aux += strlen(CMD_ADD);

                user = validate_user(aux);

                if (user == NULL) {
                    continue;
                }

                if (find_user(user->user_name, HIDE_DELETED) == NULL) {
                    insert_user(user);
                    send_response(admin_fd, USER_CREATE, user->user_name);
                } else {
                    send_response(admin_fd, ERROR_USERNAME_REPEAT, user->user_name);
                }

            } else if (starts_with_ignore_case(buffer, CMD_DEL)) {
                aux += (int) strlen(CMD_DEL);
                aux = strtok(aux, ADD_DELIM);

                if (aux == NULL) {
                    send_response(admin_fd, ERROR_MISSING_PARAM, "user name");
                    continue;
                }
                aux = trim_string(aux);

                if (delete_user(aux) == NULL) {
                    send_response(admin_fd, ERROR_USER_NOT_FOUND, aux);
                } else {
                    send_response(admin_fd, USER_DELETE, aux);
                }

            } else if (starts_with_ignore_case(buffer, CMD_LIST)) {
                aux = get_user_list_as_str(list_mode);
                send_response(admin_fd, "\n%s", aux);
                free(aux);
            } else if (starts_with_ignore_case(buffer, CMD_QUIT)) {
                send_response(admin_fd, SERVER_DISCONNECT);
                return;

            } else if (starts_with_ignore_case(buffer, SHOW_DEL)) {
                list_mode = !SHOW_DELETED;
                send_response(admin_fd, SHOW_DELETE_TOGGLE, list_mode);

                send_response(admin_fd, ERROR_INVALID_CMD, buffer);

            } else {
                send_response(admin_fd, ERROR_INVALID_CMD, buffer);
            }
        } while (n_read > 0);
    }
}

static user_t * validate_user(char buffer[LARGEST_SIZE]) {
    char * token = NULL, user_name[LARGE_SIZE];
    char host_ip[SMALL_SIZE];
    ulong password_hash;
    uint has_client_server_conn, has_p2p_conn, has_group;
    user_t * user = NULL;

    token = strtok(buffer, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "user name");
        return NULL;
    }
    strcpy(user_name, token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "password");
        return NULL;
    }
    password_hash = hash(token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "host ip");
        return NULL;
    }
    strcpy(host_ip, token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "has client server connection");
        return NULL;
    }
    has_client_server_conn = atoi(token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "has P2P connection");
        return NULL;
    }
    has_p2p_conn = atoi(token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_response(admin_fd, ERROR_MISSING_PARAM, "has group");
        return NULL;
    }
    has_group = atoi(token);

    user = new_user(user_name, password_hash, string_to_ipv4(host_ip), has_client_server_conn, has_p2p_conn, has_group);

    return user;
}

static void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
            printf("SIGINT RECEIVED! CLOSING SERVER...\n");
            close(admin_fd);
            close(server_fd);
            exit(EXIT_SUCCESS);
        case SIGABRT:
            printf("SIGABRT RECEIVED! CLOSING SERVER...\n");
            close(admin_fd);
            close(server_fd);
            exit(EXIT_SUCCESS);
        case SIGSEGV:
            printf("SIGSEGV RECEIVED! CLOSING SERVER...\n");
            close(admin_fd);
            close(server_fd);
            exit(EXIT_SUCCESS);
        default:
            printf("SIGNAL %d RECEIVED!\n", signum);
    }
}
