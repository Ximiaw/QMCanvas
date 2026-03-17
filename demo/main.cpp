// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include "demo.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    app.setApplicationName("QMCanvas Studio");
    app.setOrganizationName("Ximiaw");
    app.setApplicationDisplayName("QMCanvas");

    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(43, 43, 43));
    darkPalette.setColor(QPalette::WindowText, QColor(204, 204, 204));
    darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(43, 43, 43));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(60, 60, 60));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, QColor(204, 204, 204));
    darkPalette.setColor(QPalette::Button, QColor(60, 60, 60));
    darkPalette.setColor(QPalette::ButtonText, QColor(204, 204, 204));
    darkPalette.setColor(QPalette::BrightText, Qt::white);
    darkPalette.setColor(QPalette::Highlight, QColor(0, 120, 212));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));

    app.setPalette(darkPalette);

    app.setStyleSheet(R"(
        QMainWindow { background-color: #2b2b2b; }
        QDockWidget::title { background: #3c3c3c; padding: 8px; color: #cccccc;
                             font-weight: bold; border: none; border-bottom: 2px solid #0078d4; }
        QMenuBar { background-color: #2b2b2b; color: #cccccc; border-bottom: 1px solid #3c3c3c; padding: 2px; }
        QMenuBar::item:selected { background-color: #4a4a4a; border-radius: 4px; }
        QMenu { background-color: #2b2b2b; color: #cccccc; border: 1px solid #3c3c3c; padding: 4px; }
        QMenu::item:selected { background-color: #094771; color: white; border-radius: 3px; }
        QStatusBar { background-color: #2b2b2b; color: #cccccc; border-top: 1px solid #3c3c3c; }
        QGroupBox { font-weight: bold; border: 1px solid #555; border-radius: 6px;
                    margin-top: 12px; padding-top: 10px; color: #cccccc; background: #333; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 8px; color: #0078d4; }
        QPushButton { background-color: #0e639c; color: white; border: none; border-radius: 4px;
                      padding: 6px 12px; font-weight: bold; min-width: 60px; }
        QPushButton:hover { background-color: #1177bb; }
        QPushButton:pressed { background-color: #094771; }
        QPushButton:disabled { background-color: #4a4a4a; color: #888; }
        QSlider::groove:horizontal { height: 6px; background: #555; border-radius: 3px; }
        QSlider::handle:horizontal { background: #0078d4; border: none; width: 16px; margin: -5px 0; border-radius: 8px; }
        QSlider::sub-page:horizontal { background: #0078d4; border-radius: 3px; }
        QSpinBox, QDoubleSpinBox, QComboBox { background: #3c3c3c; color: #ccc;
                                              border: 1px solid #555; border-radius: 4px; padding: 4px; }
        QListWidget { background: #2b2b2b; color: #ccc; border: 1px solid #3c3c3c; outline: none; }
        QListWidget::item { padding: 6px; border-bottom: 1px solid #3c3c3c; }
        QListWidget::item:selected { background: #094771; color: white; }
        QCheckBox { color: #ccc; spacing: 8px; }
        QLabel { color: #cccccc; }
    )");

    QFont font("Segoe UI", 9);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    QMCanvasStudio window;
    window.show();

    return app.exec();
}