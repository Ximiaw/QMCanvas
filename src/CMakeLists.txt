project(QMCanvas)

file(GLOB SOURCES
        "QMCanvasScene/*.cpp"
        "QMCanvasScene/*.h"
        "QMCanvasView/*.cpp"
        "QMCanvasView/*.h"
        "QMDraw/*.cpp"
        "QMDraw/*.h"
)

qt_add_library(QMCanvas STATIC
        ${SOURCES}
)

target_link_libraries(QMCanvas PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(QMCanvas PUBLIC
    QMCanvasScene
    QMCanvasView
    QMDraw
)