# by xtr4ng3 [xtr4ng3-oxlgr (xtr4ng3)](https://github.com/xtr4ng3-oxlgr)
cmake_minimum_required(VERSION 3.15)
project(larvatus CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(larvatus
    src/main.cpp
    src/listener.cpp
    src/service_profile.cpp
    src/logger.cpp
    src/platform.cpp
)

target_include_directories(larvatus PRIVATE include)

if(WIN32)
    target_link_libraries(larvatus PRIVATE ws2_32)
endif()

find_package(Threads REQUIRED)
target_link_libraries(larvatus PRIVATE Threads::Threads)
