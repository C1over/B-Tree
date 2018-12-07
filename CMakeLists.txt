cmake_minimum_required(VERSION 3.12)
project(b_tree)

set(CMAKE_CXX_STANDARD 14)

add_executable(b_tree main.cpp bTree.h bTree.cpp)