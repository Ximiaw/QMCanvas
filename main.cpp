#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QColorDialog>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QMessageBox>

// 自定义绘制对象：自由画笔工具
class PenDrawObject : public QMDrawObject {
    Q_OBJECT
public:
    PenDrawObject(QColor color = Qt::black, int width = 3)
        : color_(color), penWidth_(width) {
        pen_ = QPen(color_, penWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    void draw(QPainter* painter) override {
        if (points_.size() < 2) return;
        painter->setPen(pen_);
        for (int i = 1; i < points_.size(); ++i) {
            painter->drawLine(points_[i-1], points_[i]);
        }
    }

    void begin(QPoint point) override {
        points_.clear();
        points_.append(point);
        isRecord_ = true;
        begin_ = point;
        end_ = point;
    }

    void recordPoint(QPoint point) override {
        if (isRecord_) {
            points_.append(point);
            end_ = point;
            record_ = point;
        }
    }

    void end(QPoint point) override {
        if (isRecord_) {
            points_.append(point);
            end_ = point;
            isRecord_ = false;
        }
    }

    QPen* pen() const override { return const_cast<QPen*>(&pen_); }
    QBrush* brush() const override { return nullptr; }

    void setColor(QColor color) {
        color_ = color;
        pen_.setColor(color_);
    }
    void setWidth(int width) {
        penWidth_ = width;
        pen_.setWidth(width);
    }

private:
    QVector<QPoint> points_;
    QColor color_;
    int penWidth_;
    QPen pen_;
};

// 绘制矩形工具
class RectDrawObject : public QMDrawObject {
    Q_OBJECT
public:
    RectDrawObject(QColor color = Qt::blue, int width = 2, bool fill = false)
        : color_(color), penWidth_(width), fill_(fill) {
        pen_ = QPen(color_, penWidth_);
        brush_ = fill ? QBrush(color_.lighter(180)) : QBrush(Qt::NoBrush);
    }

    void draw(QPainter* painter) override {
        painter->setPen(pen_);
        painter->setBrush(brush_);
        QRect rect(begin_, end_);
        painter->drawRect(rect.normalized());
    }

    void begin(QPoint point) override {
        isRecord_ = true;
        begin_ = point;
        end_ = point;
    }

    void recordPoint(QPoint point) override {
        if (isRecord_) {
            end_ = point;
            record_ = point;
        }
    }

    void end(QPoint point) override {
        if (isRecord_) {
            end_ = point;
            isRecord_ = false;
        }
    }

    QPen* pen() const override { return const_cast<QPen*>(&pen_); }
    QBrush* brush() const override { return const_cast<QBrush*>(&brush_); }

private:
    QColor color_;
    int penWidth_;
    bool fill_;
    QPen pen_;
    QBrush brush_;
};

class CanvasWindow : public QWidget {
    Q_OBJECT
public:
    CanvasWindow(QWidget* parent = nullptr) : QWidget(parent), layerCount_(1) {
        setWindowTitle("QMCanvas 小画板 - 多层绘图演示");
        resize(900, 700);

        // 创建画布视图和场景
        canvasView_ = new QMCanvasView(this);
        QPixmap basePixmap(1200, 1000);
        basePixmap.fill(Qt::white);
        scene_ = new QMCanvasScene(basePixmap);
        canvasView_->setCanvasScene(scene_);

        // 配置场景参数
        scene_->setFactor(1.1);  // 每次缩放 10%
        scene_->setExtraViewportMargin(1.2);  // 视口外扩 20%

        // 初始画笔工具
        currentColor_ = Qt::black;
        currentTool_ = "画笔";
        setupUI();
        setupTools();

        // 连接保存信号
        connect(canvasView_->viewport(), &Viewport::save, this, &CanvasWindow::saveImage);
    }

private:
    // 辅助函数：获取 viewport 指针并调用 update
    void refreshCanvas() {
        Viewport* vp = const_cast<Viewport*>(canvasView_->viewport());
        if (vp) vp->update();
    }

    void setupUI() {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        // 工具栏
        QHBoxLayout* toolLayout = new QHBoxLayout();

        // 工具选择
        QLabel* toolLabel = new QLabel("工具:");
        toolCombo_ = new QComboBox();
        toolCombo_->addItems({"画笔", "矩形", "橡皮擦"});
        connect(toolCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &CanvasWindow::onToolChanged);

        // 颜色选择
        QPushButton* colorBtn = new QPushButton("选择颜色");
        colorBtn->setStyleSheet("background-color: black; color: white;");
        connect(colorBtn, &QPushButton::clicked, [this, colorBtn]() {
            QColor color = QColorDialog::getColor(currentColor_, this);
            if (color.isValid()) {
                currentColor_ = color;
                colorBtn->setStyleSheet(QString("background-color: %1;").arg(color.name()));
                setupTools();
            }
        });

        // 画笔粗细
        QLabel* widthLabel = new QLabel("粗细:");
        widthSpin_ = new QSpinBox();
        widthSpin_->setRange(1, 20);
        widthSpin_->setValue(3);
        connect(widthSpin_, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &CanvasWindow::setupTools);

        // 图层控制
        QLabel* layerLabel = new QLabel("图层:");
        layerCombo_ = new QComboBox();
        updateLayerCombo();
        connect(layerCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &CanvasWindow::onLayerSwitched);

        QPushButton* newLayerBtn = new QPushButton("新建图层");
        connect(newLayerBtn, &QPushButton::clicked, this, &CanvasWindow::addNewLayer);

        hideCheck_ = new QCheckBox("隐藏当前图层");
        connect(hideCheck_, &QCheckBox::toggled, this, &CanvasWindow::onLayerHideChanged);

        // 操作按钮
        QPushButton* undoBtn = new QPushButton("撤销 (Ctrl+Z)");
        QPushButton* redoBtn = new QPushButton("重做 (Ctrl+Y)");
        QPushButton* saveBtn = new QPushButton("保存 (Ctrl+S)");
        QPushButton* clearBtn = new QPushButton("清空当前层");

        connect(undoBtn, &QPushButton::clicked, this, [this]() {
            scene_->activeLayer()->undo();
            refreshCanvas();
        });
        connect(redoBtn, &QPushButton::clicked, this, [this]() {
            scene_->activeLayer()->redo();
            refreshCanvas();
        });
        connect(saveBtn, &QPushButton::clicked, this, &CanvasWindow::saveImage);
        connect(clearBtn, &QPushButton::clicked, this, &CanvasWindow::clearCurrentLayer);

        // 缩放控制
        QLabel* zoomLabel = new QLabel("缩放:");
        QPushButton* zoomInBtn = new QPushButton("+");
        QPushButton* zoomOutBtn = new QPushButton("-");
        zoomInBtn->setFixedWidth(30);
        zoomOutBtn->setFixedWidth(30);
        connect(zoomInBtn, &QPushButton::clicked, this, [this]() {
            scene_->setRatio(scene_->ratio() * 1.2);
        });
        connect(zoomOutBtn, &QPushButton::clicked, this, [this]() {
            scene_->setRatio(scene_->ratio() / 1.2);
        });

        // 组装工具栏
        toolLayout->addWidget(toolLabel);
        toolLayout->addWidget(toolCombo_);
        toolLayout->addSpacing(10);
        toolLayout->addWidget(colorBtn);
        toolLayout->addWidget(widthLabel);
        toolLayout->addWidget(widthSpin_);
        toolLayout->addSpacing(20);
        toolLayout->addWidget(layerLabel);
        toolLayout->addWidget(layerCombo_);
        toolLayout->addWidget(newLayerBtn);
        toolLayout->addWidget(hideCheck_);
        toolLayout->addSpacing(20);
        toolLayout->addWidget(undoBtn);
        toolLayout->addWidget(redoBtn);
        toolLayout->addWidget(saveBtn);
        toolLayout->addWidget(clearBtn);
        toolLayout->addStretch();
        toolLayout->addWidget(zoomLabel);
        toolLayout->addWidget(zoomOutBtn);
        toolLayout->addWidget(zoomInBtn);

        mainLayout->addLayout(toolLayout);
        mainLayout->addWidget(canvasView_);

        // 状态栏提示
        QHBoxLayout* statusLayout = new QHBoxLayout();
        statusLayout->addWidget(new QLabel("左键拖动: 绘制 | 滚轮: 滚动视图 | Ctrl+滚轮: 缩放 | Ctrl+Z/Y: 撤销/重做"));
        mainLayout->addLayout(statusLayout);
    }

    void setupTools() {
        // 根据当前工具创建对应的绘制对象模板
        if (currentTool_ == "画笔" || currentTool_ == "橡皮擦") {
            QColor color = (currentTool_ == "橡皮擦") ? Qt::white : currentColor_;
            auto penTool = new PenDrawObject(color, widthSpin_->value());
            scene_->setActiveDrawObject(penTool);
        } else if (currentTool_ == "矩形") {
            auto rectTool = new RectDrawObject(currentColor_, widthSpin_->value(), false);
            scene_->setActiveDrawObject(rectTool);
        }
    }

    void onToolChanged(int index) {
        QStringList tools = {"画笔", "矩形", "橡皮擦"};
        if (index >= 0 && index < tools.size()) {
            currentTool_ = tools[index];
            setupTools();
        }
    }

    void addNewLayer() {
        // 创建新图层并通过 scene_ 设置为活动图层
        // 注意：setActiveLayer 会创建 QSharedPointer 接管所有权
        Layer* newLayer = new Layer();
        scene_->setActiveLayer(newLayer);
        layerCount_++;
        updateLayerCombo();
        layerCombo_->setCurrentIndex(layerCount_ - 1);
    }

    void onLayerSwitched(int index) {
        if (index >= 0 && index < layerCount_) {
            scene_->switchLayer(index);
            // 更新隐藏复选框状态
            hideCheck_->blockSignals(true);
            hideCheck_->setChecked(scene_->isLayerHide(index));
            hideCheck_->blockSignals(false);
        }
    }

    void onLayerHideChanged(bool hide) {
        int currentIndex = layerCombo_->currentIndex();
        if (currentIndex >= 0 && currentIndex < layerCount_) {
            scene_->setLayerHide(currentIndex, hide);
            refreshCanvas();
        }
    }

    void updateLayerCombo() {
        layerCombo_->clear();
        for (int i = 0; i < layerCount_; ++i) {
            layerCombo_->addItem(QString("图层 %1").arg(i + 1));
        }
    }

    void clearCurrentLayer() {
        // 通过多次撤销清空当前图层（因为 AbstractLayer 没有直接清空接口）
        Layer* currentLayer = scene_->activeLayer();
        if (!currentLayer) return;

        // 获取当前图层中的对象数量
        int itemCount = currentLayer->items().size();
        if (itemCount == 0) {
            QMessageBox::information(this, "提示", "当前图层已是空的");
            return;
        }

        // 执行多次撤销以清空
        for (int i = 0; i < itemCount; ++i) {
            currentLayer->undo();
        }
        refreshCanvas();
        QMessageBox::information(this, "提示", QString("已清空 %1 个绘制对象").arg(itemCount));
    }

    void saveImage() {
        QString fileName = QFileDialog::getSaveFileName(this, "保存图片", "drawing.png", "PNG 图片 (*.png);;JPEG 图片 (*.jpg)");
        if (!fileName.isEmpty()) {
            QPixmap pixmap = scene_->pixmap();
            if (pixmap.save(fileName)) {
                QMessageBox::information(this, "成功", "图片已保存到: " + fileName);
            } else {
                QMessageBox::warning(this, "错误", "保存失败");
            }
        }
    }

    QMCanvasView* canvasView_;
    QMCanvasScene* scene_;
    QComboBox* toolCombo_;
    QComboBox* layerCombo_;
    QCheckBox* hideCheck_;
    QSpinBox* widthSpin_;
    QColor currentColor_;
    QString currentTool_;
    int layerCount_;  // 自行维护图层数量，因为无法直接访问 LayerManager
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 设置应用程序样式
    a.setStyle("Fusion");

    CanvasWindow window;
    window.show();

    return a.exec();
}