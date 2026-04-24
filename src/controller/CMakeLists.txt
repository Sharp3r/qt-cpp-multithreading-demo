add_library(controller
    ThreadController.cpp
    ThreadController.h
    TaskModel.cpp
    TaskModel.h
)

target_include_directories(controller
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(controller
    PUBLIC
        Qt6::Core
    PRIVATE
        worker
)