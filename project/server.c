#include "stdarg.h"
#include "errno.h"
#include "string.h"
#include "netinet/in.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "pthread.h"
#include "sys/wait.h"
#include "helpers/client_reg_file_manager/client_reg_file_manager.h"
#include "helpers/trees/user_tree.h"
#include "tcp_ip/tcp/tcp.h"
#include "server.h"
#include "util/strings/strings.h"
#include "crypt.h"
#include "tcp_ip/udp/udp.h"
#include "structs/request_msg.h"
#include "structs/handshake_t.h"
#include "util/msg_queue/msg_queue.h"
#include "structs/user_session.h"
#include "helpers/trees/session_tree.h"
#include "structs/response_msg_t.h"


_Noreturn static void * tcp_worker();
_Noreturn static void * udp_worker();
static void handle_admin();
static void signal_handler(int signum);
static user_t * validate_user(char buffer[LARGEST_SIZE]);
static void show_error_if(int cond, const char * msg, ...);
static void * session_worker(void * data);
static int authenticate_server(handshake_t * handshake, int * client_id);

in_addr admin_addr_in;
int clients_port, admin_port, server_fd, admin_fd, clients_fd, msq_id;


int main(int argc, char * argv[]) {
    char * reg_file_path = NULL, * reg_file_path_b = NULL;

    printf(SERVER_INIT);

    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, signal_handler);

    show_error_if(argc != 5, INVALID_CMD_ARGS);

    clients_port = atoi(argv[1]);
    admin_port = atoi(argv[2]);
    reg_file_path = argv[3];
    reg_file_path_b = argv[4];

    msq_id = create_msg_queue();

    pthread_t tcp_handler_thread = {0}, udp_handler_thread = {0};

    client_reg_reader_init(reg_file_path, reg_file_path_b);
    read_client_regs();

    printf(REG_LOADED);

    assert(pthread_create(&udp_handler_thread, NULL, udp_worker, NULL) == 0);
    assert(pthread_create(&tcp_handler_thread, NULL, tcp_worker, NULL) == 0);

    pthread_join(udp_handler_thread, NULL);
    pthread_join(tcp_handler_thread, NULL);
    destroy_msg_queue(msq_id);
}

static void * tcp_worker() {
    sockaddr_in admin_addr = {0};
    int admin_addr_size = sizeof(admin_addr);

    server_fd = init_tcp(admin_port, NUM_MAX_TCP_CONNECTIONS);

    printf(TCP_ONLINE, admin_port);

    while (true) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        admin_fd = accept(server_fd, (sockaddr *) &admin_addr, (socklen_t *) &admin_addr_size);

        if (admin_fd > 0) {
            handle_admin();
        }
    }
}

static void * udp_worker() {
    clients_fd = init_udp_server(clients_port);
    set_udp_timeout(clients_fd, UDP_ALL_TIMEOUT_SEC);

    user_session_t * session = NULL;
    sockaddr_in client_addr = {0};
    handshake_t handshake = {0};
    request_msg_t request = {0};
    response_msg_t resp_msg = {0};
    pthread_t client_threads[NUM_MAX_UDP_CLIENTS];
    char * ipv4 = NULL;
    int i = 0, j = 0, auth_result, client_id;

    printf(UDP_ONLINE, clients_port);

    while (i < NUM_MAX_UDP_CLIENTS) {
        request.method = REQ_NA;
        show_error_if(udp_receive_msg(clients_fd, &handshake.client_addr, (void *) &request, sizeof(request_msg_t)) == -1, ERROR_UDP_RCV, clients_fd);
        if (request.method == REQ_NA) {
            printf(SERVER_DISCONNECT_TIMEOUT);
            break;
        }

        handshake.msg = request;
        client_addr = handshake.client_addr;
        ipv4 = ipv4_to_string(&client_addr.sin_addr);

        // check if the user has an active session (with the current port).
        session = find_session(client_addr.sin_addr.s_addr, client_addr.sin_port);

        if (session == NULL) {
            printf(CLIENT_CONNECT, ipv4, client_addr.sin_port);

            auth_result = authenticate_server(&handshake, &client_id);

            if (auth_result == USER_AUTHENTICATION_ERROR) {
                continue;
            } else if (auth_result == USER_ALREADY_LOGGED) {
                snd_msg(msq_id, (void *) &handshake, sizeof(handshake_t));
                continue;
            }

            handshake.client_id = client_id + 1;

            assert(pthread_create(&client_threads[i], NULL, session_worker, (void *) &handshake.client_id) == 0);
            free(ipv4);
        } else {
            if (strcmp(session->user->user_name, handshake.msg.user_name) != 0 && handshake.msg.method == REQ_LOGIN) {

                resp_msg.type = RESP_IP_NOT_ALLOWED;

                udp_send_msg(clients_fd, &client_addr, (char *) &resp_msg, sizeof(response_msg_t));

                printf(CLIENT_IP_NOT_ALLOWED, ipv4);

                continue;
            }

            handshake.client_id = session->user->id + 1;
            // just send the udp message to the respective client read_worker through the msg queue, since it is authenticated.
            snd_msg(msq_id, (void *) &handshake, sizeof(handshake_t));
        }
    }

    // wait for all the client read_worker threads.
    while (j < i) {
        assert(pthread_join(client_threads[j], NULL) == 0);
        j++;
    }

    pthread_exit(NULL);
}

static int authenticate_server(handshake_t * handshake, int * client_id) {
    user_t * user = NULL;
    user_session_t * session = NULL;
    response_msg_t resp_msg = {0};
    sockaddr_in * client_addr = &handshake->client_addr;
    char * ipv4 = NULL;

    ipv4 = ipv4_to_string(&handshake->client_addr.sin_addr);

    // check if the user exists
    user = find_user(handshake->msg.user_name, HIDE_DELETED);

    if (user == NULL) {
        // the user doesn't exist.
        resp_msg.type = RESP_USER_NOT_FOUND;
        send_resp();

        printf(CLIENT_USER_NOT_FOUND, ipv4, handshake->msg.user_name);

        return USER_AUTHENTICATION_ERROR;
    } else {
        if (user->curr_session != NULL) {
            return USER_ALREADY_LOGGED;
        }

        // check if the request is to login and if the user is logged or not-
        if (handshake->msg.method != REQ_LOGIN && user->curr_session == NULL) {
            resp_msg.type = RESP_NO_SESSION;
            send_resp();

            printf(CLIENT_NO_SESSION, ipv4, handshake->msg.user_name);

            return USER_AUTHENTICATION_ERROR;
        }

        if (user->host_ip != handshake->client_addr.sin_addr.s_addr) {
            resp_msg.type = RESP_IP_NOT_ALLOWED;
            send_resp();

            printf(CLIENT_IP_NOT_ALLOWED, ipv4);

            return USER_AUTHENTICATION_ERROR;
        }

        // check if the user has inserted the correct password.
        if (strcmp(handshake->msg.hash, user->password_hash) != 0) {
            resp_msg.type = RESP_WRONG_PASSWORD;
            send_resp();

            printf(CLIENT_WRONG_PASSWORD, ipv4);

            return USER_AUTHENTICATION_ERROR;

        }
    }

    // create a new session for the user in question.
    session = (user_session_t *) malloc(sizeof(user_session_t));
    user->curr_session = session;

    session->user = user;
    session->host_ip = handshake->client_addr.sin_addr.s_addr;
    session->port = handshake->client_addr.sin_port;
    session->user = user;

    insert_session(session);
    resp_msg.type = RESP_LOGIN_SUCCESS;
    resp_msg.port = session->port;
    resp_msg.permissions = user->has_client_server_conn * 1 + user->has_p2p_conn * 10 + user->has_group * 100;
    * client_id = user->id;

    send_resp();
    printf(CLIENT_LOGGED_IN, ipv4);

    return USER_AUTHENTICATION_SUCCESS;
}

static void * session_worker(void * data) {
    long client_id = * (long *) data;
    user_t * user = NULL;
    handshake_t msg_rcved = {0};
    sockaddr_in * client_addr = NULL, dest_addr = {0};
    response_msg_t resp_msg = {0};
    char * client_ip = NULL;
    int leave = false;

    client_ip = ipv4_to_string(&msg_rcved.client_addr.sin_addr);
    while (!leave) {
        rcv_msg(msq_id, (void *) &msg_rcved, sizeof(handshake_t), client_id);
        client_addr = &msg_rcved.client_addr;

        switch (msg_rcved.msg.method) {
            case REQ_LOGIN:
                printf(CLIENT_USER_ALREADY_LOGGED, client_ip, msg_rcved.msg.user_name);
                resp_msg.type = RESP_ALREADY_LOGGED_IN;
                send_resp();
                break;
            case REQ_GET_USER:
                user = find_user(msg_rcved.msg.user_name, HIDE_DELETED);

                if (user == NULL) {
                    printf(CLIENT_REQ_USER_NOT_FOUND, client_ip, msg_rcved.msg.user_name);
                    resp_msg.type = RESP_USER_NOT_FOUND;
                    send_resp();

                    continue;
                }

                if (user->curr_session == NULL) {
                    printf(CLIENT_REQ_USER_NOT_ONLINE, client_ip, msg_rcved.msg.user_name);
                    resp_msg.type = RESP_USER_NOT_ACTIVE;
                    send_resp();

                    continue;
                }

                resp_msg.type = RESP_NON_MEDIATED;
                resp_msg.ip_address = user->host_ip;
                resp_msg.port = user->curr_session->port;

                send_resp();

                break;
            case REQ_MULTICAST:
                break;
            case REQ_END:
                resp_msg.type = RESP_END;
                send_resp();

                // detroy user session.
                user = find_user(msg_rcved.msg.user_name, HIDE_DELETED);
                user->curr_session = NULL;
                delete_session(msg_rcved.client_addr.sin_addr.s_addr);
                leave = true;
                break;

            case REQ_MEDIATED:
                user = find_user(msg_rcved.msg.user_name, HIDE_DELETED);

                if (user == NULL) {
                    printf(CLIENT_REQ_USER_NOT_FOUND, client_ip, msg_rcved.msg.user_name);
                    resp_msg.type = RESP_USER_NOT_FOUND;
                    send_resp();

                    continue;
                }

                if (user->curr_session == NULL) {
                    printf(CLIENT_REQ_USER_NOT_ONLINE, client_ip, msg_rcved.msg.user_name);
                    resp_msg.type = RESP_USER_NOT_ACTIVE;
                    send_resp();

                    continue;
                }

                dest_addr = *client_addr;
                dest_addr.sin_addr.s_addr = user->host_ip;
                dest_addr.sin_port = htons(user->curr_session->port);

                resp_msg.type = RESP_MEDIATED;
                strcpy(resp_msg.username, msg_rcved.msg.user_name);
                strcpy(resp_msg.buffer, msg_rcved.msg.message);

                udp_send_msg(clients_fd, &dest_addr, (void *) &resp_msg, sizeof(response_msg_t));

                break;
        }
    }

    free(client_ip);
    pthread_exit(NULL);
}

static void handle_admin() {
    int n_read, list_mode = HIDE_DELETED, stay = true;
    char buffer[LARGEST_SIZE], * aux = NULL, * admin_ip;
    user_t * user = NULL;

    admin_ip = ipv4_to_string(&admin_addr_in);;

    printf(ADMIN_CONNECT, admin_ip);

    while (stay) {
        do {
            aux = &buffer[0];
            n_read = (int) read(admin_fd, buffer, LARGEST_SIZE - 1);
            buffer[n_read - 1] = '\0';
            printf(SERVER_TCP " <- " ADMIN "%s\n", admin_ip, buffer);

            if (starts_with_ignore_case(buffer, CMD_ADD)) {
                aux += strlen(CMD_ADD);

                user = validate_user(aux);

                if (user == NULL) {
                    continue;
                }

                if (find_user(user->user_name, HIDE_DELETED) == NULL) {
                    insert_user(user);
                    send_tcp_response(admin_fd, USER_CREATE, user->user_name);
                } else {
                    send_tcp_response(admin_fd, ERROR_USERNAME_REPEAT, user->user_name);
                }

            } else if (starts_with_ignore_case(buffer, CMD_DEL)) {
                aux += (int) strlen(CMD_DEL);
                aux = strtok(aux, ADD_DELIM);

                if (aux == NULL) {
                    send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "user name");
                    continue;
                }
                aux = trim_string(aux);

                if (delete_user(aux) == NULL) {
                    send_tcp_response(admin_fd, TCP_ERROR_USER_NOT_FOUND, aux);
                } else {
                    send_tcp_response(admin_fd, USER_DELETE, aux);
                }

            } else if (starts_with_ignore_case(buffer, CMD_LIST)) {
                aux = get_user_list_as_str(list_mode);
                send_tcp_response(admin_fd, "\n%s", aux);
                free(aux);
            } else if (starts_with_ignore_case(buffer, CMD_QUIT)) {
                send_tcp_response(admin_fd, SERVER_DISCONNECT);
                stay = false;
                break;

            } else if (starts_with_ignore_case(buffer, SHOW_DEL)) {
                list_mode = !list_mode;
                send_tcp_response(admin_fd, SHOW_DELETE_TOGGLE, list_mode);

            } else {
                send_tcp_response(admin_fd, ERROR_INVALID_CMD, buffer);
            }
        } while (n_read > 0);
    }
}

static user_t * validate_user(char buffer[LARGEST_SIZE]) {
    char * token = NULL, user_name[LARGE_SIZE], host_ip[SMALL_SIZE], password_hash[SMALL_SIZE];
    uint has_client_server_conn, has_p2p_conn, has_group;
    user_t * user = NULL;

    token = strtok(buffer, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "user name");
        return NULL;
    }
    strcpy(user_name, token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "host ip");
        return NULL;
    }
    strcpy(host_ip, token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "password");
        return NULL;
    }
    strcpy(password_hash, crypt(token, PASSWORD_HASH_OPT));

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "has client server connection");
        return NULL;
    }
    has_client_server_conn = atoi(token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "has P2P connection");
        return NULL;
    }
    has_p2p_conn = atoi(token);

    token = strtok(NULL, ADD_DELIM);
    if (token == NULL) {
        send_tcp_response(admin_fd, ERROR_MISSING_PARAM, "has group");
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
            destroy_msg_queue(msq_id);
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

void show_error_if(int cond, const char * msg, ...) {
    if (cond) {
        va_list args;
        va_start(args, msg);

        char buffer[LARGE_SIZE];
        vsnprintf(buffer, LARGE_SIZE, msg, args);

        if (errno != 0) {
            printf("%s\nERRNO: %d - %s", buffer, errno, strerror(errno));
        } else {
            printf("%s\n", buffer);
        }

        va_end(args);
        exit(EXIT_FAILURE);
    }
}