add_library(task_queue
    task_queue.cpp
    task_queue.h
)

target_include_directories(task_queue
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(task_queue
    PUBLIC
        Qt6::Core
)
