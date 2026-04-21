add_subdirectory(controller)
add_subdirectory(worker)
add_subdirectory(ui)

target_sources(threading_demo
    PRIVATE
        main.cpp
)

target_link_libraries(threading_demo
    PRIVATE
        Qt6::Quick
)