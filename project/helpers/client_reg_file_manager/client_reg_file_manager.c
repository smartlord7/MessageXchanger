#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "../../structs/user.h"
#include "../../util/read_line/read_line.h"
#include "../trees/user_tree.h"
#include "client_reg_file_manager.h"

static char * client_reg_file_path = NULL, * client_reg_file_path_b = NULL;
static int current_line = -1;

static user_tree_node_t * read_client_reg_file(FILE * reg_file);
static user_tree_node_t * read_client_reg_file_b(FILE * reg_file);

void client_reg_reader_init(char * file_path, char * file_path_b) {
    assert(file_path != NULL);

    client_reg_file_path = file_path;
    client_reg_file_path_b = file_path_b;
}

user_tree_node_t * read_client_regs() {
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

    write_users_b(reg_file, mode);

    assert(fclose(reg_file) != EOF);
}

user_tree_node_t * read_client_reg_file(FILE * reg_file) {
    assert(reg_file != NULL);

    reg_file = fopen(client_reg_file_path, "r");

    assert(reg_file != NULL);

    char buffer[LARGEST_SIZE];

    while (read_line(buffer, reg_file, LARGEST_SIZE) != EOF) {
        current_line++;

        insert_user(parse_user(buffer));
    }

    assert(fclose(reg_file) != EOF);

    return get_root();
}

user_tree_node_t * read_client_reg_file_b(FILE * reg_file) {
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
