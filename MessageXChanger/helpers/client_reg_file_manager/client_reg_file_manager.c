#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "../../global.h"
#include "../../structs/user.h"
#include "../../util/read_line/read_line.h"
#include "../avl_tree/users_avl.h"
#include "client_reg_file_manager.h"

static char * client_reg_file_path = NULL, * client_reg_file_path_b = NULL;
static int current_line = -1;

static node_t * read_client_reg_file(FILE * reg_file);
static node_t * read_client_reg_file_b(FILE * reg_file);

void client_reg_reader_init(char * file_path, char * file_path_b) {
    assert(file_path != NULL);

    client_reg_file_path = file_path;
    client_reg_file_path_b = file_path_b;
}

node_t * read_client_regs() {
    assert(client_reg_file_path != NULL && client_reg_file_path_b != NULL);
    FILE * reg_file = NULL;

    reg_file = fopen(client_reg_file_path_b, "rb");

    if (reg_file != NULL) {
        return read_client_reg_file_b(reg_file);
    } else {
        reg_file = fopen(client_reg_file_path, "r");
        return read_client_reg_file(reg_file);
    }
}

void write_client_regs(int mode) {
    assert(client_reg_file_path_b != NULL);

    FILE * reg_file = NULL;

    reg_file = fopen(client_reg_file_path_b, "wb");
    assert(reg_file != NULL);

    write_users(reg_file, mode);

    assert(fclose(reg_file) != EOF);
}

node_t * read_client_reg_file(FILE * reg_file) {
    assert(reg_file != NULL);

    reg_file = fopen(client_reg_file_path, "r");

    assert(reg_file != NULL);

    char buffer[LARGEST_SIZE], * token = NULL,
         user_name[LARGE_SIZE], password_hash[XLARGE_SIZE];
    int host_ip;
    uint has_client_server_conn, has_p2p_conn, has_group;
    user_t * user = NULL;

    while (read_line(buffer, reg_file, LARGEST_SIZE) != EOF) {
        current_line++;

        token = strtok(buffer, FIELD_DELIM);
        assert(token != NULL);
        strcpy(user_name, token);

        token = strtok(NULL, FIELD_DELIM);
        assert(token != NULL);
        strcpy(password_hash, token);

        token = strtok(NULL, FIELD_DELIM);
        assert(token != NULL);
        host_ip = atoi(token);

        token = strtok(NULL, FIELD_DELIM);
        assert(token != NULL);
        has_client_server_conn = atoi(token);

        token = strtok(NULL, FIELD_DELIM);
        assert(token != NULL);
        has_p2p_conn = atoi(token);

        token = strtok(NULL, FIELD_DELIM);
        assert(token != NULL);
        has_group = atoi(token);

        user = new_user(user_name, password_hash, host_ip, has_client_server_conn, has_p2p_conn, has_group);

        insert_user(user);
    }

    assert(fclose(reg_file) != EOF);

    return get_root();
}

node_t * read_client_reg_file_b(FILE * reg_file) {
    assert(reg_file != NULL);

    reg_file = fopen(client_reg_file_path_b, "rb");

    user_t * user = NULL;
    size_t n;

    while (true) {
        user = (user_t *) malloc(sizeof(user_t));

        n = fread((void *) user, sizeof(user_t), 1, reg_file);

        if (feof(reg_file)) {
            break;
        }

        assert(n > 0);

        insert_user(user);

    }

    assert(fclose(reg_file) != EOF);

    return get_root();
}