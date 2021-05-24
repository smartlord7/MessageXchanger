#include "assert.h"
#include <stdio.h>
#include "client.h"

static unsigned long hash(const char *str);
static int get_server(char * ip_address, int port);
static int authenticate_user();
static int communicate();
static void comms_available(uint permissions_code, char * buffer);

static char username[SMALL_SIZE];
static sockaddr_in * server;
static int server_fd;
static uint permissions;
static unsigned long password_hash;

int main () {

    get_server(IP, PORT);

    if(authenticate_user() == EXIT_FAILURE) {
        printf("Couldn't authenticate user\n");
        return EXIT_SUCCESS;
    }

    //faz operações que quiser
    communicate();

    //sai e manda feedback ao server que vai sair

    return  0;
}

int get_server(char * ip_address, int port){
    assert(ip_address != NULL && port > 0);

    if((server_fd = create_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == EXIT_FAILURE) {
        perror("Couldn't obtain server\n");
        return EXIT_FAILURE;
    }

    initialize_socket((sockaddr_in *) &server, AF_INET, port, ip_address);

    return EXIT_SUCCESS;
}

int authenticate_user(){
    char password[SMALL_SIZE], buffer[SMALL_SIZE];

    if(get_input("username", username) == EXIT_FAILURE || get_input("password", password) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    //password_hash = hash(password);

    snprintf(buffer, sizeof(buffer), "%s %lu\n", username, password_hash);

    //udp_send_msg(server_fd, server, buffer, (size_t) strlen(buffer));

    //udp_receive_msg(server_fd, server, buffer, sizeof(buffer));

    /*if(strcasecmp(buffer, REJECTED) == 0) {
        return EXIT_FAILURE;
    }*/

    permissions = 101;

    //permissions = atoi(buffer);

    return EXIT_SUCCESS;
}

int communicate() {
    int i = 0;
    char options[LARGE_SIZE];

    while(i < 1) {
        comms_available(permissions, options);

        printf("\n%s\n", options);

        i++;
    }

    return EXIT_SUCCESS;
}

void comms_available(uint permissions_code, char * buffer) {
    int digit_base = 1;

    for(int i = 0; i < NUM_PERMITS; i++) {

        if(permissions_code / digit_base) {
            switch (i) {

                case (SERVER_COMMS):
                    buffer = append(buffer, "opcao 1\n");
                    break;
                case (P2P):
                    buffer = append(buffer, "opcao 2\n");
                    break;
                case (GROUP_COMMS):
                    buffer = append(buffer, "opcao 3\n");
                    break;
                default:
                    buffer = append(buffer, "opcao 4\n");
                    break;
            }
        }

        digit_base *= 10;
    }
}