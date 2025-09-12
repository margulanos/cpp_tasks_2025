cmake_minimum_required(VERSION 3.20)

project(ASCII-Tetris LANGUAGES C)

add_executable(tetris tetris.c main.c)

target_include_directories(tetris PRIVATE .)