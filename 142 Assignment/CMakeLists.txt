cmake_minimum_required(VERSION 3.26)
project(142_Assignment C)

set(CMAKE_C_STANDARD 11)

add_executable(142_Assignment
        colours.c
        game.c
        ghost.c
        map.c
        tests.cpp
        apsc142project.c)
