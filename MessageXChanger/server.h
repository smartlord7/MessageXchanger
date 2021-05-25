#ifndef MESSAGEXCHANGER_SERVER_H
#define MESSAGEXCHANGER_SERVER_H

#define NUM_MAX_TCP_CONNECTIONS 1
#define NUM_MAX_UDP_CLIENTS 20
#define UDP_ALL_TIMEOUT_SEC 60

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
#define CLIENT_CONNECT SERVER_UDP "[%s] CONNECTED\n"
#define SERVER_DISCONNECT_TIMEOUT SERVER_UDP "CLOSED DUE TO TIMEOUT\n"
#define ERROR_UDP_RCV "ERROR: COULDN'T RECEIVE UDP MESSAGE FROM FD %d!"
#define INVALID_CMD_ARGS "ERROR: INVALID ARGUMENTS USAGE! THEIR FORMAT MUST BE ./server <clients_port> <admin_port> <reg_file_path_txt> <reg_file_path_bin>"
#define SERVER_INIT SERVER "INITIALIZING SERVER...\n"
#define REG_LOADED SERVER "CLIENT REGISTRIES LOADED\n"
#define UDP_ONLINE SERVER_UDP "STATUS: ONLINE\n"
#define TCP_ONLINE SERVER_TCP "STATUS: ONLINE\n"
#define ADMIN_CONNECT ADMIN "CONNECTED\n"
#define ERROR_INVALID_CMD SERVER_TCP ERROR "INVALID COMMAND '%s'!\n"
#define ERROR_USERNAME_REPEAT SERVER_TCP ERROR "USER NAMED '%s' AlREADY EXISTS!\n"
#define TCP_ERROR_USER_NOT_FOUND SERVER_TCP ERROR "USER '%s' NOT FOUND!\n"
#define UDP_ERROR_USER_NOT_FOUND SERVER_UDP "USER NOT FOUND!\n"
#define ERROR_WRONG_PASSWORD "ERROR: WRONG PASSWORD!\n"
#define ERROR_MISSING_PARAM SERVER_TCP ERROR "MISSING PARAM '%s'!\n"
#define SERVER_DISCONNECT SERVER_TCP "DISCONNECTED FROM SERVER!\n"
#define USER_CREATE SERVER_TCP "USER '%s' SUCCESSFULLY CREATED!\n"
#define USER_DELETE SERVER_TCP "USER '%s' SUCCESSFULLY DELETED!\n"
#define SHOW_DELETE_TOGGLE SERVER_TCP "<show delete> TOGGLED TO %d!\n"

#endif //MESSAGEXCHANGER_SERVER_H
