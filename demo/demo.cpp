// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "demo.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QKeySequence>

DemoWindow::DemoWindow(QWidget *parent)
    : QMainWindow(parent) {

    setupUI();
    setupConnections();

    setWindowTitle("QMCanvas 绘图演示");
    resize(1200, 800);

    QPixmap pixmap(800, 600);
    pixmap.fill(Qt::white);
    scene_ = new QMCanvasScene(pixmap, this);
    canvasView_->setCanvasScene(scene_);

    canvasView_->setViewportBackground();

    updateStatus();
}

DemoWindow::~DemoWindow() = default;

void DemoWindow::setupUI() {
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    canvasView_ = new QMCanvasView(this);
    mainLayout->addWidget(canvasView_);

    auto *toolbar = addToolBar("工具栏");
    toolbar->setMovable(false);

    toolbar->addWidget(new QLabel(" 工具: ", this));
    toolCombo_ = new QComboBox(this);
    toolCombo_->addItem("画笔", 0);
    toolCombo_->addItem("矩形", 1);
    toolCombo_->addItem("椭圆(矩形模拟)", 2);
    toolbar->addWidget(toolCombo_);

    toolbar->addSeparator();

    toolbar->addWidget(new QLabel(" 粗细: ", this));
    widthSpin_ = new QSpinBox(this);
    widthSpin_->setRange(1, 50);
    widthSpin_->setValue(3);
    toolbar->addWidget(widthSpin_);

    toolbar->addSeparator();

    toolbar->addWidget(new QLabel(" 颜色: ", this));
    colorBtn_ = new QPushButton(this);
    colorBtn_->setFixedSize(30, 30);
    colorBtn_->setStyleSheet(QString("background-color: %1").arg(currentColor_.name()));
    toolbar->addWidget(colorBtn_);

    toolbar->addSeparator();

    auto *fillAction = toolbar->addAction("填充");
    fillAction->setCheckable(true);
    connect(fillAction, &QAction::toggled, this, &DemoWindow::onFillChanged);

    toolbar->addSeparator();

    toolbar->addAction("新建图层", this, &DemoWindow::onNewLayer);
    toolbar->addAction("清空当前", this, &DemoWindow::onClearLayer);

    toolbar->addSeparator();

    toolbar->addAction("撤销 (Ctrl+Z)", this, &DemoWindow::onUndo);
    toolbar->addAction("重做 (Ctrl+Y)", this, &DemoWindow::onRedo);

    toolbar->addSeparator();

    toolbar->addAction("放大", this, &DemoWindow::onZoomIn);
    toolbar->addAction("缩小", this, &DemoWindow::onZoomOut);
    toolbar->addAction("保存", this, &DemoWindow::onSaveImage);

    statusBar()->showMessage("就绪");

    statusLabel_ = new QLabel("当前工具: 画笔", this);
    zoomLabel_ = new QLabel("缩放: 100%", this);
    statusBar()->addPermanentWidget(statusLabel_);
    statusBar()->addPermanentWidget(zoomLabel_);
}

void DemoWindow::setupConnections() {
    connect(toolCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DemoWindow::onToolChanged);
    connect(colorBtn_, &QPushButton::clicked, this, &DemoWindow::onColorChanged);
    connect(widthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &DemoWindow::onWidthChanged);

    // 修复：为 QShortcut 设置父对象，避免内存泄漏警告
    auto *undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    auto *redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
    auto *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);

    connect(undoShortcut, &QShortcut::activated, this, &DemoWindow::onUndo);
    connect(redoShortcut, &QShortcut::activated, this, &DemoWindow::onRedo);
    connect(saveShortcut, &QShortcut::activated, this, &DemoWindow::onSaveImage);
}

void DemoWindow::onToolChanged(int index) {
    if (!scene_) return;

    QMDrawObject *tool = nullptr;

    switch (index) {
        case 0: {
            tool = new QMDrawPen(currentColor_, currentWidth_);
            statusLabel_->setText("当前工具: 画笔");
            break;
        }
        case 1: {
            tool = new QMDrawRect(currentColor_, currentWidth_, fillRect_);
            statusLabel_->setText("当前工具: 矩形");
            break;
        }
        case 2: {
            tool = new QMDrawRect(currentColor_, currentWidth_, fillRect_);
            statusLabel_->setText("当前工具: 矩形(椭圆占位)");
            break;
        }
        default:  // 修复：处理默认情况
            qWarning() << "Unknown tool index:" << index;
            tool = new QMDrawPen(currentColor_, currentWidth_);
            statusLabel_->setText("当前工具: 画笔(默认)");
            break;
    }

    if (tool) {
        scene_->setActiveDrawObject(tool);
    }
}

void DemoWindow::onColorChanged() {
    QColor color = QColorDialog::getColor(currentColor_, this, "选择颜色");
    if (color.isValid()) {
        currentColor_ = color;
        colorBtn_->setStyleSheet(QString("background-color: %1").arg(color.name()));
        onToolChanged(toolCombo_->currentIndex());
    }
}

void DemoWindow::onWidthChanged(int width) {
    currentWidth_ = width;
    onToolChanged(toolCombo_->currentIndex());
}

void DemoWindow::onFillChanged(bool fill) {
    fillRect_ = fill;
    int currentTool = toolCombo_->currentIndex();
    if (currentTool == 1 || currentTool == 2) {
        onToolChanged(currentTool);
    }
}

void DemoWindow::onNewLayer() {
    if (!scene_) return;

    scene_->finishActiveLayer();
    statusBar()->showMessage("新建图层已创建", 2000);
}

void DemoWindow::onClearLayer() {
    if (!scene_) return;

    // 修复：将变量移动到 if 语句中，缩小作用域
    if (auto *layer = scene_->activeLayer(); layer != nullptr) {
        layer->clear();
        canvasView_->updateViewport();
        statusBar()->showMessage("当前图层已清空", 2000);
    }
}

void DemoWindow::onSaveImage() {
    if (!scene_) return;

    QString fileName = QFileDialog::getSaveFileName(this,
        "保存图像",
        "untitled.png",
        "PNG 图像 (*.png);;JPEG 图像 (*.jpg *.jpeg);;BMP 图像 (*.bmp)");

    if (!fileName.isEmpty()) {
        QPixmap pixmap = scene_->pixmap();
        if (pixmap.save(fileName)) {
            statusBar()->showMessage(QString("已保存到: %1").arg(fileName), 3000);
        } else {
            QMessageBox::critical(this, "错误", "保存图像失败！");
        }
    }
}

void DemoWindow::onUndo() {
    if (!scene_) return;
    scene_->onCtrlAndZ();
    canvasView_->updateViewport();
    statusBar()->showMessage("撤销", 1000);
}

void DemoWindow::onRedo() {
    if (!scene_) return;
    scene_->onCtrlAndY();
    canvasView_->updateViewport();
    statusBar()->showMessage("重做", 1000);
}

void DemoWindow::onZoomIn() {
    if (!scene_) return;
    scene_->onScaleBy(true, QPoint(400, 300));
    updateStatus();
}

void DemoWindow::onZoomOut() {
    if (!scene_) return;
    scene_->onScaleBy(false, QPoint(400, 300));
    updateStatus();
}

// 修复：添加 const 修饰符
void DemoWindow::updateStatus() const {
    if (scene_) {
        int percent = static_cast<int>(scene_->ratio() * 100);
        zoomLabel_->setText(QString("缩放: %1%").arg(percent));
    }
}