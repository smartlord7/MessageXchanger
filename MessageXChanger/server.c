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
#include "crypt.h"

_Noreturn static void * tcp_worker(void * data);
static void * udp_worker(void * data);
static void handle_admin();
static void signal_handler(int signum);
static user_t * validate_user(char buffer[LARGEST_SIZE]);

in_addr admin_addr_in;
int clients_port, admin_port, server_fd, admin_fd;


int main(int argc, char * argv[]) {
    char * reg_file_path = NULL, * reg_file_path_b = NULL;

    printf(SERVER_INIT);

    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, signal_handler);

    if (argc != 5) {
        fprintf(stderr, "ERROR: INVALID ARGUMENTS USAGE! THEIR FORMAT MUST BE ./server <clients_port> <admin_port> <reg_file_path_txt> <reg_file_path_bin>");
        exit(EXIT_FAILURE);
    }

    clients_port = atoi(argv[1]);
    admin_port = atoi(argv[2]);
    reg_file_path = argv[3];
    reg_file_path_b = argv[4];

    pthread_t tcp_handler_thread = {0}, udp_handler_thread = {0};

    client_reg_reader_init(reg_file_path, reg_file_path_b);
    read_client_regs();

    printf(REG_LOADED);

    pthread_create(&tcp_handler_thread, NULL, tcp_worker, NULL);
    pthread_create(&tcp_handler_thread, NULL, udp_worker, NULL);

    pthread_join(tcp_handler_thread, NULL);
    pthread_join(udp_handler_thread, NULL);
}

static void * tcp_worker(void * data) {
    sockaddr_in admin_addr = {0};
    int admin_addr_size = sizeof(admin_addr);

    server_fd = init_tcp(admin_port, NUM_MAX_TCP_CONNECTIONS);

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
            printf(ADMIN "%s\n", admin_ip, buffer);

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

    free(admin_ip);
}

static user_t * validate_user(char buffer[LARGEST_SIZE]) {
    char * token = NULL, user_name[LARGE_SIZE], host_ip[SMALL_SIZE], password_hash[SMALL_SIZE];
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
    strcpy(password_hash, crypt(token, "an"));

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
            write_client_regs(SHOW_DELETED);
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