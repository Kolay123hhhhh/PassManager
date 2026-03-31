cmake_minimum_required(VERSION 4.1)
project(PassManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(PassManager main.cpp)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")