qt_add_library(demo_ui STATIC)

qt_add_qml_module(demo_ui
    URI ThreadingDemo.UI
    VERSION 1.0
    QML_FILES
        Main.qml
        TaskCard.qml
)

target_link_libraries(demo_ui
    PRIVATE
        controller
)