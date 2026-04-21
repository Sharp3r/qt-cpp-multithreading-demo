add_library(worker
    worker_thread.cpp
    worker_thread.h
)

target_include_directories(worker
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(worker
    PUBLIC
        Qt6::Core
)