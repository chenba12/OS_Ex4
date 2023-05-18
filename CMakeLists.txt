cmake_minimum_required(VERSION 3.25)
project(OS_Ex4)

set(CMAKE_CXX_STANDARD 17)

add_executable(OS_Ex4 Reactor.hpp Reactor.cpp Server.cpp Server.hpp Main.cpp)
