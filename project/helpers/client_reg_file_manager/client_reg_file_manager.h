#ifndef MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H
#define MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H

#include "../../structs/user_tree_node_t.h"

extern void client_reg_reader_init(char * file_path, char * file_path_b);
extern user_tree_node_t * read_client_regs();
extern void write_client_regs(int mode);

#endif //MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H
