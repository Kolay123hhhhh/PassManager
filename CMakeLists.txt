cmake_minimum_required(VERSION 4.1)
project(PassManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(PassManager src/main.cpp
        src/UpdateSystem/UpdaterClass.cpp
        src/UpdateSystem/UpdaterClass.h


)

add_executable(GuardManager src/Security/GuardManager.cpp
        src/UpdateSystem/UpdaterClass.cpp
        src/UpdateSystem/UpdaterClass.h

)

add_executable(Updater src/Updater.cpp


)

add_library(utils src/utils.cpp src/utils.h)
add_library(sha256 src/sha256-cpp-master/sha256-cpp/sha256-cpp.cpp
        src/sha256-cpp-master/sha256-cpp/SHA256.h)

target_link_libraries(PassManager utils)
target_link_libraries(PassManager urlmon)
target_link_libraries(GuardManager utils)
target_link_libraries(Updater utils)

target_link_libraries(PassManager sha256)

target_include_directories(PassManager PRIVATE src/sha256-cpp-master/sha256-cpp)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")