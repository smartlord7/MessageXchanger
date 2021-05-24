#include "assert.h"
#include <stdio.h>
#include "client.h"

static unsigned long hash(const char *str);
static int get_server(char * ip_address, int port);
static int authenticate_user();
static int communicate();
static void comms_available(uint permissions_code, char * buffer);
//static int mediated_chat();

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
    int i = 0, option;
    char options[LARGE_SIZE], input[SMALL_SIZE];

    while(true) {

        comms_available(permissions, options);
        printf("%s", options);

        fgets(input, sizeof(input), stdin);
        if(!is_numeric(input, strlen(input)) || to_float(input, (float *) &option) < 1) {
            option = -1;
        }

        switch (option) {
            case (SERVER_COMMS):
                //mediated_chat();
                printf("comunicando com outro client com servidor\n");
                break;
            case (P2P):
                //non_mediated_chat();
                printf("comunicando com outro cliente sem server\n");
                break;
            case (GROUP_COMMS):
                //group_chat();
                printf("group chat\n");
                break;
            default:
                return EXIT_SUCCESS;
        }

        i++;
    }
}

void comms_available(uint permissions_code, char * buffer) {
    int digit_base = 1;
    char aux[SMALL_SIZE];

    snprintf(aux, sizeof(aux), "\n---- Available types of chat ----\n\n");
    buffer = append(buffer, aux);

    for(int i = 0; i < NUM_PERMITS; i++) {

        if(permissions_code / digit_base) {
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
        digit_base *= 10;
    }
    snprintf(aux, sizeof(aux), "\nPress any option to end the session.\nOption: ");
    buffer = append(buffer, aux);
}