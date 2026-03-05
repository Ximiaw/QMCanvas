//
// Created by m3311 on 2026/2/28.
//

#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QObject>
#include <QtCore>
#include <QRectF>
#include <QWidget>
#include <QScrollArea>
#include <QWheelEvent>
#include <QtOpenGLWidgets/QOpenGLWidget>

enum class WheelMode {
    ROLL,
    ZOOM
};

class View : public QScrollArea{
    Q_OBJECT

public:
    View(QWidget* parent=nullptr);
    ~View() override;

private:
    WheelMode wheelMode_ = WheelMode::ROLL;

public:
    void setWheelMode(WheelMode mode);
    [[nodiscard]] WheelMode wheelMode() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    void translation();
private:
    void renderScaled(QWheelEvent *event);
private://all
    void renderScaledA();
private://part
    void renderScaledP();
signals:
    void viewportChanged(QRectF rect);
    void zoomChanged(qreal zoom);
};



#endif //MAIN_VIEW_H