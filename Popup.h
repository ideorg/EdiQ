/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-20
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_POPUP_H
#define EDIQ_POPUP_H

#include <QListWidget>

class Popup : public QListWidget {
Q_OBJECT
    QWidget *parent;
    void installFilter();
    int outFocusCounter = 0;
    bool leftButtonPressed;
    bool dragStart = false;
    QPoint dragPos;
    int startWidth, startHeight;
    bool isDragPos(const QPoint &pos);
    const int MAX_HISTORY = 40;
public:
    explicit Popup(QWidget *parent);
    void popup(QPoint bottomLeft);
    QString selectedText;
    void select();
    void addToHistory(QString str);
signals:
    void selectSignal();
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    void focusChanged(bool in);
    //https://gist.github.com/Nico-Duduf/b8c799f1f2a694732abd1238843b1d70
    void mouseHover(QHoverEvent*);
    void mouseLeave(QEvent*);
    bool mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent *e);
    bool mouseMove(QMouseEvent*);
    void updateCursorShape(const QPoint &);
};


#endif //EDIQ_POPUP_H
