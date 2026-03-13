project(QMCanvas)

qt_add_library(QMCanvas STATIC
        QMCanvasView/View.cpp
        QMCanvasView/View.h
        QMCanvasScene/QMCanvasScene.cpp
        QMCanvasScene/QMCanvasScene.h
        QMCanvasView/Viewport.cpp
        QMCanvasView/Viewport.h
        QMCanvasView/QMCanvasView.cpp
        QMCanvasView/QMCanvasView.h
        QMDraw/QMDrawObject.h
        QMCanvasScene/LayerManager.cpp
        QMCanvasScene/LayerManager.h
        QMCanvasScene/Layer.cpp
        QMCanvasScene/Layer.h
)

target_link_libraries(QMCanvas PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(QMCanvas PUBLIC
    QMCanvasScene
    QMCanvasView
    QMDraw
)