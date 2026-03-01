//
// Created by m3311 on 2026/2/28.
//

#ifndef MAIN_QMCANVASVIEW_H
#define MAIN_QMCANVASVIEW_H

#include <QObject>
#include <QtCore>
#include <QWidget>
#include <QScrollArea>
#include <QWheelEvent>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QRectF>
#include <QScopedPointer>
#include <QSharedPointer>

class QMCanvasScene;
class Viewport;

enum class WheelMode {
    ROLL,
    ZOOM
};

class QMCanvasView : public QScrollArea{
    Q_OBJECT

    Q_ENUM(WheelMode)
    Q_PROPERTY(WheelMode wheelMode READ wheelMode WRITE setWheelMode NOTIFY wheelModeChanged)
    Q_PROPERTY(const Viewport* viewport READ viewport)
    Q_PROPERTY(QMCanvasScene* canvasScene READ canvasScene WRITE setCanvasScene NOTIFY canvasSceneChanged)

public:
    QMCanvasView(QWidget* parent=nullptr);
    QMCanvasView(QMCanvasScene* scene,QWidget* parent=nullptr);
    ~QMCanvasView() override;

private:
    WheelMode wheelMode_ = WheelMode::ROLL;
    QScopedPointer<Viewport> viewportPointer_;
    QSharedPointer<QMCanvasScene> scenePointer_;

public:
    void setWheelMode(WheelMode mode);
    [[nodiscard]] WheelMode wheelMode() const;
    void setCanvasScene(QMCanvasScene* scene);
    [[nodiscard]] QMCanvasScene* canvasScene() const;
    const Viewport* viewport() const;

signals:
    void wheelModeChanged(WheelMode mode);
    void canvasSceneChanged(QMCanvasScene* scene);

protected:
    void wheelEvent(QWheelEvent *event) override;

private://tool

private:
    void renderScaled(QWheelEvent *event);
private://all
    void renderScaledA();
private://part
    void renderScaledP();
};



#endif //MAIN_QMCANVASVIEW_H