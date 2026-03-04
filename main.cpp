#include "View.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    View qmcv;
    qmcv.resize(500, 500);
    qmcv.show();
    return QApplication::exec();
}