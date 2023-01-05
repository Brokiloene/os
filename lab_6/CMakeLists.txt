cmake_minimum_required(VERSION 3.22.1)
project(os_lab6)

add_executable(manage_node ./src/manage_node.c ./src/lab6_utils.c ./src/lab6_utils.h ./src/list.c ./src/list.h)
add_executable(computing_node ./src/computing_node.c ./src/lab6_utils.c ./src/lab6_utils.h ./src/list.c ./src/list.h)

target_link_libraries(manage_node PRIVATE libzmq.so)
target_link_libraries(computing_node PRIVATE libzmq.so)