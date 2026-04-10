cmake_minimum_required(VERSION 4.1)
project(PassManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(PassManager main.cpp
        UpdaterClass.cpp
        UpdaterClass.h


)

add_executable(GuardManager GuardManager.cpp
        UpdaterClass.cpp
        UpdaterClass.h

)

add_library(utils utils.cpp utils.h)
add_library(sha256 sha256-cpp-master/sha256-cpp/sha256-cpp.cpp
            sha256-cpp-master/sha256-cpp/SHA256.h)

target_link_libraries(PassManager utils)
target_link_libraries(GuardManager utils)

target_link_libraries(PassManager sha256)

target_include_directories(PassManager PRIVATE sha256-cpp-master/sha256-cpp)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")