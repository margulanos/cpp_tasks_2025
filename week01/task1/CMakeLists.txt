cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER g++)

project(my_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)