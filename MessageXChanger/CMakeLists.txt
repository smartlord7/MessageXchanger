cmake_minimum_required(VERSION 3.19)
project(MessageXChanger C)

set(CMAKE_C_STANDARD 11)

add_executable(MessageXChanger main.c structs/user.h global.h helpers/client_reg_reader/client_reg_reader.c helpers/client_reg_reader/client_reg_reader.h structs/user.c)