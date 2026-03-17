// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef TEST_DOME_H
#define TEST_DOME_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>  // 修复：添加 QStatusBar 头文件
#include <QShortcut>

#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include "QMDrawPen.h"
#include "QMDrawRect.h"

class DemoWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DemoWindow(QWidget *parent = nullptr);
    ~DemoWindow() override;

private slots:
    void onToolChanged(int index);
    void onColorChanged();
    void onWidthChanged(int width);
    void onFillChanged(bool fill);
    void onNewLayer();
    void onClearLayer();
    void onSaveImage();
    void onUndo();
    void onRedo();
    void onZoomIn();
    void onZoomOut();

private:
    void setupUI();
    void setupConnections();
    void updateStatus() const;  // 修复：可被设为 const

    QMCanvasView *canvasView_ = nullptr;
    QMCanvasScene *scene_ = nullptr;

    QComboBox *toolCombo_ = nullptr;
    QSpinBox *widthSpin_ = nullptr;
    QPushButton *colorBtn_ = nullptr;
    QColor currentColor_ = Qt::black;
    int currentWidth_ = 3;
    bool fillRect_ = false;

    QLabel *statusLabel_ = nullptr;
    QLabel *zoomLabel_ = nullptr;
};

#endif //TEST_DOME_H