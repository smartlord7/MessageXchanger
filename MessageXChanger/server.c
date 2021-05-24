#include "stdlib.h"
#include "server.h"
#include "helpers/client_reg_file_manager/client_reg_file_manager.h"

int main(void) {
    node_t * users = NULL;

    client_reg_reader_init(CLIENT_REG_FILE_PATH, CLIENT_REG_FILE_PATH_B);
    users = read_client_regs();

}
