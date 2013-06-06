cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "-std=c++11")

project(satori)

if(DEBUG)
    add_definitions(-DDEBUG)
endif(DEBUG)

add_executable(satori RegularLanguage.cpp satori.cxx)

