cmake_minimum_required(VERSION 3.19)
project(MessageXChanger C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pthread")

add_executable(Server server.c structs/user.h global.h helpers/client_reg_file_manager/client_reg_file_manager.c helpers/client_reg_file_manager/client_reg_file_manager.h structs/user.c main.h util/read_line/read_line.c util/read_line/read_line.h helpers/avl_tree/users_avl.c helpers/avl_tree/users_avl.h structs/avl_node.h server.h tcp_ip/tcp/tcp.c tcp_ip/tcp/tcp.h tcp_ip/udp/udp.c tcp_ip/udp/udp.h util/strings/strings.h util/strings/strings.c)
add_executable(Client structs/user.h global.h helpers/client_reg_file_manager/client_reg_file_manager.c helpers/client_reg_file_manager/client_reg_file_manager.h structs/user.c main.h util/read_line/read_line.c util/read_line/read_line.h helpers/avl_tree/users_avl.c helpers/avl_tree/users_avl.h structs/avl_node.h client.c client.h tcp_ip/tcp/tcp.c tcp_ip/tcp/tcp.h tcp_ip/udp/udp.c tcp_ip/udp/udp.h util/strings/strings.c util/strings/strings.h util/numbers/numbers.c util/numbers/numbers.h)