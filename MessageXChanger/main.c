#include "helpers/client_reg_file_manager/client_reg_file_manager.h"
#include "main.h"

int main() {
    client_reg_reader_init(CLIENT_REG_FILE_PATH, CLIENT_REG_FILE_PATH_B);
    read_client_regs();
}
