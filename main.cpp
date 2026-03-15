#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include <QPixmap>
#include <QWidget>
#include <QLayout>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMCanvasView cv;
    QPixmap pixmap(1000,1000);
    pixmap.fill(Qt::yellow);
    QMCanvasScene* scene = new QMCanvasScene(pixmap);
    cv.setCanvasScene(scene);
    cv.resize(500,500);
    //cv.setWheelMode(WheelMode::ZOOM);
    cv.canvasScene()->setFactor(1.2);
    cv.show();

    return QApplication::exec();
}