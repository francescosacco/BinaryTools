# Define the minimum version of the CMake to be used in this project.
cmake_minimum_required(VERSION 3.10)

# Name of the project and language used.
project(BinaryTools C)

# Standard of the language used.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add subdirectories with the source of the code, applications and unit tests.
add_subdirectory(source)
add_subdirectory(apps)
add_subdirectory(tests)
