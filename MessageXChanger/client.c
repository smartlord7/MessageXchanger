#include "assert.h"
#include <stdio.h>
#include <unistd.h>
#include "client.h"
#include "unistd.h"

unsigned long hash(const char *str);
static int get_server(char * ip_address, int port);
static int authenticate_user();
static void communicate();
static void comms_available(uint permissions_code, char * buffer);
static void mediated_chat();
static void non_mediated_chat();
static void group_chat();

static char username[SMALL_SIZE];
static int server_fd;
static uint permissions;
static char password_hash[SMALL_SIZE];
static sockaddr_in server = {0};

int main () {
    get_server(IP, PORT);

    if(authenticate_user() == EXIT_FAILURE) {
        return EXIT_SUCCESS;
    }

    //faz operações que quiser
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

int authenticate_user(){
    response_msg_t response;
    request_msg_t request;
    char password[SMALL_SIZE], buffer[SMALL_SIZE];

    set_udp_timeout(server_fd, 5);

    if(get_input("Username", username) == EXIT_FAILURE || get_input("Password", password) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    request.method = LOGIN;
    strcpy(request.hash, crypt(password, PASSWORD_HASH_OPT));
    strcpy(request.user_name, username);

    printf("envia msg\n");
    udp_send_msg(server_fd, &server, (char *) &request, (size_t) sizeof(request_msg_t));

    printf("espera resposta\n");
    udp_receive_msg(server_fd, &server, (char *) &response, sizeof(response_msg_t));

    if(response.type != RESP_LOGIN_SUCCESS) {
        printf("Login failed\n");
        return EXIT_FAILURE;
    } else {
        printf("Login successful\n");
        permissions = response.permissions;
    }

    return EXIT_SUCCESS;
}

void communicate() {
    int i = 0, option;
    char options[LARGE_SIZE], input[SMALL_SIZE];


    while(true) {

        comms_available(permissions, options);
        printf("%s", options);

        fgets(input, sizeof(input), stdin);
        trim_string(input);
        if(!is_numeric(input, strlen(input))) {
            option = -1;
        } else {
            option = atoi(input);
        }

        switch (option) {
            case (SERVER_COMMS):

                if(((permissions >> SERVER_COMMS) % 2)) {
                    mediated_chat();
                } else {
                    return;
                }

                break;
            case (P2P):

                if(((permissions >> P2P) % 2)) {
                    non_mediated_chat();
                } else {
                    return;
                }

                break;
            case (GROUP_COMMS):

                if(((permissions >> GROUP_COMMS) % 2)) {
                    group_chat();
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

    snprintf(aux, sizeof(aux), "\n---- Available types of chat ----\n\n");
    buffer = append(buffer, aux);

    for(int i = 0; i < NUM_PERMITS; i++) {

        if(((permissions_code >> i) % 2)) {
            switch (i) {

                case (SERVER_COMMS):
                    snprintf(aux, sizeof(aux), "Mediated chat - press %d\n", SERVER_COMMS);
                    buffer = append(buffer, aux);
                    break;
                case (P2P):
                    snprintf(aux, sizeof(aux), "Non mediated chat - press %d\n", P2P);
                    buffer = append(buffer, aux);
                    break;
                case (GROUP_COMMS):
                    snprintf(aux, sizeof(aux), "Group chat - press %d\n", GROUP_COMMS);
                    buffer = append(buffer, aux);
                    break;
                default:
                    break;

            }
        }
    }

    snprintf(aux, sizeof(aux), "\nPress any option to end the session.\nOption: ");
    buffer = append(buffer, aux);
}

void mediated_chat() {
    char message[MEDIUM_SIZE];

    printf("Initiating mediated chat...");

    /*while (true) {
        if(strcasecmp(message, END))
    }*/

    printf("comunicando com outro client com servidor\n");
    return;
}

void non_mediated_chat(){
    printf("comunicando com outro cliente sem server\n");
    return;
}
void group_chat() {

    printf("group chat\n");

    return;
}