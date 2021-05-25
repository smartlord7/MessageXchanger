cmake_minimum_required(VERSION 3.19)
project(MessageXChanger C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pthread")

add_executable(Server server.c server.h structs/user.h global.h helpers/client_reg_file_manager/client_reg_file_manager.c helpers/client_reg_file_manager/client_reg_file_manager.h structs/user.c util/read_line/read_line.c util/read_line/read_line.h helpers/trees/user_tree.c helpers/trees/user_tree.h structs/user_tree_node_t.h server.h tcp_ip/tcp/tcp.c tcp_ip/tcp/tcp.h tcp_ip/udp/udp.c tcp_ip/udp/udp.h util/strings/strings.h util/strings/strings.c structs/user_session.h helpers/trees/session_tree.c helpers/trees/session_tree.h structs/session_tree_node_t.h structs/user_session.h structs/response_msg_t.h structs/handshake_t.h util/msg_queue/msg_queue.c util/msg_queue/msg_queue.h)
target_link_libraries(Server -lcrypt)
add_executable(Client structs/user.h global.h helpers/client_reg_file_manager/client_reg_file_manager.c helpers/client_reg_file_manager/client_reg_file_manager.h structs/user.c util/read_line/read_line.c util/read_line/read_line.h helpers/trees/user_tree.c helpers/trees/user_tree.h structs/user_tree_node_t.h client.c client.h tcp_ip/tcp/tcp.c tcp_ip/tcp/tcp.h tcp_ip/udp/udp.c tcp_ip/udp/udp.h util/strings/strings.c util/strings/strings.h util/numbers/numbers.c util/numbers/numbers.h structs/session_tree_node_t.h structs/user_session.h structs/request_msg.h)
target_link_libraries(Client -lcrypt)