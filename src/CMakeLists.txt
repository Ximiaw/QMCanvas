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
        QMCanvasScene/LayerManager.h
        QMCanvasScene/Layer.h
        QMCanvasScene/AbstractLayer.h
        QMCanvasScene/AbstractLayer.cpp
        QMCanvasScene/Layer.cpp
        QMCanvasScene/LayerManager.cpp
        QMCanvasScene/Location.cpp
        QMCanvasScene/Location.h
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