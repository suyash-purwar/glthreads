cmake_minimum_required(VERSION 3.28)
project(glthreads C)

set(CMAKE_C_STANDARD 11)

add_executable(glthreads src/main.c src/glthread.c)

target_include_directories(glthreads PRIVATE include)
