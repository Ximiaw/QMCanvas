//
// Created by m3311 on 2026/2/28.
//

#ifndef MAIN_QMCANVASVIEW_H
#define MAIN_QMCANVASVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QWheelEvent>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QRectF>

enum class WheelMode {
    ROLL,
    ZOOM
};

class QMCanvasView : private QScrollArea{
    Q_PROPERTY(WheelMode wheel_mode WHITE setWheelMode READ wheelMode)
private:
    WheelMode wheel_mode_ = WheelMode::ROLL;
public:
    void setWheelMode(WheelMode);
    [[nodiscard]] WheelMode wheelMode() const;

public:
    QMCanvasView();
    ~QMCanvasView() override;
    QWidget* viewport() const;

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