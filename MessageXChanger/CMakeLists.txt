cmake_minimum_required(VERSION 3.19)
project(MessageXChanger C)

set(CMAKE_C_STANDARD 11)

add_executable(MessageXChanger main.c structs/user.h global.h "helpers/client_reg_file_manager/client_reg_file_ manager.c" "helpers/client_reg_file_manager/client_reg_file_ manager.h" structs/user.c main.h util/read_line/read_line.c util/read_line/read_line.h helpers/avl_tree/avl_tree.c helpers/avl_tree/avl_tree.h structs/avl_node.h)