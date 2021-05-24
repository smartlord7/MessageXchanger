#include "helpers/client_reg_file_manager/client_reg_file_manager.h"
#include "main.h"
#include "helpers/avl_tree/users_avl.h"

int main() {
    client_reg_reader_init(CLIENT_REG_FILE_PATH, CLIENT_REG_FILE_PATH_B);
    read_client_regs();
    node_t *users = get_root();

    delete_user("joaoartur");
    print_users(users);
    write_client_regs(SHOW_DELETED);

}