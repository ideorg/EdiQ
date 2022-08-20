//
// Created by Andrzej Borucki on 2022-08-20
//

#ifndef EDIQ_POPUP_H
#define EDIQ_POPUP_H

#include <QListWidget>

class Popup : public QListWidget {
    QWidget *parent;
    void installFilter();
    int outFocusCounter = 0;
    bool leftButtonPressed;
    int _borderWidth = 5;
    bool dragStart = false;
    QPoint dragPos;
    int startWidth, startHeight;
    enum Edge {
        None = 0x0,
        Left = 0x1,
        Top = 0x2,
        Right = 0x4,
        Bottom = 0x8,
        TopLeft = 0x10,
        TopRight = 0x20,
        BottomLeft = 0x40,
        BottomRight = 0x80,
    };
    Q_ENUM(Edge);
    Q_DECLARE_FLAGS(Edges, Edge);
    bool isDragPos(const QPoint &pos);
public:
    explicit Popup(QWidget *parent);
    void popup(QPoint bottomLeft);
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
    void calculateCursorPosition(const QPoint &, const QRect &, Edges &);
};


#endif //EDIQ_POPUP_H
