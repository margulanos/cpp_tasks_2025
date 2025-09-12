cmake_minimum_required(VERSION 3.20)
project(trigonometry_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)


add_executable(main main.cpp)

target_sources(main PRIVATE
    src/ans_handler.cpp
    src/my_math.cpp)

target_include_directories(main PRIVATE include)