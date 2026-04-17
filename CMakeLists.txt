cmake_minimum_required(VERSION 3.16)
project(ThreadingDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent)

add_executable(threading_demo
    main.cpp
    worker_thread.cpp
    worker_thread.h
)

target_link_libraries(threading_demo
    PRIVATE Qt6::Core Qt6::Concurrent
)