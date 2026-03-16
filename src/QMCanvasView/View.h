// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QObject>
#include <QtCore>
#include <QPoint>
#include <QSize>
#include <QRectF>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMetaObject>

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
    WheelMode wheelMode() const;
protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    void factor(QWheelEvent *event);
public slots:
    void onPropertyChanged(QPoint point,QSize size);
signals:
    void viewportChanged(QRectF rect);
    void scaleFactorChanged(bool magnify,QPoint point);
};



#endif //MAIN_VIEW_H