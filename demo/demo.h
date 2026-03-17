// demo.h
// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QColorDialog>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QInputDialog>
#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include "QMDrawPen.h"
#include "QMDrawRect.h"
#include "QMDrawPixmap.h"

class DemoWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DemoWindow(QWidget* parent = nullptr);
    ~DemoWindow() override;

private slots:
    // 工具切换
    void onPenToolSelected();
    void onRectToolSelected();
    void onPixmapToolSelected();

    // 颜色设置
    void onColorChanged();

    // 线宽设置
    void onWidthChanged(int width);

    // 图层管理
    void onNewLayer();
    void onSwitchLayer();
    void onToggleLayerHide();
    void onSwapLayers();

    // 撤销重做
    void onUndo();
    void onRedo();

    // 滚轮模式
    void onWheelModeChanged(int index);

    // 缩放控制
    void onZoomIn();
    void onZoomOut();
    void onResetZoom();

private:
    void setupUI();
    void setupTools();
    void setupLayerControls();
    void updateLayerInfo();
    void createNewDrawObject();

    QMCanvasView* canvasView_;
    QMCanvasScene* scene_;

    // 工具栏控件
    QPushButton* btnPen_;
    QPushButton* btnRect_;
    QPushButton* btnPixmap_;
    QPushButton* btnColor_;
    QSpinBox* spinWidth_;
    QComboBox* comboWheelMode_;
    QComboBox* comboLayerSelect_;
    QLabel* lblLayerInfo_;

    // 当前工具状态
    QColor currentColor_;
    int currentWidth_;
    enum class ToolType { Pen, Rect, Pixmap };
    ToolType currentTool_;
    bool fillRect_;

    // 图层计数
    int layerCount_;
};

#endif // DEMO_H