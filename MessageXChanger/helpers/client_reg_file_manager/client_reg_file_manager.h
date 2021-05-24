#ifndef MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H
#define MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H

#define FIELD_DELIM ","

extern void client_reg_reader_init(char * file_path, char * file_path_b);
extern void read_client_regs();
extern void write_client_regs();

#endif //MESSAGEXCHANGER_CLIENT_REG_FILE_MANAGER_H
