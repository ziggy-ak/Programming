# Лабораторная работа №13: Системы сборки Make и CMake

**Цель:** освоить Make и CMake.

**Выполнение:**
Структура проекта: src/ - исходники, include/ - заголовки.

Makefile:
CC = gcc
CFLAGS = -Wall -Iinclude
all: program
program: main.o smarthome.o
clean: rm -f *.o program

CMakeLists.txt:
cmake_minimum_required(VERSION 3.10)
project(SmartHome)
add_executable(program main.c src/smarthome.c)
target_include_directories(program PRIVATE include)

Опция -D: cmake -DBUILD_SHARED=ON ..

**Вывод:** освоены Make и CMake для автоматизации сборки.