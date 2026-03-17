// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef DEMO_H
#define DEMO_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QShortcut>
#include <QKeySequence>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QScrollArea>
#include <QMenuBar>
#include <QStatusBar>
#include <QComboBox>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QColorDialog>

#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include "QMDrawPen.h"
#include "QMDrawRect.h"
#include "QMDrawPixmap.h"
#include "LayerManager.h"

// 工具类型枚举
enum class ToolType {
    Hand,
    Pencil,
    Line,
    Rect,
    Ellipse,
    Eraser
};

// 自定义椭圆绘制对象
class QMDrawEllipse : public QMDrawObject {
    QPen pen_;
    QBrush brush_;
    bool fill_;
public:
    explicit QMDrawEllipse(QColor color = Qt::green, int width = 2, bool fill = false);
    void draw(QPainter* painter) override;
    QPen* pen() override;
    QBrush* brush() override;
    QSharedPointer<QMDrawObject> clone() override;
};

// 自定义直线绘制对象
class QMDrawLine : public QMDrawObject {
    QPen pen_;
public:
    explicit QMDrawLine(QColor color = Qt::black, int width = 2);
    void draw(QPainter* painter) override;
    QPen* pen() override;
    QBrush* brush() override;
    QSharedPointer<QMDrawObject> clone() override;
};

// 支持网格显示的自定义场景
class GridCanvasScene : public QMCanvasScene {
    Q_OBJECT
public:
    explicit GridCanvasScene(QPixmap pixmap = QPixmap(500,500), QObject* parent = nullptr);

    void setShowGrid(bool show);
    void setGridSize(int size);
    void setGridColor(QColor color);

    void draw(QPainter* painter);

private:
    bool showGrid_ = false;
    int gridSize_ = 20;
    QColor gridColor_ = QColor(200, 200, 200, 100);
};

class QMCanvasStudio : public QMainWindow {
    Q_OBJECT

public:
    explicit QMCanvasStudio(QWidget *parent = nullptr);
    ~QMCanvasStudio() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onToolChanged(int toolId);
    void onPenWidthChanged(int width);
    void onColorChanged();
    void onFillChanged(bool checked);
    void onOpacityChanged(int value);
    void onAntiAliasChanged(bool checked);

    void onHandToolPressed(QPoint pos);
    void onHandToolMoved(QPoint pos);
    void onHandToolReleased();

    void onZoomIn();
    void onZoomOut();
    void onZoomChanged(int value);
    void onFitToWindow();
    void onResetView();
    void onGridToggled(bool visible);
    void onGridSizeChanged(int size);
    void onBgColorChanged();

    void onNewLayer();
    void onDuplicateLayer();
    void onDeleteLayer();
    void onMoveLayerUp();
    void onMoveLayerDown();
    void onMoveLayerToTop();
    void onMoveLayerToBottom();
    void onLayerVisibilityChanged(QListWidgetItem* item);
    void onLayerSelected();
    void onLayerDoubleClicked(QListWidgetItem* item);
    void onMergeDown();
    void onMergeAll();
    void onClearLayer();
    void onRenameLayer();

    void onNewCanvas();
    void onOpenImage();
    void onOpenAsLayer();
    void onSave();
    void onExport();
    void onExportVisible();

    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();

    void updateLayerList();
    void updateLayerUIState();
    void syncZoomSlider();
    void onSceneUpdated();
    void updateStatusInfo();
    void onMousePosChanged(QPoint pos);
    void recreateCurrentTool();  // 重新创建当前工具对象以支持连续绘制
    void updatePropertyPanelState(); // 根据当前工具更新属性面板状态

private:
    void setupUI();
    void setupMenuBar();
    void setupToolDock();
    void setupLayerDock();
    void setupPropertyDock();
    void setupNavigatorDock();
    void setupStatusBar();
    void setupShortcuts();
    void setupConnections();

    void createNewCanvas(const QSize &size, bool transparent = false);
    void addImageAsLayer(const QPixmap &pixmap, const QString &name);
    QString getToolName(ToolType tool) const;

    bool isHandDragging_ = false;
    QPoint handLastPos_;

    ToolType currentTool_ = ToolType::Pencil;

    QColor currentColor_ = QColor(0, 0, 0);
    int currentWidth_ = 3;
    bool fillShape_ = false;
    bool antiAlias_ = true;
    int currentOpacity_ = 100;
    bool showGrid_ = false;
    QColor bgColor_ = Qt::white;

    QMCanvasView *canvasView_ = nullptr;
    GridCanvasScene *scene_ = nullptr;

    QButtonGroup *toolGroup_ = nullptr;
    QToolButton *btnHand_ = nullptr;
    QToolButton *btnPencil_ = nullptr;
    QToolButton *btnLine_ = nullptr;
    QToolButton *btnRect_ = nullptr;
    QToolButton *btnEllipse_ = nullptr;
    QToolButton *btnEraser_ = nullptr;

    QSpinBox *widthSpin_ = nullptr;
    QSlider *widthSlider_ = nullptr;
    QPushButton *colorBtn_ = nullptr;
    QLabel *colorPreview_ = nullptr;
    QCheckBox *fillCheck_ = nullptr;
    QCheckBox *antiAliasCheck_ = nullptr;
    QSlider *opacitySlider_ = nullptr;
    QLabel *opacityLabel_ = nullptr;
    QComboBox *blendModeCombo_ = nullptr;
    QPushButton *bgColorBtn_ = nullptr;

    QListWidget *layerList_ = nullptr;
    QPushButton *btnNewLayer_ = nullptr;
    QPushButton *btnDupLayer_ = nullptr;
    QPushButton *btnDelLayer_ = nullptr;
    QPushButton *btnMoveUp_ = nullptr;
    QPushButton *btnMoveDown_ = nullptr;
    QPushButton *btnMoveTop_ = nullptr;
    QPushButton *btnMoveBottom_ = nullptr;
    QPushButton *btnMergeDown_ = nullptr;
    QPushButton *btnMergeAll_ = nullptr;
    QPushButton *btnClearLayer_ = nullptr;

    QSlider *zoomSlider_ = nullptr;
    QLabel *zoomLabel_ = nullptr;
    QLabel *navigatorPreview_ = nullptr;
    QSpinBox *gridSizeSpin_ = nullptr;
    QCheckBox *gridCheck_ = nullptr;

    QLabel *statusLabel_ = nullptr;
    QLabel *mousePosLabel_ = nullptr;
    QLabel *canvasSizeLabel_ = nullptr;
    QLabel *zoomStatusLabel_ = nullptr;
    QLabel *layerCountLabel_ = nullptr;
};

#endif // DEMO_H