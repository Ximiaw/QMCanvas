// main.cpp
#include <QApplication>
#include "demo.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置应用程序属性
    app.setApplicationName("QMCanvas Demo");
    app.setOrganizationName("Ximiaw");

    Demo window;
    window.show();

    return app.exec();
}