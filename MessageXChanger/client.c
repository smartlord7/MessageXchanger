#include "assert.h"
#include <stdio.h>
#include <unistd.h>
#include "client.h"

static int get_server(char * ip_address, int port);
static int authenticate_client();
static void communicate();
static void comms_available(uint permissions_code, char * buffer);
static void active_direct_chat(sockaddr_in destination, int mode);
static void passive_direct_chat(sockaddr_in source, int mode, response_msg_t msg);

static char username[SMALL_SIZE];
static int server_fd;
static uint permissions;
static char password_hash[SMALL_SIZE];
static sockaddr_in server = {0};

int main () {
    get_server(IP, PORT);

    if(authenticate_client() == EXIT_FAILURE) {
        return EXIT_SUCCESS;
    }

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

    set_udp_timeout(server_fd, UDP_TIMEOUT_SEC);

    if(get_input(USERNAME, username) == EXIT_FAILURE || get_input(PASSWORD, password) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    password[strlen(password) - 1] = '\0';

    request.method = REQ_LOGIN;
    strcpy(request.user_name,username);
    strcpy(request.hash, (crypt(trim_string(password), PASSWORD_HASH_OPT)));

    udp_send_msg(server_fd, &server, (char *) &request, (size_t) sizeof(request_msg_t));

    udp_receive_msg(server_fd, &server, (char *) &response, sizeof(response_msg_t));

    /*if(response.type != RESP_LOGIN_SUCCESS) {
        printf(LOGIN_FAILURE);
        return EXIT_FAILURE;
    } else {
        printf(LOGIN_SUCCESS);
        permissions = response.permissions;
    }*/

    permissions = 111;

    return EXIT_SUCCESS;
}

void communicate() {
    int i = 0, option;
    char options[LARGE_SIZE], input[SMALL_SIZE];
    sockaddr_in source;
    response_msg_t response;

    comms_available(permissions, options);

    while(true) {
        set_udp_timeout(server_fd, CHECK_TIMEOUT_SEC);

        //check if received a message
        if(udp_receive_msg(server_fd, &source, (char *) &response, sizeof(response_msg_t)) == EXIT_SUCCESS) {

            switch (response.type) {
                case RESP_MEDIATED:
                    passive_direct_chat(server, REQ_MEDIATED, response);
                    break;
                case RESP_NON_MEDIATED:
                    passive_direct_chat(source, REQ_NON_MEDIATED, response);
                    break;
                case RESP_MULTICAST:
                    printf("multicast\n");
                    break;
                default:
                    break;
            }

            return;
        }

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
                    active_direct_chat(server, REQ_MEDIATED);
                } else {
                    return;
                }

                break;
            case (P2P):

                if(((permissions >> P2P) % 2)) {
                    active_direct_chat(server, REQ_NON_MEDIATED);
                } else {
                    return;
                }

                break;
            case (GROUP_COMMS):

                if(((permissions >> GROUP_COMMS) % 2)) {
                    //group_chat_active();
                } else {
                    return;
                }
                break;

            default:
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
    buffer = append(buffer, aux);
}

void active_direct_chat(sockaddr_in destination, int mode) {
    request_msg_t request;
    response_msg_t response;
    uint p_feedback, n_feedback, method;

    set_udp_timeout(server_fd, UDP_TIMEOUT_SEC);

    if(get_input(CONTACT_USER, request.user_name) == EXIT_FAILURE) {
        return;
    }

    if(mode == REQ_MEDIATED) {
        p_feedback = RESP_MEDIATED;
        n_feedback = RESP_MED_FAILED;
        method = REQ_MEDIATED;




    } else {

        p_feedback = RESP_NON_MEDIATED;
        n_feedback = RESP_NON_MEDIATED_FAILED;
        method = REQ_NON_MEDIATED;

        request.method = GET_USER;

        //obtain other user ip and port
        udp_send_msg(server_fd, &destination, (char *) &request, sizeof(request_msg_t));

        //receive_response
        if(udp_receive_msg(server_fd, &destination, (char *) &response, sizeof(response_msg_t)) == EXIT_FAILURE) {
            printf(FAILED_REC_CLOSE);
            return;
        }

        destination.sin_addr.s_addr = response.ip_address;
        destination.sin_port = response.port;
        destination.sin_family = AF_INET;

    }



    while (true) {
        printf("\n%s", INSERT_EXIT);

        if(get_input(MESSAGE, request.message) == EXIT_FAILURE) {
            return;
        }
        request.message[strlen(request.message) - 1] = '\0';
        if(strcasecmp(request.message, EXIT) == 0) {
            printf(CLOSE_CHAT);
            return;
        }
        request.method = method;

        //send message to user through server
        udp_send_msg(server_fd, &destination, (char *) &request, sizeof(request_msg_t));

        //receive_response
        if(udp_receive_msg(server_fd, &destination, (char *) &response, sizeof(response_msg_t)) == EXIT_FAILURE) {
            printf(FAILED_REC_CLOSE);
            return;
        }

        //other user is not active or registered
        if(response.type == n_feedback) {
            printf(USER_NOT_FOUND, request.user_name);
            return;
        } else if(response.type == p_feedback) {
            printf(RECEIVED_MSG, response.username, response.buffer);
        }

    }
}

void passive_direct_chat(sockaddr_in source, int mode, response_msg_t msg) {
    request_msg_t request;
    response_msg_t response;

    set_udp_timeout(server_fd, UDP_TIMEOUT_SEC);

    uint p_feedback, n_feedback, method;

    if(mode == REQ_MEDIATED) {
        p_feedback = RESP_MEDIATED;
        n_feedback = RESP_MED_FAILED;
        method = REQ_MEDIATED;
    } else {
        p_feedback = RESP_NON_MEDIATED;
        n_feedback = RESP_NON_MEDIATED_FAILED;
        method = REQ_NON_MEDIATED;
    }

    //print received message
    printf(RECEIVED_MSG, msg.username, msg.buffer);

    while (true) {
        printf("\n%s", INSERT_EXIT);

        //ask user for the response message
        if(get_input(MESSAGE, request.message) == EXIT_FAILURE) {
            return;
        }
        request.message[strlen(request.message) - 1] = '\0';
        if(strcasecmp(request.message, EXIT) == 0) {
            printf(CLOSE_CHAT);
            return;
        }
        strcpy(request.user_name, msg.username);
        request.method = method;

        //send message to user through server
        udp_send_msg(server_fd, &source, (char *) &request, sizeof(request_msg_t));

        //receive_response
        if(udp_receive_msg(server_fd, &source, (char *) &response, sizeof(response_msg_t)) == EXIT_FAILURE) {
            printf(FAILED_REC_CLOSE);
            return;
        }

        //other user is not active or registered
        if(response.type == n_feedback) {
            printf(USER_NOT_FOUND, request.user_name);
            return;
        } else if(response.type == p_feedback) {
            printf(RECEIVED_MSG, response.username, response.buffer);
        }

    }
}