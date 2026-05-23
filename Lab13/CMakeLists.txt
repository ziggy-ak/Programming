cmake_minimum_required(VERSION 3.10)

project(Lab12)

include_directories(include)

option(BUILD_SHARED_LIBS "Создать динамическую библиотеку" OFF)

add_library(game src/game.c)

add_executable(program src/main.c)

target_link_libraries(program game)