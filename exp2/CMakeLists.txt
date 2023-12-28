cmake_minimum_required(VERSION 3.27)
project(exp2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(exp2
        exp2.cpp
        StringAnalysis.cpp
        StringAnalysis.h
        grammar.txt
        grammar.cpp
        grammar.h
        LL1Parser.cpp
        LL1Parser.h)
