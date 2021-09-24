cmake_minimum_required(VERSION 3.19)
project(developer_joyofenergy_c)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_subdirectory(meter)
add_executable(firmware main.c)
target_link_libraries(firmware PRIVATE meter)

enable_testing()
add_subdirectory(test)
