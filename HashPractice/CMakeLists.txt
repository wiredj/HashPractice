cmake_minimum_required(VERSION 3.6)
project(HashPractice)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HashPractice.h)
add_executable(HashPractice ${SOURCE_FILES})