// demo.cpp
// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "demo.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>

DemoWindow::DemoWindow(QWidget* parent)
    : QMainWindow(parent)
    , currentColor_(Qt::black)
    , currentWidth_(3)
    , currentTool_(ToolType::Pen)
    , fillRect_(false)
    , layerCount_(1) {

    setWindowTitle("QMCanvas Demo - 多层画布演示");
    resize(800, 600);

    setupUI();

    // 创建初始画布（黄色背景）
    QPixmap pixmap(1000, 1000);
    pixmap.fill(QColor(255, 250, 205)); // 浅黄色背景

    // 创建场景
    scene_ = new QMCanvasScene(pixmap, this);

    // 设置初始绘制对象为画笔
    createNewDrawObject();

    // 关联场景到视图
    canvasView_->setCanvasScene(scene_);

    // 设置视口背景色
    canvasView_->setViewportBackground(Qt::gray);

    updateLayerInfo();
}

DemoWindow::~DemoWindow() = default;

void DemoWindow::setupUI() {
    // 创建中央部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 左侧工具面板
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(10);

    // 工具选择组
    QGroupBox* toolGroup = new QGroupBox("绘制工具", this);
    QVBoxLayout* toolLayout = new QVBoxLayout(toolGroup);

    btnPen_ = new QPushButton("✏️ 画笔", this);
    btnRect_ = new QPushButton("⬜ 矩形", this);
    btnPixmap_ = new QPushButton("🖼️ 位图", this);
    btnColor_ = new QPushButton("🎨 选择颜色", this);

    btnPen_->setCheckable(true);
    btnRect_->setCheckable(true);
    btnPixmap_->setCheckable(true);
    btnPen_->setChecked(true);

    toolLayout->addWidget(btnPen_);
    toolLayout->addWidget(btnRect_);
    toolLayout->addWidget(btnPixmap_);
    toolLayout->addWidget(btnColor_);

    // 线宽控制
    QHBoxLayout* widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("线宽:", this));
    spinWidth_ = new QSpinBox(this);
    spinWidth_->setRange(1, 50);
    spinWidth_->setValue(currentWidth_);
    widthLayout->addWidget(spinWidth_);
    toolLayout->addLayout(widthLayout);

    leftLayout->addWidget(toolGroup);

    // 视图控制组
    QGroupBox* viewGroup = new QGroupBox("视图控制", this);
    QVBoxLayout* viewLayout = new QVBoxLayout(viewGroup);

    comboWheelMode_ = new QComboBox(this);
    comboWheelMode_->addItem("滚轮: 滚动视图", static_cast<int>(WheelMode::ROLL));
    comboWheelMode_->addItem("滚轮: 缩放视图", static_cast<int>(WheelMode::ZOOM));
    viewLayout->addWidget(comboWheelMode_);

    QPushButton* btnZoomIn = new QPushButton("🔍 放大", this);
    QPushButton* btnZoomOut = new QPushButton("🔍 缩小", this);
    QPushButton* btnResetZoom = new QPushButton("↺ 重置缩放", this);

    viewLayout->addWidget(btnZoomIn);
    viewLayout->addWidget(btnZoomOut);
    viewLayout->addWidget(btnResetZoom);

    leftLayout->addWidget(viewGroup);

    // 图层管理组
    QGroupBox* layerGroup = new QGroupBox("图层管理", this);
    QVBoxLayout* layerLayout = new QVBoxLayout(layerGroup);

    lblLayerInfo_ = new QLabel("当前: 图层 0", this);
    layerLayout->addWidget(lblLayerInfo_);

    comboLayerSelect_ = new QComboBox(this);
    comboLayerSelect_->addItem("图层 0");
    layerLayout->addWidget(comboLayerSelect_);

    QPushButton* btnNewLayer = new QPushButton("➕ 新建图层", this);
    QPushButton* btnSwitchLayer = new QPushButton("🔄 切换图层", this);
    QPushButton* btnToggleHide = new QPushButton("👁️ 隐藏/显示", this);
    QPushButton* btnSwapLayers = new QPushButton("⇅ 交换图层", this);

    layerLayout->addWidget(btnNewLayer);
    layerLayout->addWidget(btnSwitchLayer);
    layerLayout->addWidget(btnToggleHide);
    layerLayout->addWidget(btnSwapLayers);

    leftLayout->addWidget(layerGroup);

    // 编辑操作组
    QGroupBox* editGroup = new QGroupBox("编辑操作", this);
    QVBoxLayout* editLayout = new QVBoxLayout(editGroup);

    QPushButton* btnUndo = new QPushButton("↩️ 撤销 (Ctrl+Z)", this);
    QPushButton* btnRedo = new QPushButton("↪️ 重做 (Ctrl+Y)", this);
    QPushButton* btnSave = new QPushButton("💾 保存 (Ctrl+S)", this);

    editLayout->addWidget(btnUndo);
    editLayout->addWidget(btnRedo);
    editLayout->addWidget(btnSave);

    leftLayout->addWidget(editGroup);

    leftLayout->addStretch();

    // 画布视图
    canvasView_ = new QMCanvasView(this);

    mainLayout->addLayout(leftLayout, 0);
    mainLayout->addWidget(canvasView_, 1);

    // 连接信号槽
    connect(btnPen_, &QPushButton::clicked, this, &DemoWindow::onPenToolSelected);
    connect(btnRect_, &QPushButton::clicked, this, &DemoWindow::onRectToolSelected);
    connect(btnPixmap_, &QPushButton::clicked, this, &DemoWindow::onPixmapToolSelected);
    connect(btnColor_, &QPushButton::clicked, this, &DemoWindow::onColorChanged);
    connect(spinWidth_, QOverload<int>::of(&QSpinBox::valueChanged), this, &DemoWindow::onWidthChanged);

    connect(comboWheelMode_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DemoWindow::onWheelModeChanged);
    connect(btnZoomIn, &QPushButton::clicked, this, &DemoWindow::onZoomIn);
    connect(btnZoomOut, &QPushButton::clicked, this, &DemoWindow::onZoomOut);
    connect(btnResetZoom, &QPushButton::clicked, this, &DemoWindow::onResetZoom);

    connect(btnNewLayer, &QPushButton::clicked, this, &DemoWindow::onNewLayer);
    connect(btnSwitchLayer, &QPushButton::clicked, this, &DemoWindow::onSwitchLayer);
    connect(btnToggleHide, &QPushButton::clicked, this, &DemoWindow::onToggleLayerHide);
    connect(btnSwapLayers, &QPushButton::clicked, this, &DemoWindow::onSwapLayers);

    connect(btnUndo, &QPushButton::clicked, this, &DemoWindow::onUndo);
    connect(btnRedo, &QPushButton::clicked, this, &DemoWindow::onRedo);

    // 监听保存信号
    connect(canvasView_->viewport(), &Viewport::save, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "保存图片", "",
            "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            scene_->pixmap().save(fileName);
            QMessageBox::information(this, "保存成功", "图片已保存至: " + fileName);
        }
    });
}

void DemoWindow::setupTools() {
    // 工具初始化已在 setupUI 中完成
}

void DemoWindow::setupLayerControls() {
    // 图层控制已在 setupUI 中完成
}

void DemoWindow::createNewDrawObject() {
    if (!scene_) return;

    QMDrawObject* drawObj = nullptr;

    switch (currentTool_) {
        case ToolType::Pen:
            drawObj = new QMDrawPen(currentColor_, currentWidth_);
            break;
        case ToolType::Rect:
            drawObj = new QMDrawRect(currentColor_, currentWidth_, fillRect_);
            break;
        case ToolType::Pixmap: {
            // 创建一个示例位图（蓝绿渐变）
            QPixmap pix(200, 200);
            QPainter painter(&pix);
            painter.fillRect(pix.rect(), QBrush(QGradient(QGradient::DeepBlue)));
            drawObj = new QMDrawPixmap(pix);
            static_cast<QMDrawPixmap*>(drawObj)->setFill(false); // 区域绘制模式
            break;
        }
    }

    if (drawObj) {
        scene_->setActiveDrawObject(drawObj);
    }
}

void DemoWindow::onPenToolSelected() {
    currentTool_ = ToolType::Pen;
    btnPen_->setChecked(true);
    btnRect_->setChecked(false);
    btnPixmap_->setChecked(false);
    createNewDrawObject();
}

void DemoWindow::onRectToolSelected() {
    currentTool_ = ToolType::Rect;
    btnPen_->setChecked(false);
    btnRect_->setChecked(true);
    btnPixmap_->setChecked(false);
    createNewDrawObject();
}

void DemoWindow::onPixmapToolSelected() {
    currentTool_ = ToolType::Pixmap;
    btnPen_->setChecked(false);
    btnRect_->setChecked(false);
    btnPixmap_->setChecked(true);
    createNewDrawObject();
}

void DemoWindow::onColorChanged() {
    QColor color = QColorDialog::getColor(currentColor_, this, "选择绘制颜色");
    if (color.isValid()) {
        currentColor_ = color;
        // 更新当前活动绘制对象的颜色
        createNewDrawObject();

        // 设置按钮背景色提示
        QString style = QString("background-color: %1; color: %2;")
            .arg(currentColor_.name())
            .arg(currentColor_.lightness() > 128 ? "black" : "white");
        btnColor_->setStyleSheet(style);
    }
}

void DemoWindow::onWidthChanged(int width) {
    currentWidth_ = width;
    createNewDrawObject();
}

void DemoWindow::onNewLayer() {
    if (!scene_) return;

    scene_->finishActiveLayer();

    // 创建新图层
    Layer* newLayer = new Layer();
    newLayer->setBase(QSize(1000, 1000));

    scene_->setActiveLayer(newLayer);
    scene_->finishActiveLayer(); // 完成设置，准备绘制

    layerCount_++;
    comboLayerSelect_->addItem(QString("图层 %1").arg(layerCount_ - 1));
    comboLayerSelect_->setCurrentIndex(layerCount_ - 1);

    // 为新图层创建绘制对象
    createNewDrawObject();

    updateLayerInfo();
}

void DemoWindow::onSwitchLayer() {
    if (!scene_ || comboLayerSelect_->count() == 0) return;

    int index = comboLayerSelect_->currentIndex();
    scene_->switchLayer(index);
    createNewDrawObject();
    updateLayerInfo();
    canvasView_->updateViewport();
}

void DemoWindow::onToggleLayerHide() {
    if (!scene_) return;

    int index = scene_->layerManager().items().indexOf(scene_->activeLayer());
    bool currentlyHidden = scene_->isLayerHide(index);
    scene_->setLayerHide(index, !currentlyHidden);

    updateLayerInfo();
    canvasView_->updateViewport();
}

void DemoWindow::onSwapLayers() {
    if (!scene_ || layerCount_ < 2) return;

    bool ok;
    int a = QInputDialog::getInt(this, "交换图层", "第一个图层索引:", 0, 0, layerCount_-1, 1, &ok);
    if (!ok) return;
    int b = QInputDialog::getInt(this, "交换图层", "第二个图层索引:", 1, 0, layerCount_-1, 1, &ok);
    if (!ok) return;

    scene_->swapLayer(a, b);

    // 交换下拉框中的文本
    QString textA = comboLayerSelect_->itemText(a);
    QString textB = comboLayerSelect_->itemText(b);
    comboLayerSelect_->setItemText(a, textB);
    comboLayerSelect_->setItemText(b, textA);

    canvasView_->updateViewport();
}

void DemoWindow::onUndo() {
    if (!scene_) return;
    scene_->onCtrlAndZ();
    canvasView_->updateViewport();
}

void DemoWindow::onRedo() {
    if (!scene_) return;
    scene_->onCtrlAndY();
    canvasView_->updateViewport();
}

void DemoWindow::onWheelModeChanged(int index) {
    if (!canvasView_) return;
    WheelMode mode = static_cast<WheelMode>(comboWheelMode_->itemData(index).toInt());
    canvasView_->setWheelMode(mode);
}

void DemoWindow::onZoomIn() {
    if (!scene_) return;
    qreal current = scene_->ratio();
    qreal factor = scene_->factor();
    scene_->setRatio(qMin(current * factor, scene_->maxRatio()));
}

void DemoWindow::onZoomOut() {
    if (!scene_) return;
    qreal current = scene_->ratio();
    qreal factor = scene_->factor();
    scene_->setRatio(qMax(current / factor, scene_->minRatio()));
}

void DemoWindow::onResetZoom() {
    if (!scene_) return;
    scene_->setRatio(1.0);
}

void DemoWindow::updateLayerInfo() {
    if (!scene_) return;

    int currentLayer = scene_->layerManager().items().indexOf(scene_->activeLayer());
    QString info = QString("当前: 图层 %1").arg(currentLayer);

    if (scene_->isLayerHide(currentLayer)) {
        info += " (隐藏)";
    } else {
        info += " (显示)";
    }

    lblLayerInfo_->setText(info);
}