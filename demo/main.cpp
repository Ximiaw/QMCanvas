// main.cpp
#include <QApplication>
#include "demo.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("QMCanvasDemo");
    app.setOrganizationName("Ximiaw");

    // 创建并显示主窗口
    DemoWindow window;
    window.show();

    return app.exec();
}