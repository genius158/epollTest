cmake_minimum_required(VERSION 3.4.1)


aux_source_directory(src/main/cpp SRC_LIST)

add_library(
        epoll_test
        SHARED
        ${SRC_LIST})

find_library(log-lib log)

target_link_libraries(
        epoll_test
        ${log-lib})