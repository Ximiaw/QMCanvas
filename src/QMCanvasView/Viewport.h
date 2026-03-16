// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_VIEWPORT_H
#define MAIN_VIEWPORT_H

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QPointer>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

class QMCanvasScene;

class Viewport :public QWidget{
    Q_OBJECT
private:
    QPointer<QMCanvasScene> scenePointer_;
public:
    Viewport(QWidget* parent=nullptr);
    ~Viewport() override = default;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
signals:
    void mouseMove(QPoint point);
    void mouseRelease(QPoint point);
    void mousePress(QPoint point);
    void ctrlAndZ();
    void ctrlAndY();
    void save();//ctrl+s会触发，保存当前状态还是导出图片需要自行实现
public slots:
    void onSceneChanged(QMCanvasScene* scene);
    void onRectChanged();
    void onPixmapChanged();
};

#endif //MAIN_VIEWPORT_H