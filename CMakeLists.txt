cmake_minimum_required(VERSION 3.13)
project(myCalc)

set(CMAKE_CXX_STANDARD 11)

add_executable(myCalc news/scanner.c news/parse.c news/state.h)