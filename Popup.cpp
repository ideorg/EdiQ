/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-20
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "Popup.h"
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QScrollBar>

int select();

Popup::Popup(QWidget *parent) : QListWidget(parent), parent(parent) {
    leftButtonPressed = false;
}

bool Popup::eventFilter(QObject *obj, QEvent *e) {
    Q_UNUSED(obj)
    if (e->type() == QEvent::MouseMove ||
        e->type() == QEvent::HoverMove ||
        e->type() == QEvent::Leave ||
        e->type() == QEvent::MouseButtonPress ||
        e->type() == QEvent::MouseButtonDblClick ||
        e->type() == QEvent::MouseButtonRelease) {
        switch (e->type()) {
            case QEvent::MouseButtonDblClick:
                select();
                return false;
            case QEvent::MouseMove:
                return mouseMove(static_cast<QMouseEvent*>(e));
                break;
            case QEvent::Leave:
                mouseLeave(e);
                return false;
                break;
            case QEvent::MouseButtonPress:
                return mousePress(static_cast<QMouseEvent*>(e));
                break;
            case QEvent::MouseButtonRelease:
                mouseRelease(static_cast<QMouseEvent *>(e));
                return false;
                break;
            default: return false;
        }
    }
    else if (e->type() == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key()==Qt::Key_Enter) {
            select();
        }
        return false;
    }
    else {
        if (e->type() == QEvent::FocusIn)
            focusChanged(true);
        else if (e->type() == QEvent::FocusOut)
            focusChanged(false);
        return false;
    }
}

void Popup::select() {
    selectedText = selectedItems()[0]->text();
    hide();
    emit selectSignal();
}

void Popup::focusChanged(bool in) {
    if (!in) {
        outFocusCounter++;
        if (outFocusCounter>=4) {
            qApp->removeEventFilter(this);
            hide();
        }
    }
}

void Popup::popup(QPoint bottomLeft) {
    show();
    setGeometry(QRect(bottomLeft, QSize(200,200)));
    installFilter();
}

void Popup::installFilter() {
    outFocusCounter = 0;
    qApp->installEventFilter(this);
}

void Popup::mouseHover(QHoverEvent *e) {
    updateCursorShape(e->pos());
}

void Popup::mouseLeave(QEvent *e) {
    if (!leftButtonPressed) {
        unsetCursor();
    }
}

bool Popup::mousePress(QMouseEvent *e) {
    if (e->button() & Qt::LeftButton) {
        leftButtonPressed = true;
        if (isDragPos(e->pos())) {
            dragStart = true;
            dragPos = e->pos();
            startWidth = width();
            startHeight = height();
            return true;
        }
    }
    return false;
}

void Popup::mouseRelease(QMouseEvent *e) {
    if (e->button() & Qt::LeftButton) {
        leftButtonPressed = false;
        dragStart = false;
    }
}

bool Popup::mouseMove(QMouseEvent *e) {
    if (leftButtonPressed) {
        if (dragStart) {
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect geometry = screen->availableGeometry();
            if (e->globalPos().x() >= geometry.right()-verticalScrollBar()->width()) return false;
            if (e->globalPos().y() >= geometry.bottom()-horizontalScrollBar()->height()) return false;
            int newW = e->pos().x() - dragPos.x() + startWidth;
            int newH = e->pos().y() - dragPos.y() + startHeight;
            if (newW <= verticalScrollBar()->width()) return false;
            if (newH <= horizontalScrollBar()->height()) return false;
            resize(newW, newH);
            return true;
        }
    }
    else
        updateCursorShape(e->pos());
    return false;
}

void Popup::updateCursorShape(const QPoint &pos) {
    if (isDragPos(pos))
        setCursor(Qt::SizeFDiagCursor);
    else
        unsetCursor();
}

bool Popup::isDragPos(const QPoint &pos) {
    return pos.x()>width()-verticalScrollBar()->width()
        && pos.y()>height()-horizontalScrollBar()->height();
}

void Popup::addToHistory(QString str) {
    if (str.isEmpty()) return;
    int idxA = -1;
    int idxB = -1;
    for (int i=0; i<count(); i++) {
        QString str1 = item(i)->text();
        if (str.startsWith(str1)) {
            idxA = i;
            break;
        }
        if (str1.startsWith(str)) {
            idxB = i;
            break;
        }
    }
    if (idxA >= 0)
        takeItem(idxA);
    else if (idxB >= 0)
        return;
    else if (count()>=MAX_HISTORY)
        takeItem(MAX_HISTORY-1);

    assert(count()<MAX_HISTORY);
    insertItem(0,str);
}
