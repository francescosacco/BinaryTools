cmake_minimum_required(VERSION 3.10)

project(BinaryTools C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(source)
add_subdirectory(apps)
add_subdirectory(tests)
