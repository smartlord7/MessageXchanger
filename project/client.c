#include "assert.h"
#include <stdio.h>
#include <unistd.h>
#include "client.h"
#include "pthread.h"

static int get_server(char * ip_address, uint port);
static int authenticate_client();
static void communicate();
static void comms_available(uint permissions_code, char * buffer);
static void * read_worker();
static void send_to_server();
static void send_p2p();

static char username[SMALL_SIZE];
static int server_fd, multi_fd, multi_rcv_fd, p2p_fd, msg_fd;
static uint permissions;
static sockaddr_in server = {0};
static sockaddr_in myself = {0};

uint port;

int main (int argc, char * argv[]) {

    if(argc != 3) {
        printf("INVALID PARAMETERS!\n");
        return EXIT_FAILURE;
    }

    uint server_port = atoi(argv[2]);

    get_server(argv[1], server_port);

    assert((multi_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    assert((multi_rcv_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    assert((p2p_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);

    while (authenticate_client() == EXIT_FAILURE);

    communicate();

    //sai e manda feedback ao server que vai sair

    return  0;
}

int get_server(char * ip_address, uint port){
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
        port = response.port;
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

    return EXIT_SUCCESS;
}

void communicate() {
    int i = 0, option;
    char options[LARGE_SIZE], input[SMALL_SIZE];
    sockaddr_in source;
    response_msg_t response;
    pthread_t read_handler = {0}, multicast_handler = {0};

    myself.sin_port = PORT;
    myself.sin_addr.s_addr = htonl(INADDR_ANY);
    myself.sin_family = AF_INET;

    assert(pthread_create(&read_handler, NULL, read_worker, NULL) == 0);

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
                    printf("FUNCTIONALITY NOT WORKING...\n");
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
void send_to_server() {
    request_msg_t request;

    if(get_input(CONTACT_USER, request.user_name) == EXIT_FAILURE || get_input(MESSAGE, request.message)) {
        return;
    }

    request.method = REQ_MEDIATED;
    udp_send_msg(server_fd, &server, (void *) &request, sizeof(request_msg_t));

}

void send_p2p(){
    request_msg_t request;
    response_msg_t response;
    sockaddr_in destination;

    if(get_input(CONTACT_USER, request.user_name) == EXIT_FAILURE) {
        return;
    }

    request.method = REQ_GET_USER;
    udp_send_msg(server_fd, &server, (void *) &request, sizeof(request_msg_t));

    if (udp_receive_msg(server_fd, (sockaddr_in *) &server, (void *) &response, sizeof(response_msg_t)) == EXIT_FAILURE) {
        return;
    }
    if(response.type != RESP_NON_MEDIATED) return;

    destination.sin_family = AF_INET;
    destination.sin_addr.s_addr= response.ip_address;
    destination.sin_port = htons(response.port);

    if(get_input(MESSAGE, request.message) == EXIT_FAILURE) {
        return;
    }
    request.method = REQ_NON_MEDIATED;

    udp_send_msg(p2p_fd, &destination, (void *) &request, sizeof(request_msg_t));
}

void * read_worker() {
    sockaddr_in reader;
    response_msg_t response;

    reader.sin_family = AF_INET;
    reader.sin_addr.s_addr = htonl(INADDR_ANY);
    reader.sin_port = htons(port);

    assert((msg_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
    assert(bind(msg_fd, (sockaddr *) &reader, sizeof(sockaddr)) != -1);

    while (true) {

        if (udp_receive_msg(msg_fd, &reader, (void *) &response, sizeof(response_msg_t)) == EXIT_SUCCESS) {
            printf(RECEIVED_MSG, response.username, response.buffer);
        }
    }
}