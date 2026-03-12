//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_VIEWPORT_H
#define MAIN_VIEWPORT_H

#include <QObject>
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
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
public slots:
    void onSceneChanged(QMCanvasScene* scene);
    void onRectChanged();
    void onPixmapChanged();
};

#endif //MAIN_VIEWPORT_H