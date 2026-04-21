cmake_minimum_required(VERSION 3.16)
project(ThreadingDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Gui Quick)

add_executable(threading_demo
    main.cpp
    worker_thread.cpp
    worker_thread.h
    ThreadController.cpp
    ThreadController.h
    resources.qrc
)

target_link_libraries(threading_demo
    PRIVATE Qt6::Core Qt6::Concurrent Qt6::Gui Qt6::Quick
)

add_custom_command(TARGET threading_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/test.txt
        $<TARGET_FILE_DIR:threading_demo>
)