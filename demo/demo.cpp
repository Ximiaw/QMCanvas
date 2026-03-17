// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "demo.h"

// demo.cpp
#include "demo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "QMDrawPen.h"
#include "QMDrawRect.h"
#include "QMDrawPixmap.h"

Demo::Demo(QWidget *parent)
    : QMainWindow(parent)
    , canvasView_(nullptr)
    , scene_(nullptr)
    , currentColor_(Qt::black)
    , currentLayerId_(0) {

    // 创建默认场景（500x500 透明画布）
    QPixmap initialPixmap(800, 600);
    initialPixmap.fill(QColor(255, 255, 255));

    scene_ = new QMCanvasScene(initialPixmap, this);
    canvasView_ = new QMCanvasView(scene_, this);

    setupUI();
    setupConnections();

    // 初始化第一个绘图工具（画笔）
    onToolChanged(0);

    setWindowTitle("QMCanvas 绘图演示");
    resize(1200, 800);
}

Demo::~Demo() = default;

void Demo::setupUI() {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    // 左侧工具栏
    auto *leftPanel = new QGroupBox("工具箱", this);
    auto *leftLayout = new QVBoxLayout(leftPanel);

    // 工具选择
    auto *toolLabel = new QLabel("选择工具:", this);
    toolCombo_ = new QComboBox(this);
    toolCombo_->addItem("画笔", 0);
    toolCombo_->addItem("矩形", 1);
    toolCombo_->addItem("图片", 2);
    leftLayout->addWidget(toolLabel);
    leftLayout->addWidget(toolCombo_);

    // 颜色选择
    auto *colorLabel = new QLabel("颜色:", this);
    colorBtn_ = new QPushButton(this);
    colorBtn_->setStyleSheet(QString("background-color: %1; min-height: 30px;").arg(currentColor_.name()));
    leftLayout->addWidget(colorLabel);
    leftLayout->addWidget(colorBtn_);

    // 线宽
    auto *widthLabel = new QLabel("线宽:", this);
    widthSpin_ = new QSpinBox(this);
    widthSpin_->setRange(1, 50);
    widthSpin_->setValue(3);
    leftLayout->addWidget(widthLabel);
    leftLayout->addWidget(widthSpin_);

    leftLayout->addStretch();

    // 中央画布
    auto *canvasContainer = new QWidget(this);
    auto *canvasLayout = new QVBoxLayout(canvasContainer);
    canvasLayout->addWidget(canvasView_);

    // 右侧图层面板
    auto *rightPanel = new QGroupBox("图层管理", this);
    auto *rightLayout = new QVBoxLayout(rightPanel);

    // 图层列表
    layerList_ = new QListWidget(this);
    layerList_->setMaximumHeight(300);
    rightLayout->addWidget(layerList_);

    // 图层按钮
    auto *layerBtnLayout = new QHBoxLayout();
    addLayerBtn_ = new QPushButton("添加", this);
    delLayerBtn_ = new QPushButton("删除", this);
    layerBtnLayout->addWidget(addLayerBtn_);
    layerBtnLayout->addWidget(delLayerBtn_);
    rightLayout->addLayout(layerBtnLayout);

    rightLayout->addSpacing(20);

    // 操作按钮
    undoBtn_ = new QPushButton("撤销 (Ctrl+Z)", this);
    redoBtn_ = new QPushButton("重做 (Ctrl+Y)", this);
    clearBtn_ = new QPushButton("清空当前层", this);
    saveBtn_ = new QPushButton("保存图片", this);

    rightLayout->addWidget(undoBtn_);
    rightLayout->addWidget(redoBtn_);
    rightLayout->addWidget(clearBtn_);
    rightLayout->addWidget(saveBtn_);
    rightLayout->addStretch();

    // 添加到主布局
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(canvasContainer, 5);
    mainLayout->addWidget(rightPanel, 1);

    setCentralWidget(centralWidget);

    // 初始化图层列表
    updateLayerList();
}

void Demo::setupConnections() {
    connect(toolCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Demo::onToolChanged);
    connect(colorBtn_, &QPushButton::clicked, this, &Demo::onColorChanged);
    connect(widthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Demo::onWidthChanged);

    connect(addLayerBtn_, &QPushButton::clicked, this, &Demo::onAddLayer);
    connect(delLayerBtn_, &QPushButton::clicked, this, &Demo::onDeleteLayer);
    connect(layerList_, &QListWidget::itemChanged, this, &Demo::onLayerItemChanged);
    connect(layerList_, &QListWidget::currentRowChanged, this, &Demo::onLayerSelectionChanged);

    connect(undoBtn_, &QPushButton::clicked, this, &Demo::onUndo);
    connect(redoBtn_, &QPushButton::clicked, this, &Demo::onRedo);
    connect(clearBtn_, &QPushButton::clicked, this, &Demo::onClearCanvas);
    connect(saveBtn_, &QPushButton::clicked, this, &Demo::onSaveImage);
}

void Demo::onToolChanged(int index) {
    if (!scene_ || !scene_->activeLayer()) return;

    QMDrawObject *drawObject = nullptr;

    switch (index) {
        case 0: { // 画笔
            drawObject = new QMDrawPen(currentColor_, widthSpin_->value());
            break;
        }
        case 1: { // 矩形
            drawObject = new QMDrawRect(currentColor_, widthSpin_->value(), false);
            break;
        }
        case 2: { // 图片（示例：加载一个默认图片或选择文件）
            QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "",
                "Images (*.png *.xpm *.jpg *.jpeg)");
            if (fileName.isEmpty()) return;

            QPixmap pixmap(fileName);
            if (pixmap.isNull()) {
                QMessageBox::warning(this, "错误", "无法加载图片");
                return;
            }
            auto *pixmapObj = new QMDrawPixmap(pixmap);
            pixmapObj->setFill(true);
            drawObject = pixmapObj;
            break;
        }
    }

    if (drawObject) {
        scene_->setActiveDrawObject(drawObject);
    }
}

void Demo::onColorChanged() {
    QColor color = QColorDialog::getColor(currentColor_, this, "选择颜色");
    if (color.isValid()) {
        currentColor_ = color;
        colorBtn_->setStyleSheet(QString("background-color: %1; min-height: 30px;").arg(color.name()));
        // 重新创建当前工具以应用新颜色
        onToolChanged(toolCombo_->currentIndex());
    }
}

void Demo::onWidthChanged(int width) {
    Q_UNUSED(width)
    // 重新创建当前工具以应用新线宽
    onToolChanged(toolCombo_->currentIndex());
}

void Demo::onAddLayer() {
    if (!scene_) return;

    auto *newLayer = new Layer();
    scene_->setActiveLayer(newLayer);
    updateLayerList();

    // 选中新添加的图层（最后一个）
    layerList_->setCurrentRow(layerList_->count() - 1);
}

void Demo::onDeleteLayer() {
    int row = layerList_->currentRow();
    if (row < 0 || !scene_) return;

    // 注意：当前实现没有直接删除图层的接口，这里只是隐藏示例
    // 实际项目中应该实现 LayerManager 的删除功能
    scene_->setLayerHide(row, true);
    updateLayerList();
}

void Demo::onLayerItemChanged(QListWidgetItem *item) {
    if (!item || !scene_) return;

    int index = layerList_->row(item);
    bool visible = item->checkState() == Qt::Checked;
    scene_->setLayerHide(index, !visible);
}

void Demo::onLayerSelectionChanged() {
    int row = layerList_->currentRow();
    if (row < 0 || !scene_) return;

    scene_->switchLayer(row);
    currentLayerId_ = row;
}

void Demo::onUndo() {
    if (scene_) {
        scene_->onCtrlAndZ();
    }
}

void Demo::onRedo() {
    if (scene_) {
        scene_->onCtrlAndY();
    }
}

void Demo::onClearCanvas() {
    // 清空当前图层的所有绘制对象
    // 注意：这里只是一个示例，实际实现可能需要更复杂的逻辑
    QMessageBox::information(this, "提示", "清空功能需要扩展 Layer 类实现");
}

void Demo::onSaveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", "output.png",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if (fileName.isEmpty()) return;

    QPixmap pixmap = scene_->pixmap();
    if (pixmap.save(fileName)) {
        QMessageBox::information(this, "成功", "图片已保存");
    } else {
        QMessageBox::warning(this, "错误", "保存失败");
    }
}

void Demo::updateLayerList() {
    if (!scene_) return;

    layerList_->clear();
    LayerManager *lm = nullptr;
    // 通过 scene 获取 LayerManager（需要添加 getter 或修改访问权限）
    // 这里简化处理，假设我们知道有若干图层

    // 由于 LayerManager 是私有的，我们通过其他方式获取图层数量
    // 实际项目中应该在 QMCanvasScene 中添加获取图层数量的方法

    // 临时方案：至少显示当前活动图层
    auto *item = new QListWidgetItem("图层 1", layerList_);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
    layerList_->addItem(item);
}