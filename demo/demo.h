// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef TEST_DOME_H
#define TEST_DOME_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include "QMCanvasView.h"
#include "QMCanvasScene.h"

class Demo : public QMainWindow {
    Q_OBJECT
public:
    explicit Demo(QWidget *parent = nullptr);
    ~Demo();

private slots:
    void onToolChanged(int index);
    void onColorChanged();
    void onWidthChanged(int width);
    void onAddLayer();
    void onDeleteLayer();
    void onLayerItemChanged(QListWidgetItem *item);
    void onLayerSelectionChanged();
    void onUndo();
    void onRedo();
    void onClearCanvas();
    void onSaveImage();
    void updateLayerList();

private:
    void setupUI();
    void setupConnections();

    QMCanvasView *canvasView_;
    QMCanvasScene *scene_;

    // 工具栏控件
    QComboBox *toolCombo_;
    QPushButton *colorBtn_;
    QSpinBox *widthSpin_;

    // 图层面板
    QListWidget *layerList_;
    QPushButton *addLayerBtn_;
    QPushButton *delLayerBtn_;

    // 操作按钮
    QPushButton *undoBtn_;
    QPushButton *redoBtn_;
    QPushButton *clearBtn_;
    QPushButton *saveBtn_;

    QColor currentColor_;
    int currentLayerId_;
};

#endif //TEST_DOME_H