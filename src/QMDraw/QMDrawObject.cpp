// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMDrawObject.h"

void QMDrawObject::begin(QPoint point){
    isRecord_ = true;
    begin_ = QPoint(point);
}

void QMDrawObject::recordPoint(QPoint point){
    if (isRecord_){
        record_ = QPoint(point);
        end_ = QPoint(point);
    }
}

void QMDrawObject::end(QPoint point){
    isRecord_ = false;
    end_ = QPoint(point);
}
