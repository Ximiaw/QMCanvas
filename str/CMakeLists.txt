project(QMCanvas)

qt_add_library(QMCanvas STATIC
        QMCanvasView/QMCanvasView.cpp
)

target_link_libraries(QMCanvas PRIVATE
        Qt::Widgets
)