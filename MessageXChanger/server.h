#ifndef MESSAGEXCHANGER_SERVER_H
#define MESSAGEXCHANGER_SERVER_H

#define NUM_MAX_TCP_CONNECTIONS 1
#define NUM_MAX_UDP_CLIENTS 20
#define UDP_ALL_TIMEOUT_SEC 300
#define USER_AUTHENTICATION_SUCCESS (-3)
#define USER_AUTHENTICATION_ERROR (-2)
#define USER_ALREADY_LOGGED (-1)

#define PASSWORD_HASH_OPT "DZ"
#define SERVER_TCP "[SERVER (TCP)] "
#define SERVER_UDP "[SERVER (UDP)] "
#define SERVER "[SERVER] "
#define ADMIN "[ADMIN %s] "
#define CLIENT "[CLIENT %s] "
#define ERROR "ERROR: "
#define CMD_LIST "LIST"
#define CMD_ADD "ADD"
#define CMD_DEL "DEL"
#define CMD_QUIT "QUIT"
#define SHOW_DEL "SHOWDEL"

#define ADD_DELIM " "
#define CLIENT_CONNECT SERVER_UDP "-> [%s] CONNECTED FROM PORT %d\n"
#define CLIENT_LOGGED_IN SERVER_UDP "-> [%s] LOGGED IN\n"
#define CLIENT_IP_NOT_ALLOWED SERVER_UDP "-> [%s] IP NOT ALLOWED FOR THE SPECIFIED USER\n"
#define CLIENT_WRONG_PASSWORD SERVER_UDP "-> [%s] WRONG PASSWORD\n"
#define CLIENT_REQ_USER_NOT_FOUND SERVER_UDP "-> [%s] REQUESTED USER %s NOT FOUND!\n"
#define CLIENT_REQ_USER_NOT_ONLINE SERVER_UDP "-> [%s] REQUESTED USER %s NOT ACTIVE!\n"
#define CLIENT_USER_NOT_FOUND SERVER_UDP "-> [%s] USER %s NOT FOUND\n"
#define CLIENT_USER_ALREADY_LOGGED SERVER_UDP "-> [%s] USER %s ALREADY LOGGED\n"
#define CLIENT_NO_SESSION SERVER_UDP "-> [%s] USER %s HAS NO SESSION\n"
#define SERVER_DISCONNECT_TIMEOUT SERVER_UDP "CLOSED DUE TO TIMEOUT\n"
#define ERROR_UDP_RCV "ERROR: COULDN'T RECEIVE UDP MESSAGE FROM FD %d!"
#define INVALID_CMD_ARGS "ERROR: INVALID ARGUMENTS USAGE! THEIR FORMAT MUST BE ./server <clients_port> <admin_port> <reg_file_path_txt> <reg_file_path_bin>"
#define SERVER_INIT SERVER "INITIALIZING SERVER...\n"
#define REG_LOADED SERVER "CLIENT REGISTRIES LOADED\n"
#define UDP_ONLINE SERVER_UDP "STATUS: ONLINE ON PORT %d\n"
#define TCP_ONLINE SERVER_TCP "STATUS: ONLINE ON PORT %d\n"
#define ADMIN_CONNECT SERVER_TCP "<- " ADMIN "CONNECTED\n"
#define ERROR_INVALID_CMD SERVER_TCP ERROR "INVALID COMMAND '%s'!\n"
#define ERROR_USERNAME_REPEAT SERVER_TCP ERROR "USER NAMED '%s' AlREADY EXISTS!\n"
#define TCP_ERROR_USER_NOT_FOUND SERVER_TCP ERROR "USER '%s' NOT FOUND!\n"
#define UDP_ERROR_USER_NOT_FOUND SERVER_UDP "USER NOT FOUND!\n"
#define ERROR_USER_IP_NOT_ALLOWED ERROR "IP NOT ALLOWED FOR THE SPECIFIED USER!\n"
#define ERROR_USER_NOT_LOGGED_IN ERROR "USER IS NOT LOGGED IN!\n"
#define ERROR_WRONG_PASSWORD ERROR "WRONG PASSWORD!\n"
#define ERROR_MISSING_PARAM SERVER_TCP ERROR "MISSING PARAM '%s'!\n"
#define SERVER_DISCONNECT SERVER_TCP "DISCONNECTED FROM SERVER!\n"
#define USER_CREATE SERVER_TCP "USER '%s' SUCCESSFULLY CREATED!\n"
#define USER_DELETE SERVER_TCP "USER '%s' SUCCESSFULLY DELETED!\n"
#define SHOW_DELETE_TOGGLE SERVER_TCP "<show delete> TOGGLED TO %d!\n"

#define send_resp() udp_send_msg(clients_fd, client_addr, (void *) &resp_msg, sizeof(response_msg_t))

#endif //MESSAGEXCHANGER_SERVER_H
