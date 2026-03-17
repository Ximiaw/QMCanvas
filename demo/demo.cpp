// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "demo.h"
#include <QtMath>
#include <QPainter>
#include <QGraphicsEffect>

QMDrawEllipse::QMDrawEllipse(QColor color, int width, bool fill) : fill_(fill) {
    pen_ = QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    brush_ = fill ? QBrush(color.lighter(180)) : QBrush(Qt::NoBrush);
}

void QMDrawEllipse::draw(QPainter* painter) {
    if (begin_ == end_) return;
    QRect rect = QRect(begin_, end_).normalized();
    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    painter->setPen(pen_);
    painter->setBrush(brush_);
    painter->drawEllipse(rect);
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
}

QPen* QMDrawEllipse::pen() { return &pen_; }
QBrush* QMDrawEllipse::brush() { return &brush_; }

QSharedPointer<QMDrawObject> QMDrawEllipse::clone() {
    return QSharedPointer<QMDrawObject>(new QMDrawEllipse(pen_.color(), pen_.width(), fill_));
}

QMDrawLine::QMDrawLine(QColor color, int width) {
    pen_ = QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

void QMDrawLine::draw(QPainter* painter) {
    if (begin_ == end_) return;
    QPen oldPen = painter->pen();
    painter->setPen(pen_);
    painter->drawLine(begin_, end_);
    painter->setPen(oldPen);
}

QPen* QMDrawLine::pen() { return &pen_; }
QBrush* QMDrawLine::brush() { return nullptr; }

QSharedPointer<QMDrawObject> QMDrawLine::clone() {
    return QSharedPointer<QMDrawObject>(new QMDrawLine(pen_.color(), pen_.width()));
}

GridCanvasScene::GridCanvasScene(QPixmap pixmap, QObject* parent)
    : QMCanvasScene(pixmap, parent) {}

void GridCanvasScene::setShowGrid(bool show) { showGrid_ = show; }
void GridCanvasScene::setGridSize(int size) { gridSize_ = size; }
void GridCanvasScene::setGridColor(QColor color) { gridColor_ = color; }

void GridCanvasScene::draw(QPainter* painter) {
    QMCanvasScene::draw(painter);
    if (showGrid_) {
        QRect rect = getViewportRect();
        painter->setPen(QPen(gridColor_, 1, Qt::SolidLine));
        int startX = (rect.left() / gridSize_) * gridSize_;
        int startY = (rect.top() / gridSize_) * gridSize_;
        for (int x = startX; x <= rect.right(); x += gridSize_)
            painter->drawLine(x, rect.top(), x, rect.bottom());
        for (int y = startY; y <= rect.bottom(); y += gridSize_)
            painter->drawLine(rect.left(), y, rect.right(), y);
    }
}

QMCanvasStudio::QMCanvasStudio(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QMCanvas");
    resize(1600, 1000);

    setupUI();
    setupMenuBar();
    setupShortcuts();
    setupConnections();

    createNewCanvas(QSize(1200, 900));
    onToolChanged(static_cast<int>(ToolType::Pencil));
    updatePropertyPanelState();
}

QMCanvasStudio::~QMCanvasStudio() = default;

void QMCanvasStudio::setupUI() {
    canvasView_ = new QMCanvasView(this);
    canvasView_->setViewportBackground(bgColor_);
    setCentralWidget(canvasView_);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks |
                   QMainWindow::AllowTabbedDocks);

    setupToolDock();
    setupLayerDock();
    setupPropertyDock();
    setupNavigatorDock();
    setupStatusBar();
}

void QMCanvasStudio::setupMenuBar() {
    auto *fileMenu = menuBar()->addMenu("文件(&F)");
    fileMenu->addAction("新建画布(&N)...", QKeySequence::New, this, &QMCanvasStudio::onNewCanvas);
    fileMenu->addAction("打开图像(&O)...", QKeySequence::Open, this, &QMCanvasStudio::onOpenImage);
    fileMenu->addAction("作为图层打开(&L)...", QKeySequence("Ctrl+Shift+O"),
                       this, &QMCanvasStudio::onOpenAsLayer);
    fileMenu->addSeparator();
    fileMenu->addAction("保存(&S)", QKeySequence::Save, this, &QMCanvasStudio::onSave);
    fileMenu->addAction("导出(&E)...", QKeySequence("Ctrl+Shift+S"),
                       this, &QMCanvasStudio::onExport);
    fileMenu->addAction("导出可见图层(&V)...", QKeySequence("Ctrl+Alt+S"),
                       this, &QMCanvasStudio::onExportVisible);
    fileMenu->addSeparator();
    fileMenu->addAction("退出(&Q)", QKeySequence::Quit, this, &QWidget::close);

    auto *editMenu = menuBar()->addMenu("编辑(&E)");
    editMenu->addAction("撤销(&U)", QKeySequence::Undo, this, &QMCanvasStudio::onUndo);
    editMenu->addAction("重做(&R)", QKeySequence::Redo, this, &QMCanvasStudio::onRedo);
    editMenu->addSeparator();
    editMenu->addAction("剪切(&T)", QKeySequence::Cut, this, &QMCanvasStudio::onCut);
    editMenu->addAction("复制(&C)", QKeySequence::Copy, this, &QMCanvasStudio::onCopy);
    editMenu->addAction("粘贴(&P)", QKeySequence::Paste, this, &QMCanvasStudio::onPaste);
    editMenu->addSeparator();
    editMenu->addAction("清空当前图层(&D)", QKeySequence("Ctrl+Del"),
                       this, &QMCanvasStudio::onClearLayer);

    auto *viewMenu = menuBar()->addMenu("视图(&V)");
    viewMenu->addAction("放大(&+)", QKeySequence::ZoomIn, this, &QMCanvasStudio::onZoomIn);
    viewMenu->addAction("缩小(&-)", QKeySequence::ZoomOut, this, &QMCanvasStudio::onZoomOut);
    viewMenu->addAction("适合窗口(&F)", QKeySequence("Ctrl+0"), this, &QMCanvasStudio::onFitToWindow);
    viewMenu->addAction("实际像素(&1)", QKeySequence("Ctrl+1"), this, &QMCanvasStudio::onResetView);
    viewMenu->addSeparator();
    auto *gridAction = viewMenu->addAction("显示网格(&G)");
    gridAction->setCheckable(true);
    gridAction->setChecked(showGrid_);
    connect(gridAction, &QAction::toggled, this, &QMCanvasStudio::onGridToggled);
    viewMenu->addAction("网格设置(&S)...", this, [this]() {
        bool ok;
        int size = QInputDialog::getInt(this, "网格设置", "网格大小 (像素):",
                                       20, 5, 100, 1, &ok);
        if (ok) onGridSizeChanged(size);
    });

    auto *layerMenu = menuBar()->addMenu("图层(&L)");
    layerMenu->addAction("新建图层(&N)", QKeySequence("Ctrl+Shift+N"),
                        this, &QMCanvasStudio::onNewLayer);
    layerMenu->addAction("复制图层(&D)", this, &QMCanvasStudio::onDuplicateLayer);
    layerMenu->addAction("删除图层(&L)", QKeySequence("Ctrl+Shift+Delete"),
                        this, &QMCanvasStudio::onDeleteLayer);
    layerMenu->addSeparator();
    layerMenu->addAction("向上移动(&U)", QKeySequence("Ctrl+]"),
                        this, &QMCanvasStudio::onMoveLayerUp);
    layerMenu->addAction("向下移动(&D)", QKeySequence("Ctrl+["),
                        this, &QMCanvasStudio::onMoveLayerDown);
    layerMenu->addAction("置为顶层(&T)", QKeySequence("Ctrl+Shift+]"),
                        this, &QMCanvasStudio::onMoveLayerToTop);
    layerMenu->addAction("置为底层(&B)", QKeySequence("Ctrl+Shift+["),
                        this, &QMCanvasStudio::onMoveLayerToBottom);
    layerMenu->addSeparator();
    layerMenu->addAction("向下合并(&E)", QKeySequence("Ctrl+E"),
                        this, &QMCanvasStudio::onMergeDown);
    layerMenu->addAction("合并所有(&M)", this, &QMCanvasStudio::onMergeAll);

    auto *helpMenu = menuBar()->addMenu("帮助(&H)");
    helpMenu->addAction("快捷键参考(&K)", this, [this]() {
        QMessageBox::information(this, "快捷键参考",
            "<b>工具切换:</b><br>"
            "Space - 抓手工具<br>"
            "B - 画笔<br>"
            "L - 直线<br>"
            "R - 矩形<br>"
            "O - 椭圆<br>"
            "E - 橡皮擦<br><br>"
            "<b>视图操作:</b><br>"
            "Ctrl++ / Ctrl+- - 放大/缩小<br>"
            "Ctrl+0 - 适合窗口<br>"
            "Ctrl+1 - 实际像素<br>"
            "Ctrl+滚轮 - 缩放<br><br>"
            "<b>编辑:</b><br>"
            "Ctrl+Z - 撤销<br>"
            "Ctrl+Y - 重做<br>"
            "Ctrl+Shift+Z - 重做<br>"
            "Delete - 删除图层<br><br>"
            "<b>图层:</b><br>"
            "Ctrl+Shift+N - 新建图层<br>"
            "Ctrl+] / Ctrl+[ - 上移/下移<br>"
            "Ctrl+E - 向下合并");
    });
    helpMenu->addSeparator();
    helpMenu->addAction("关于(&A)", this, [this]() {
        QMessageBox::about(this, "关于 QMCanvas Studio",
            "<h2>QMCanvas Studio</h2>"
            "<p>基于 QMCanvas 组件的专业绘图演示程序</p>"
            "<p>功能包括：多图层管理、撤销重做、视口变换、多种绘制工具</p>");
    });
}

void QMCanvasStudio::setupToolDock() {
    auto *toolDock = new QDockWidget("工具箱", this);
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    auto *container = new QWidget;
    auto *layout = new QVBoxLayout(container);
    layout->setSpacing(8);
    layout->setAlignment(Qt::AlignTop);

    toolGroup_ = new QButtonGroup(this);
    toolGroup_->setExclusive(true);

    auto createToolBtn = [this](const QString &icon, const QString &tip, ToolType type) -> QToolButton* {
        auto *btn = new QToolButton;
        btn->setText(icon);
        btn->setToolTip(tip);
        btn->setCheckable(true);
        btn->setMinimumSize(48, 48);
        btn->setStyleSheet(R"(
            QToolButton {
                border: 2px solid #888;
                border-radius: 6px;
                background: #3c3c3c;
                color: #eee;
                font-size: 20px;
                font-weight: bold;
            }
            QToolButton:checked {
                border-color: #0078d4;
                background: #005a9e;
                color: white;
            }
            QToolButton:hover {
                background: #4a4a4a;
                border-color: #aaa;
            }
            QToolButton:checked:hover {
                background: #0066b8;
            }
        )");
        toolGroup_->addButton(btn, static_cast<int>(type));
        return btn;
    };

    btnHand_ = createToolBtn("✋", "抓手工具 (Space)", ToolType::Hand);
    btnPencil_ = createToolBtn("✏️", "画笔 (B)", ToolType::Pencil);
    btnLine_ = createToolBtn("📏", "直线 (L)", ToolType::Line);
    btnRect_ = createToolBtn("⬜", "矩形 (R)", ToolType::Rect);
    btnEllipse_ = createToolBtn("⭕", "椭圆 (O)", ToolType::Ellipse);
    btnEraser_ = createToolBtn("🧼", "橡皮擦 (E)", ToolType::Eraser);

    auto *grid = new QGridLayout;
    grid->addWidget(btnHand_, 0, 0);
    grid->addWidget(btnPencil_, 0, 1);
    grid->addWidget(btnLine_, 1, 0);
    grid->addWidget(btnRect_, 1, 1);
    grid->addWidget(btnEllipse_, 2, 0);
    grid->addWidget(btnEraser_, 2, 1);

    layout->addLayout(grid);

    auto *optionsLabel = new QLabel("<b>工具选项</b>");
    optionsLabel->setStyleSheet("color: #ccc; margin-top: 10px;");
    layout->addWidget(optionsLabel);

    auto *presetsLayout = new QHBoxLayout;
    QStringList presets = {"细线", "标准", "粗线"};
    int widths[] = {1, 3, 8};
    for (int i = 0; i < presets.size(); ++i) {
        auto *btn = new QPushButton(presets[i]);
        btn->setFixedHeight(28);
        connect(btn, &QPushButton::clicked, [this, i, widths]() {
            widthSlider_->setValue(widths[i]);
        });
        presetsLayout->addWidget(btn);
    }
    layout->addLayout(presetsLayout);
    layout->addStretch();

    toolDock->setWidget(container);
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);

    connect(toolGroup_, &QButtonGroup::idClicked, this, &QMCanvasStudio::onToolChanged);
    btnPencil_->setChecked(true);
}

void QMCanvasStudio::setupPropertyDock() {
    auto *propDock = new QDockWidget("属性", this);
    propDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    auto *container = new QWidget;
    auto *layout = new QVBoxLayout(container);
    layout->setSpacing(15);

    auto *strokeGroup = new QGroupBox("笔触设置");
    strokeGroup->setStyleSheet("QGroupBox { color: #ccc; font-weight: bold; }");
    auto *strokeLayout = new QVBoxLayout(strokeGroup);

    auto *widthLayout = new QHBoxLayout;
    widthLayout->addWidget(new QLabel("粗细:"));
    widthSpin_ = new QSpinBox;
    widthSpin_->setRange(1, 100);
    widthSpin_->setValue(3);
    widthSpin_->setSuffix(" px");
    widthLayout->addWidget(widthSpin_);
    strokeLayout->addLayout(widthLayout);

    widthSlider_ = new QSlider(Qt::Horizontal);
    widthSlider_->setRange(1, 100);
    widthSlider_->setValue(3);
    strokeLayout->addWidget(widthSlider_);

    auto *colorLayout = new QHBoxLayout;
    colorLayout->addWidget(new QLabel("颜色:"));
    colorPreview_ = new QLabel;
    colorPreview_->setFixedSize(40, 24);
    colorPreview_->setStyleSheet(QString("background-color: %1; border: 1px solid #666; border-radius: 3px;")
                                .arg(currentColor_.name()));
    colorLayout->addWidget(colorPreview_);

    colorBtn_ = new QPushButton("选择...");
    colorBtn_->setFixedHeight(24);
    colorLayout->addWidget(colorBtn_);
    colorLayout->addStretch();
    strokeLayout->addLayout(colorLayout);

    fillCheck_ = new QCheckBox("填充形状");
    fillCheck_->setToolTip("仅对矩形和椭圆有效");
    strokeLayout->addWidget(fillCheck_);

    antiAliasCheck_ = new QCheckBox("抗锯齿");
    antiAliasCheck_->setChecked(true);
    strokeLayout->addWidget(antiAliasCheck_);

    layout->addWidget(strokeGroup);

    auto *opacityGroup = new QGroupBox("不透明度");
    opacityGroup->setStyleSheet("QGroupBox { color: #ccc; font-weight: bold; }");
    auto *opacityLayout = new QVBoxLayout(opacityGroup);

    auto *opacitySliderLayout = new QHBoxLayout;
    opacitySlider_ = new QSlider(Qt::Horizontal);
    opacitySlider_->setRange(0, 100);
    opacitySlider_->setValue(100);
    opacitySliderLayout->addWidget(opacitySlider_);
    opacityLabel_ = new QLabel("100%");
    opacityLabel_->setFixedWidth(45);
    opacitySliderLayout->addWidget(opacityLabel_);
    opacityLayout->addLayout(opacitySliderLayout);

    layout->addWidget(opacityGroup);

    auto *blendGroup = new QGroupBox("混合模式");
    blendGroup->setStyleSheet("QGroupBox { color: #ccc; font-weight: bold; }");
    auto *blendLayout = new QVBoxLayout(blendGroup);
    blendModeCombo_ = new QComboBox;
    blendModeCombo_->addItems({"正常", "正片叠底", "滤色", "叠加", "差值"});
    blendModeCombo_->setCurrentIndex(0);
    blendModeCombo_->setEnabled(false);
    blendLayout->addWidget(blendModeCombo_);
    layout->addWidget(blendGroup);

    auto *viewGroup = new QGroupBox("视图设置");
    viewGroup->setStyleSheet("QGroupBox { color: #ccc; font-weight: bold; }");
    auto *viewLayout = new QVBoxLayout(viewGroup);

    gridCheck_ = new QCheckBox("显示网格");
    viewLayout->addWidget(gridCheck_);

    auto *bgLayout = new QHBoxLayout;
    bgLayout->addWidget(new QLabel("背景色:"));
    bgColorBtn_ = new QPushButton("更改");
    bgLayout->addWidget(bgColorBtn_);
    bgLayout->addStretch();
    viewLayout->addLayout(bgLayout);

    layout->addWidget(viewGroup);
    layout->addStretch();

    scroll->setWidget(container);
    propDock->setWidget(scroll);
    addDockWidget(Qt::RightDockWidgetArea, propDock);

    connect(widthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            widthSlider_, &QSlider::setValue);
    connect(widthSlider_, &QSlider::valueChanged,
            widthSpin_, &QSpinBox::setValue);
    connect(widthSlider_, &QSlider::valueChanged, this, &QMCanvasStudio::onPenWidthChanged);
    connect(colorBtn_, &QPushButton::clicked, this, &QMCanvasStudio::onColorChanged);
    connect(fillCheck_, &QCheckBox::toggled, this, &QMCanvasStudio::onFillChanged);
    connect(opacitySlider_, &QSlider::valueChanged, this, &QMCanvasStudio::onOpacityChanged);
    connect(antiAliasCheck_, &QCheckBox::toggled, this, &QMCanvasStudio::onAntiAliasChanged);
    connect(gridCheck_, &QCheckBox::toggled, this, &QMCanvasStudio::onGridToggled);
    connect(bgColorBtn_, &QPushButton::clicked, this, &QMCanvasStudio::onBgColorChanged);
}

void QMCanvasStudio::setupLayerDock() {
    auto *layerDock = new QDockWidget("图层", this);
    layerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

    auto *container = new QWidget;
    auto *layout = new QVBoxLayout(container);

    layerList_ = new QListWidget;
    layerList_->setSelectionMode(QAbstractItemView::SingleSelection);
    layerList_->setContextMenuPolicy(Qt::CustomContextMenu);
    layerList_->setStyleSheet(R"(
        QListWidget {
            background: #2b2b2b;
            color: #eee;
            border: 1px solid #3c3c3c;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #3c3c3c;
        }
        QListWidget::item:selected {
            background: #094771;
            color: white;
        }
        QListWidget::item:!selected {
            background: #2b2b2b;
        }
    )");

    connect(layerList_, &QListWidget::itemChanged, this, &QMCanvasStudio::onLayerVisibilityChanged);
    connect(layerList_, &QListWidget::itemClicked, this, &QMCanvasStudio::onLayerSelected);
    connect(layerList_, &QListWidget::itemDoubleClicked, this, &QMCanvasStudio::onLayerDoubleClicked);

    layout->addWidget(layerList_);

    auto *btnLayout1 = new QHBoxLayout;
    btnNewLayer_ = new QPushButton("新建");
    btnDupLayer_ = new QPushButton("复制");
    btnDelLayer_ = new QPushButton("删除");
    btnLayout1->addWidget(btnNewLayer_);
    btnLayout1->addWidget(btnDupLayer_);
    btnLayout1->addWidget(btnDelLayer_);
    layout->addLayout(btnLayout1);

    auto *btnLayout2 = new QHBoxLayout;
    btnMoveUp_ = new QPushButton("↑");
    btnMoveDown_ = new QPushButton("↓");
    btnMoveTop_ = new QPushButton("置顶");
    btnMoveBottom_ = new QPushButton("置底");
    btnMoveUp_->setToolTip("上移一层 (Ctrl+])");
    btnMoveDown_->setToolTip("下移一层 (Ctrl+[)");
    btnLayout2->addWidget(btnMoveUp_);
    btnLayout2->addWidget(btnMoveDown_);
    btnLayout2->addWidget(btnMoveTop_);
    btnLayout2->addWidget(btnMoveBottom_);
    layout->addLayout(btnLayout2);

    auto *btnLayout3 = new QHBoxLayout;
    btnMergeDown_ = new QPushButton("向下合并");
    btnMergeAll_ = new QPushButton("合并全部");
    btnClearLayer_ = new QPushButton("清空");
    btnLayout3->addWidget(btnMergeDown_);
    btnLayout3->addWidget(btnMergeAll_);
    btnLayout3->addWidget(btnClearLayer_);
    layout->addLayout(btnLayout3);

    connect(btnNewLayer_, &QPushButton::clicked, this, &QMCanvasStudio::onNewLayer);
    connect(btnDupLayer_, &QPushButton::clicked, this, &QMCanvasStudio::onDuplicateLayer);
    connect(btnDelLayer_, &QPushButton::clicked, this, &QMCanvasStudio::onDeleteLayer);
    connect(btnMoveUp_, &QPushButton::clicked, this, &QMCanvasStudio::onMoveLayerUp);
    connect(btnMoveDown_, &QPushButton::clicked, this, &QMCanvasStudio::onMoveLayerDown);
    connect(btnMoveTop_, &QPushButton::clicked, this, &QMCanvasStudio::onMoveLayerToTop);
    connect(btnMoveBottom_, &QPushButton::clicked, this, &QMCanvasStudio::onMoveLayerToBottom);
    connect(btnMergeDown_, &QPushButton::clicked, this, &QMCanvasStudio::onMergeDown);
    connect(btnMergeAll_, &QPushButton::clicked, this, &QMCanvasStudio::onMergeAll);
    connect(btnClearLayer_, &QPushButton::clicked, this, &QMCanvasStudio::onClearLayer);

    layerDock->setWidget(container);
    addDockWidget(Qt::RightDockWidgetArea, layerDock);
    tabifyDockWidget(findChildren<QDockWidget*>().first(), layerDock);
}

void QMCanvasStudio::setupNavigatorDock() {
    auto *navDock = new QDockWidget("导航器", this);
    navDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    navDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    auto *container = new QWidget;
    auto *layout = new QVBoxLayout(container);

    navigatorPreview_ = new QLabel("画布预览");
    navigatorPreview_->setAlignment(Qt::AlignCenter);
    navigatorPreview_->setMinimumSize(200, 150);
    navigatorPreview_->setStyleSheet("background: #1e1e1e; border: 1px solid #3c3c3c;");
    layout->addWidget(navigatorPreview_);

    auto *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(new QLabel("缩放:"));
    zoomSlider_ = new QSlider(Qt::Horizontal);
    zoomSlider_->setRange(5, 500);
    zoomSlider_->setValue(100);
    zoomLayout->addWidget(zoomSlider_);
    zoomLabel_ = new QLabel("100%");
    zoomLabel_->setFixedWidth(50);
    zoomLayout->addWidget(zoomLabel_);
    layout->addLayout(zoomLayout);

    auto *gridLayout = new QHBoxLayout;
    gridLayout->addWidget(new QLabel("网格大小:"));
    gridSizeSpin_ = new QSpinBox;
    gridSizeSpin_->setRange(5, 100);
    gridSizeSpin_->setValue(20);
    gridSizeSpin_->setSuffix(" px");
    gridLayout->addWidget(gridSizeSpin_);
    layout->addLayout(gridLayout);

    connect(zoomSlider_, &QSlider::valueChanged, this, &QMCanvasStudio::onZoomChanged);
    connect(gridSizeSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &QMCanvasStudio::onGridSizeChanged);

    navDock->setWidget(container);
    addDockWidget(Qt::RightDockWidgetArea, navDock);
}

void QMCanvasStudio::setupStatusBar() {
    statusLabel_ = new QLabel("就绪");
    mousePosLabel_ = new QLabel("X: 0, Y: 0");
    canvasSizeLabel_ = new QLabel("1200 x 900");
    zoomStatusLabel_ = new QLabel("100%");
    layerCountLabel_ = new QLabel("图层: 1");

    mousePosLabel_->setMinimumWidth(100);
    canvasSizeLabel_->setMinimumWidth(100);
    zoomStatusLabel_->setMinimumWidth(60);
    layerCountLabel_->setMinimumWidth(80);

    statusBar()->addWidget(statusLabel_);
    statusBar()->addPermanentWidget(layerCountLabel_);
    statusBar()->addPermanentWidget(zoomStatusLabel_);
    statusBar()->addPermanentWidget(mousePosLabel_);
    statusBar()->addPermanentWidget(canvasSizeLabel_);
}

void QMCanvasStudio::setupShortcuts() {
    auto *handShortcut = new QShortcut(QKeySequence("Space"), this);
    connect(handShortcut, &QShortcut::activated, this, [this]() {
        btnHand_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Hand));
    });

    new QShortcut(QKeySequence("B"), this, [this]() {
        btnPencil_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Pencil));
    });

    new QShortcut(QKeySequence("L"), this, [this]() {
        btnLine_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Line));
    });

    new QShortcut(QKeySequence("R"), this, [this]() {
        btnRect_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Rect));
    });

    new QShortcut(QKeySequence("O"), this, [this]() {
        btnEllipse_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Ellipse));
    });

    new QShortcut(QKeySequence("E"), this, [this]() {
        btnEraser_->setChecked(true);
        onToolChanged(static_cast<int>(ToolType::Eraser));
    });
}

void QMCanvasStudio::setupConnections() {
    if (canvasView_ && canvasView_->viewport()) {
        canvasView_->viewport()->installEventFilter(this);
    }
}

void QMCanvasStudio::createNewCanvas(const QSize &size, bool transparent) {
    QPixmap pixmap(size);
    if (transparent) {
        pixmap.fill(Qt::transparent);
    } else {
        pixmap.fill(bgColor_);
    }

    if (scene_) {
        disconnect(scene_, nullptr, this, nullptr);
    }

    scene_ = new GridCanvasScene(pixmap, this);
    scene_->setExtraViewportMargin(1.2);
    canvasView_->setCanvasScene(scene_);

    connect(scene_, &QMCanvasScene::viewportPixmapChanged, this, &QMCanvasStudio::onSceneUpdated);

    if (canvasView_->viewport()) {
        canvasView_->viewport()->installEventFilter(this);
    }

    canvasSizeLabel_->setText(QString("%1 x %2").arg(size.width()).arg(size.height()));
    updateLayerList();
    updateStatusInfo();
    syncZoomSlider();

    statusLabel_->setText("新建画布完成");
}

void QMCanvasStudio::addImageAsLayer(const QPixmap &pixmap, const QString &name) {
    if (!scene_) return;

    scene_->finishActiveLayer();

    auto *drawPixmap = new QMDrawPixmap(const_cast<QPixmap&>(pixmap));
    drawPixmap->setFill(true);
    scene_->setActiveDrawObject(drawPixmap);
    scene_->finishActiveDrawObject();

    updateLayerList();
    statusLabel_->setText(QString("已添加图层: %1").arg(name));
}

void QMCanvasStudio::updateLayerList() {
    if (!scene_) return;

    layerList_->blockSignals(true);
    layerList_->clear();

    LayerManager *lm = const_cast<LayerManager*>(&scene_->layerManager());
    if (!lm) {
        layerList_->blockSignals(false);
        return;
    }

    int count = lm->items().size();

    for (int i = count - 1; i >= 0; --i) {
        const auto &layerPtr = lm->items().at(i);
        if (layerPtr.isNull()) continue;

        bool isHidden = scene_->isLayerHide(i);
        bool isActive = (lm->activeObject() == layerPtr.get());

        QString name = QString("图层 %1").arg(count - i);
        if (isActive) name += " (当前)";

        auto *item = new QListWidgetItem(name);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(isHidden ? Qt::Unchecked : Qt::Checked);
        item->setData(Qt::UserRole, i);

        if (isActive) {
            item->setBackground(QColor(0, 120, 212));
            item->setForeground(Qt::white);
        }

        layerList_->addItem(item);
    }

    int activeIndex = -1;
    for (int i = 0; i < layerList_->count(); ++i) {
        if (layerList_->item(i)->data(Qt::UserRole).toInt() == count - 1) {
            activeIndex = i;
            break;
        }
    }
    if (activeIndex >= 0) {
        layerList_->setCurrentRow(activeIndex);
    }

    layerList_->blockSignals(false);
    layerCountLabel_->setText(QString("图层: %1").arg(count));
    updateLayerUIState();
}

void QMCanvasStudio::updateLayerUIState() {
    bool hasSelection = layerList_->currentRow() >= 0;
    int currentRow = layerList_->currentRow();
    int count = layerList_->count();

    btnDelLayer_->setEnabled(hasSelection && count > 1);
    btnMoveUp_->setEnabled(hasSelection && currentRow > 0);
    btnMoveDown_->setEnabled(hasSelection && currentRow < count - 1);
    btnMoveTop_->setEnabled(hasSelection && currentRow > 0);
    btnMoveBottom_->setEnabled(hasSelection && currentRow < count - 1);
    btnMergeDown_->setEnabled(hasSelection && currentRow < count - 1);
    btnClearLayer_->setEnabled(hasSelection);
}

void QMCanvasStudio::updatePropertyPanelState() {
    bool isHand = (currentTool_ == ToolType::Hand);
    bool isShape = (currentTool_ == ToolType::Rect || currentTool_ == ToolType::Ellipse);
    bool isLine = (currentTool_ == ToolType::Line);
    bool isPencil = (currentTool_ == ToolType::Pencil);
    bool isEraser = (currentTool_ == ToolType::Eraser);
    bool isDrawTool = !isHand;

    widthSpin_->setEnabled(isDrawTool);
    widthSlider_->setEnabled(isDrawTool);
    colorBtn_->setEnabled(isDrawTool && !isEraser);
    colorPreview_->setEnabled(isDrawTool && !isEraser);

    fillCheck_->setEnabled(isShape);
    if (!isShape) fillCheck_->setChecked(false);

    antiAliasCheck_->setEnabled(isDrawTool);
    opacitySlider_->setEnabled(isDrawTool);
    opacityLabel_->setEnabled(isDrawTool);
    blendModeCombo_->setEnabled(false);
}

QString QMCanvasStudio::getToolName(ToolType tool) const {
    switch (tool) {
        case ToolType::Hand: return "抓手";
        case ToolType::Pencil: return "画笔";
        case ToolType::Line: return "直线";
        case ToolType::Rect: return "矩形";
        case ToolType::Ellipse: return "椭圆";
        case ToolType::Eraser: return "橡皮擦";
        default: return "未知";
    }
}

void QMCanvasStudio::onToolChanged(int toolId) {
    currentTool_ = static_cast<ToolType>(toolId);

    if (!scene_) return;

    if (currentTool_ == ToolType::Hand) {
        scene_->endMove();
        setCursor(Qt::OpenHandCursor);
    } else {
        recreateCurrentTool();
        setCursor(Qt::CrossCursor);
    }

    updatePropertyPanelState();
    statusLabel_->setText(QString("当前工具: %1 | 粗细: %2px").arg(getToolName(currentTool_)).arg(currentWidth_));
}

void QMCanvasStudio::recreateCurrentTool() {
    if (!scene_ || currentTool_ == ToolType::Hand) return;

    QMDrawObject *tool = nullptr;
    QColor drawColor = (currentTool_ == ToolType::Eraser) ? Qt::white : currentColor_;
    int drawWidth = (currentTool_ == ToolType::Eraser) ? currentWidth_ * 3 : currentWidth_;

    switch (currentTool_) {
        case ToolType::Pencil:
            tool = new QMDrawPen(drawColor, drawWidth);
            break;
        case ToolType::Line:
            tool = new QMDrawLine(drawColor, drawWidth);
            break;
        case ToolType::Rect:
            tool = new QMDrawRect(drawColor, drawWidth, fillShape_);
            break;
        case ToolType::Ellipse:
            tool = new QMDrawEllipse(drawColor, drawWidth, fillShape_);
            break;
        case ToolType::Eraser:
            tool = new QMDrawPen(Qt::white, currentWidth_ * 3);
            break;
        default:
            break;
    }

    if (tool) {
        scene_->setActiveDrawObject(tool);
    }
}

void QMCanvasStudio::onPenWidthChanged(int width) {
    currentWidth_ = width;
    if (currentTool_ != ToolType::Hand) {
        recreateCurrentTool();
    }
}

void QMCanvasStudio::onColorChanged() {
    QColor color = QColorDialog::getColor(currentColor_, this, "选择颜色",
                                         QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
    if (color.isValid()) {
        currentColor_ = color;
        colorPreview_->setStyleSheet(QString("background-color: %1; border: 1px solid #666; border-radius: 3px;")
                                    .arg(color.name(QColor::HexArgb)));
        if (currentTool_ != ToolType::Hand && currentTool_ != ToolType::Eraser) {
            recreateCurrentTool();
        }
    }
}

void QMCanvasStudio::onFillChanged(bool checked) {
    fillShape_ = checked;
    if (currentTool_ == ToolType::Rect || currentTool_ == ToolType::Ellipse) {
        recreateCurrentTool();
    }
}

void QMCanvasStudio::onOpacityChanged(int value) {
    currentOpacity_ = value;
    opacityLabel_->setText(QString("%1%").arg(value));
}

void QMCanvasStudio::onAntiAliasChanged(bool checked) {
    antiAlias_ = checked;
    // QPainter::setRenderHint 应在绘制时调用，而非设置在 QWidget 上
    // 如需全局抗锯齿，应修改 Viewport::paintEvent 中的 painter 设置
    statusLabel_->setText(checked ? "抗锯齿已开启" : "抗锯齿已关闭");
}

void QMCanvasStudio::onHandToolPressed(QPoint pos) {
    if (currentTool_ == ToolType::Hand) {
        isHandDragging_ = true;
        handLastPos_ = pos;
        setCursor(Qt::ClosedHandCursor);
    }
}

void QMCanvasStudio::onHandToolMoved(QPoint pos) {
    if (isHandDragging_ && currentTool_ == ToolType::Hand) {
        QPoint delta = pos - handLastPos_;

        auto *view = canvasView_->findChild<QScrollArea*>();
        if (view) {
            QScrollBar *hBar = view->horizontalScrollBar();
            QScrollBar *vBar = view->verticalScrollBar();

            if (hBar) hBar->setValue(hBar->value() - delta.x());
            if (vBar) vBar->setValue(vBar->value() - delta.y());
        }

        handLastPos_ = pos;
    }
}

void QMCanvasStudio::onHandToolReleased() {
    if (isHandDragging_) {
        isHandDragging_ = false;
        if (currentTool_ == ToolType::Hand) {
            setCursor(Qt::OpenHandCursor);
        }
    }
}

void QMCanvasStudio::onNewLayer() {
    if (!scene_) return;
    scene_->finishActiveLayer();
    updateLayerList();
    statusLabel_->setText("已创建新图层");
}

void QMCanvasStudio::onDuplicateLayer() {
    QMessageBox::information(this, "提示", "当前版本暂不支持图层复制");
}

void QMCanvasStudio::onDeleteLayer() {
    QMessageBox::information(this, "提示", "当前版本暂不支持图层删除，可以使用“清空图层”功能");
}

void QMCanvasStudio::onMoveLayerUp() {
    if (!scene_) return;
    int current = layerList_->currentRow();
    if (current > 0) {
        int idx1 = layerList_->item(current)->data(Qt::UserRole).toInt();
        int idx2 = layerList_->item(current - 1)->data(Qt::UserRole).toInt();
        scene_->swapLayer(idx1, idx2);
        updateLayerList();
        layerList_->setCurrentRow(current - 1);
        statusLabel_->setText("图层上移");
    }
}

void QMCanvasStudio::onMoveLayerDown() {
    if (!scene_) return;
    int current = layerList_->currentRow();
    if (current < layerList_->count() - 1) {
        int idx1 = layerList_->item(current)->data(Qt::UserRole).toInt();
        int idx2 = layerList_->item(current + 1)->data(Qt::UserRole).toInt();
        scene_->swapLayer(idx1, idx2);
        updateLayerList();
        layerList_->setCurrentRow(current + 1);
        statusLabel_->setText("图层下移");
    }
}

void QMCanvasStudio::onMoveLayerToTop() {
    if (!scene_) return;
    int current = layerList_->currentRow();
    if (current > 0) {
        while (current > 0) {
            int idx = layerList_->item(current)->data(Qt::UserRole).toInt();
            int idxAbove = layerList_->item(current - 1)->data(Qt::UserRole).toInt();
            scene_->swapLayer(idx, idxAbove);
            current--;
        }
        updateLayerList();
        layerList_->setCurrentRow(0);
        statusLabel_->setText("图层已置顶");
    }
}

void QMCanvasStudio::onMoveLayerToBottom() {
    if (!scene_) return;
    int current = layerList_->currentRow();
    int count = layerList_->count();
    if (current < count - 1) {
        while (current < count - 1) {
            int idx = layerList_->item(current)->data(Qt::UserRole).toInt();
            int idxBelow = layerList_->item(current + 1)->data(Qt::UserRole).toInt();
            scene_->swapLayer(idx, idxBelow);
            current++;
        }
        updateLayerList();
        layerList_->setCurrentRow(count - 1);
        statusLabel_->setText("图层已置底");
    }
}

void QMCanvasStudio::onLayerVisibilityChanged(QListWidgetItem* item) {
    if (!scene_ || !item) return;
    int originalIndex = item->data(Qt::UserRole).toInt();
    bool visible = (item->checkState() == Qt::Checked);

    scene_->setLayerHide(originalIndex, !visible);
    statusLabel_->setText(QString("图层 %1 %2").arg(originalIndex + 1).arg(visible ? "显示" : "隐藏"));
}

void QMCanvasStudio::onLayerSelected() {
    int row = layerList_->currentRow();
    if (!scene_ || row < 0) return;

    int originalIndex = layerList_->item(row)->data(Qt::UserRole).toInt();
    scene_->switchLayer(originalIndex);
    updateLayerUIState();
}

void QMCanvasStudio::onLayerDoubleClicked(QListWidgetItem* item) {
    Q_UNUSED(item)
    onRenameLayer();
}

void QMCanvasStudio::onRenameLayer() {
    int row = layerList_->currentRow();
    if (row < 0) return;

    bool ok;
    QString text = QInputDialog::getText(this, "重命名图层", "图层名称:",
                                        QLineEdit::Normal, layerList_->item(row)->text(), &ok);
    if (ok && !text.isEmpty()) {
        layerList_->item(row)->setText(text);
    }
}

void QMCanvasStudio::onMergeDown() {
    statusLabel_->setText("图层合并需要扩展实现");
}

void QMCanvasStudio::onMergeAll() {
    statusLabel_->setText("合并全部需要扩展实现");
}

void QMCanvasStudio::onClearLayer() {
    if (!scene_) return;
    auto *layer = scene_->activeLayer();
    if (layer) {
        layer->clear();
        canvasView_->updateViewport();
        statusLabel_->setText("图层已清空");
    }
}

void QMCanvasStudio::onZoomChanged(int value) {
    if (!scene_) return;
    scene_->setRatio(value / 100.0);
    zoomLabel_->setText(QString("%1%").arg(value));
    zoomStatusLabel_->setText(QString("%1%").arg(value));
}

void QMCanvasStudio::onZoomIn() {
    if (!scene_) return;
    QRect viewport = scene_->getViewportRect();
    QPoint center(viewport.width()/2, viewport.height()/2);
    scene_->onScaleBy(true, center);
    syncZoomSlider();
}

void QMCanvasStudio::onZoomOut() {
    if (!scene_) return;
    QRect viewport = scene_->getViewportRect();
    QPoint center(viewport.width()/2, viewport.height()/2);
    scene_->onScaleBy(false, center);
    syncZoomSlider();
}

void QMCanvasStudio::onFitToWindow() {
    zoomSlider_->setValue(100);
    onZoomChanged(100);
    statusLabel_->setText("视图已适合窗口");
}

void QMCanvasStudio::onResetView() {
    zoomSlider_->setValue(100);
    onZoomChanged(100);
    if (scene_) {
        scene_->setRatio(1.0);
    }
    statusLabel_->setText("视图已重置");
}

void QMCanvasStudio::onGridToggled(bool visible) {
    showGrid_ = visible;
    if (scene_) {
        scene_->setShowGrid(visible);
        canvasView_->updateViewport();
    }
    gridCheck_->setChecked(visible);
    statusLabel_->setText(visible ? "显示网格" : "隐藏网格");
}

void QMCanvasStudio::onGridSizeChanged(int size) {
    if (scene_) {
        scene_->setGridSize(size);
        if (showGrid_) canvasView_->updateViewport();
    }
}

void QMCanvasStudio::onBgColorChanged() {
    QColor color = QColorDialog::getColor(bgColor_, this, "选择背景色");
    if (color.isValid()) {
        bgColor_ = color;
        canvasView_->setViewportBackground(color);
        statusLabel_->setText("背景色已更改");
    }
}

void QMCanvasStudio::onNewCanvas() {
    bool ok;
    int width = QInputDialog::getInt(this, "新建画布", "宽度:", 1200, 100, 10000, 1, &ok);
    if (!ok) return;
    int height = QInputDialog::getInt(this, "新建画布", "高度:", 800, 100, 10000, 1, &ok);
    if (!ok) return;

    bool transparent = QMessageBox::question(this, "透明背景", "是否创建透明背景画布？") == QMessageBox::Yes;
    createNewCanvas(QSize(width, height), transparent);
}

void QMCanvasStudio::onOpenImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开图像", "",
        "Images (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;All Files (*)");
    if (fileName.isEmpty()) return;

    QPixmap pixmap(fileName);
    if (pixmap.isNull()) {
        QMessageBox::critical(this, "错误", "无法加载图像文件");
        return;
    }

    createNewCanvas(pixmap.size());
    addImageAsLayer(pixmap, QFileInfo(fileName).fileName());
}

void QMCanvasStudio::onOpenAsLayer() {
    if (!scene_) return;
    QString fileName = QFileDialog::getOpenFileName(this, "作为图层打开", "",
        "Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)");
    if (fileName.isEmpty()) return;

    QPixmap pixmap(fileName);
    if (pixmap.isNull()) {
        QMessageBox::critical(this, "错误", "无法加载图像");
        return;
    }

    addImageAsLayer(pixmap, QFileInfo(fileName).fileName());
}

void QMCanvasStudio::onSave() {
    onExport();
}

void QMCanvasStudio::onExport() {
    if (!scene_) return;

    QString fileName = QFileDialog::getSaveFileName(this, "导出图像", "untitled.png",
        "PNG (*.png);;JPEG (*.jpg);;BMP (*.bmp);;TIFF (*.tiff)");
    if (fileName.isEmpty()) return;

    QPixmap pixmap = scene_->pixmap();
    if (pixmap.save(fileName)) {
        statusLabel_->setText(QString("已保存: %1").arg(fileName));
    } else {
        QMessageBox::critical(this, "错误", "保存文件失败");
    }
}

void QMCanvasStudio::onExportVisible() {
    onExport();
}

void QMCanvasStudio::onUndo() {
    if (!scene_) return;
    scene_->onCtrlAndZ();
    canvasView_->updateViewport();
    statusLabel_->setText("已撤销");
    updateLayerList(); // 撤销可能影响图层内容显示
}

void QMCanvasStudio::onRedo() {
    if (!scene_) return;
    scene_->onCtrlAndY();
    canvasView_->updateViewport();
    statusLabel_->setText("已重做");
}

void QMCanvasStudio::onCut() { statusLabel_->setText("剪切功能暂未实现"); }
void QMCanvasStudio::onCopy() { statusLabel_->setText("复制功能暂未实现"); }
void QMCanvasStudio::onPaste() { statusLabel_->setText("粘贴功能暂未实现"); }
void QMCanvasStudio::onSelectAll() { statusLabel_->setText("全选功能暂未实现"); }

void QMCanvasStudio::syncZoomSlider() {
    if (!scene_) return;
    int percent = static_cast<int>(scene_->ratio() * 100);
    zoomSlider_->blockSignals(true);
    zoomSlider_->setValue(qBound(5, percent, 500));
    zoomSlider_->blockSignals(false);
    zoomLabel_->setText(QString("%1%").arg(percent));
    zoomStatusLabel_->setText(QString("%1%").arg(percent));
}

void QMCanvasStudio::onSceneUpdated() {
}

void QMCanvasStudio::updateStatusInfo() {
}

void QMCanvasStudio::onMousePosChanged(QPoint pos) {
    if (scene_) {
        int x = static_cast<int>(pos.x() / scene_->ratio());
        int y = static_cast<int>(pos.y() / scene_->ratio());
        mousePosLabel_->setText(QString("X: %1, Y: %2").arg(x).arg(y));
    }
}

bool QMCanvasStudio::eventFilter(QObject *watched, QEvent *event) {
    if (watched != canvasView_->viewport()) {
        return QMainWindow::eventFilter(watched, event);
    }

    // 处理滚轮缩放（Ctrl+滚轮）
    if (event->type() == QEvent::Wheel) {
        auto *wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers() & Qt::ControlModifier) {
            QPoint numSteps = wheelEvent->angleDelta() / 8 / 15;
            if (qAbs(numSteps.y()) >= 1) {
                if (scene_) {
                    bool magnify = numSteps.y() > 0;
                    QPoint point = wheelEvent->position().toPoint();
                    scene_->onScaleBy(magnify, point);
                    syncZoomSlider();
                }
                return true; // 阻止事件继续传递（防止滚动视图）
            }
        }
        return false; // 非Ctrl滚轮正常传递
    }

    // 处理键盘快捷键（撤销/重做）- 作为备份机制
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Z) {
            onUndo();
            return true;
        }
        if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Y) {
            onRedo();
            return true;
        }
    }

    // 鼠标移动 - 更新坐标显示
    if (event->type() == QEvent::MouseMove) {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);
        onMousePosChanged(mouseEvent->pos());

        // 抓手工具拖动
        if (currentTool_ == ToolType::Hand && isHandDragging_) {
            onHandToolMoved(mouseEvent->pos());
            return true;
        }
    }

    // 抓手工具按下/释放
    if (currentTool_ == ToolType::Hand) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                onHandToolPressed(mouseEvent->pos());
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            auto *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                onHandToolReleased();
                // 注意：不返回true，让Viewport也处理以结束绘制状态
            }
        }
    }

    return false; // 其他事件正常传递
}