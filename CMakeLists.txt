cmake_minimum_required(VERSION 3.16)
project(ThreadingDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Gui Quick Qml)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
qt_standard_project_setup()

qt_add_executable(threading_demo)

add_subdirectory(src)

target_link_libraries(threading_demo
    PRIVATE
        Qt6::Core
        Qt6::Concurrent
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        controller
        worker
        demo_uiplugin
)

add_custom_command(TARGET threading_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/test.txt
        $<TARGET_FILE_DIR:threading_demo>
)