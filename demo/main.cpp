// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include <QApplication>
#include "demo.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("QMCanvas Demo");
    app.setOrganizationName("Ximiaw");

    // 创建并显示主窗口
    DemoWindow window;
    window.show();

    return app.exec();
}