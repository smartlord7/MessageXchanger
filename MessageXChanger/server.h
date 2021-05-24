#ifndef MESSAGEXCHANGER_SERVER_H
#define MESSAGEXCHANGER_SERVER_H

#define CLIENT_REG_FILE_PATH "client_reg.txt"
#define CLIENT_REG_FILE_PATH_B "client_reg.bin"

#define SERVER_TCP_PORT 9000
#define SERVER_UDP_PORT 4500
#define NUM_MAX_TCP_CONNECTIONS 1


#define SERVER_TCP "[SERVER (TCP)] "
#define SERVER_UDP "[SERVER (UDP)] "
#define ADMIN "[ADMIN] "
#define ERROR "ERROR: "
#define CMD_LIST "LIST"
#define CMD_ADD "ADD"
#define CMD_DEL "DEL"
#define CMD_QUIT "QUIT"
#define SHOW_DEL "SHOWDEL"

#define ADD_DELIM " "
#define ADMIN_CONNECT ADMIN "CONNECTED FROM %s!\n"
#define ERROR_INVALID_CMD SERVER_TCP ERROR "INVALID COMMAND '%s'!\n"
#define ERROR_USERNAME_REPEAT SERVER_TCP ERROR "USER NAMED '%s' AlREADY EXISTS!\n"
#define ERROR_USER_NOT_FOUND SERVER_TCP ERROR "USER '%s' NOT FOUND!\n"
#define ERROR_MISSING_PARAM SERVER_TCP ERROR "MISSING PARAM '%s'!\n"
#define SERVER_DISCONNECT SERVER_TCP "DISCONNECTED FROM SERVER!\n"
#define USER_CREATE SERVER_TCP "USER '%s' SUCCESSFULLY CREATED!\n"
#define USER_DELETE SERVER_TCP "USER '%s' SUCCESSFULLY DELETED!\n"
#define SHOW_DELETE_TOGGLE SERVER_TCP "<show delete> TOGGLED TO %d!"

#endif //MESSAGEXCHANGER_SERVER_H
