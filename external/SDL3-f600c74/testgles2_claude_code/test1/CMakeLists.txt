cmake_minimum_required(VERSION 3.10)
project(red_triangle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

find_package(SDL3 REQUIRED CONFIG)

add_executable(red_triangle main.c)
target_link_libraries(red_triangle SDL3::SDL3 GLESv2)
