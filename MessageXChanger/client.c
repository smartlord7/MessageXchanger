#include "assert.h"
#include <stdio.h>
#include <unistd.h>
#include "client.h"
#include "pthread.h"

#define GROUP_PORT 6000

static int get_server(char * ip_address, int port);
static int authenticate_client();
static void communicate();
static void comms_available(uint permissions_code, char * buffer);
static void * worker();
static void send_to_server();
static void send_p2p();
static void send_multicast(sockaddr_in group);
static void * multi_worker();

static char username[SMALL_SIZE];
static int server_fd, socket_fd, multi_fd;
static uint permissions;
static char password_hash[SMALL_SIZE];
static sockaddr_in server = {0};
static sockaddr_in myself = {0};
static sockaddr_in multicast = {0};

int main () {
    get_server(IP, PORT);

    while (authenticate_client() == EXIT_FAILURE);

    communicate();

    //sai e manda feedback ao server que vai sair

    return  0;
}

int get_server(char * ip_address, int port){
    assert(ip_address != NULL && port > 0);

    uint address = string_to_ipv4( ip_address);

    server_fd = init_udp_client(port, address, &server);

    return EXIT_SUCCESS;
}

int authenticate_client(){
    response_msg_t response;
    request_msg_t request;
    char password[SMALL_SIZE];

    if(get_input(USERNAME, username) == EXIT_FAILURE || get_input(PASSWORD, password) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    request.method = REQ_LOGIN;
    strcpy(request.user_name,username);
    strcpy(request.hash, (crypt(password, PASSWORD_HASH_OPT)));

    udp_send_msg(server_fd, &server, (char *) &request, (size_t) sizeof(request_msg_t));

    udp_receive_msg(server_fd, &server, (char *) &response, sizeof(response_msg_t));

    if(response.type == RESP_LOGIN_SUCCESS) {
        printf(LOGIN_SUCCESS);
        permissions = response.permissions;

    } else if (response.type == RESP_ALREADY_LOGGED_IN) {
        printf("USER ALREADY LOGGED IN!\n");
    } else if (response.type == RESP_USER_NOT_FOUND) {
        printf(LOGIN_FAILURE);
        printf("ERROR: USER NOT FOUND!\n");

        return EXIT_FAILURE;
    } else if (response.type == RESP_WRONG_PASSWORD) {
        printf(LOGIN_FAILURE);
        printf("ERROR: WRONG PASSWORD!\n");

        return EXIT_FAILURE;
    }

    //prepare for multicast
    if(((permissions >> GROUP_COMMS) % 2)) {
        multicast.sin_addr.s_addr = htonl(INADDR_ANY);
        multicast.sin_port = htons(GROUP_PORT);
        multicast.sin_family = AF_INET;
    }
    permissions = 111;

    return EXIT_SUCCESS;
}

void communicate() {
    int i = 0, option;
    char options[LARGE_SIZE], input[SMALL_SIZE];
    sockaddr_in source;
    response_msg_t response;
    pthread_t rec_handler = {0}, multicast_handler = {0};

    myself.sin_port = PORT;
    myself.sin_addr.s_addr = htonl(INADDR_ANY);
    myself.sin_family = AF_INET;

    assert(pthread_create(&rec_handler, NULL, worker, NULL) == 0);

    if(((permissions >> GROUP_COMMS) % 2)) {
        assert(pthread_create(&rec_handler, NULL, multi_worker, NULL) == 0);
    }

    comms_available(permissions, options);

    while(true) {
        printf("%s", options);

        fgets(input, sizeof(input), stdin);
        if(!is_numeric(input, strlen(input))) {
            option = -1;
        } else {
            option = atoi(input);
        }

        switch (option) {
            case (SERVER_COMMS):

                if(((permissions >> SERVER_COMMS) % 2)) {
                    send_to_server();
                    udp_receive_msg(socket_fd, &server, (void *) &response, sizeof(response_msg_t));

                    if (response.type == RESP_USER_NOT_FOUND) {
                        printf("ERROR: USER NOT FOUND!\n");
                    } else if (response.type == RESP_USER_NOT_ACTIVE) {
                        printf("ERROR: USER NOT ACTIVE!\n");
                    } else if (response.type == RESP_MESSAGE_SENT) {
                        printf("MESSAGE SUCCESSFULLY SENT!\n");
                    }

                } else {
                    printf("ACTION NOT ALLOWED!\n");
                    return;
                }

                break;
            case (P2P):

                if(((permissions >> P2P) % 2)) {
                    send_p2p();
                } else {
                    printf("ACTION NOT ALLOWED!\n");
                    return;
                }

                break;
            case (GROUP_COMMS):

                if(((permissions >> GROUP_COMMS) % 2)) {
                    send_multicast(multicast);
                } else {
                    printf("ACTION NOT ALLOWED!\n");
                    return;
                }
                break;

            default:
                printf("ACTION NOT ALLOWED!\n");
                return;
        }

        i++;
    }
}

void comms_available(uint permissions_code, char * buffer) {
    char aux[SMALL_SIZE];

    snprintf(aux, sizeof(aux), TYPES_CHAT);
    buffer = append(buffer, aux);

    for(int i = 0; i < NUM_PERMITS; i++) {

        if(((permissions_code >> i) % 2)) {
            switch (i) {

                case (SERVER_COMMS):
                    snprintf(aux, sizeof(aux), MEDIATED_CHAT, SERVER_COMMS);
                    buffer = append(buffer, aux);
                    break;
                case (P2P):
                    snprintf(aux, sizeof(aux), NON_MEDIATED_CHAT, P2P);
                    buffer = append(buffer, aux);
                    break;
                case (GROUP_COMMS):
                    snprintf(aux, sizeof(aux), GROUP_CHAT, GROUP_COMMS);
                    buffer = append(buffer, aux);
                    break;
                default:
                    break;

            }
        }
    }

    snprintf(aux, sizeof(aux), GET_CHAT_OPTION);
    append(buffer, aux);
}

void * worker() {
    response_msg_t response;

    assert((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    //assert(bind(socket_fd, (sockaddr *) &myself, sizeof(sockaddr_in)) != -1);

    while(true) {

        if(udp_receive_msg(socket_fd, &myself, (void *) &response, sizeof(response_msg_t))) {
            printf(RECEIVED_MSG, response.username, response.buffer);
        }

    }
}

void send_to_server() {
    request_msg_t request;

    if(get_input(CONTACT_USER, request.user_name) == EXIT_FAILURE || get_input(MESSAGE, request.message)) {
        return;
    }

    request.method = REQ_MEDIATED;
    udp_send_msg(server_fd, &server, (void *) &request, sizeof(request_msg_t));

}

void send_p2p() {
    request_msg_t request, message;
    response_msg_t response;
    sockaddr_in aux;

    if(get_input(CONTACT_USER, request.user_name) == EXIT_FAILURE || get_input(MESSAGE, message.message)) {
        return;
    }
    request.method = REQ_GET_USER;

    udp_send_msg(server_fd, &server, (void *) &request, sizeof(request_msg_t));

    if(udp_receive_msg(server_fd, &server, (void *) &response, sizeof(response_msg_t)) == EXIT_FAILURE) {
        printf(FAILED_REC_CLOSE);
        return;
    }

    if(response.type == RESP_USER_NOT_FOUND) {
        printf(USER_NOT_FOUND, request.user_name);
        return;
    }

    //fill info about destination
    aux.sin_addr.s_addr = response.ip_address;
    aux.sin_port = response.port;
    aux.sin_family = AF_INET;

    //fill message info
    message.method = REQ_NON_MEDIATED;
    strcpy(message.user_name, request.user_name);

    //send message
    udp_send_msg(socket_fd, &aux, (void *) &request, sizeof(response_msg_t));
}

static void send_multicast(sockaddr_in group) {
    request_msg_t request;

    if(get_input(MESSAGE, request.message)) {
        return;
    }
    request.method = REQ_MEDIATED;

    udp_send_msg(server_fd, &server, (void *) &request, sizeof(request_msg_t));
}

void * multi_worker() {
    response_msg_t response;

    assert((multi_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    assert(bind(multi_fd, (sockaddr *) &multicast, sizeof(sockaddr_in)) != -1);

    int multicastTTL = 255;
    if (setsockopt(multi_fd, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &multicastTTL, sizeof(multicastTTL)) < 0) {
        perror("Error on multicast setup\n");
        return NULL;
    }

    while(true) {

        if(udp_receive_msg(multi_fd, &multicast, (void *) &response, sizeof(response_msg_t))) {
            printf(RECEIVED_MSG_MULTI, response.buffer);
        }

    }

}