#include "QMCanvasView.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMCanvasView qmcv;
    qmcv.resize(500, 500);
    qmcv.show();
    return QApplication::exec();
}