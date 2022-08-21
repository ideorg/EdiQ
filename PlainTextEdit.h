/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-20
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_PLAINTEXTEDIT_H
#define EDIQ_PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QRectF>

class PlainTextEdit : public QPlainTextEdit {
public:
    explicit PlainTextEdit(QWidget *parent = nullptr): QPlainTextEdit(parent){};
    QTextBlock firstVisibleBlock_pub() {
        return QPlainTextEdit::firstVisibleBlock();
    }
    QRectF blockBoundingGeometry_pub(const QTextBlock &block) const {
        return QPlainTextEdit::blockBoundingGeometry(block);
    }
    QPointF contentOffset_pub() const {
        return QPlainTextEdit::contentOffset();
    }
    QRectF blockBoundingRect_pub(const QTextBlock &block) const {
        return QPlainTextEdit::blockBoundingRect(block);
    }
    virtual void resizeEvent(QResizeEvent *e) override {
        return QPlainTextEdit::resizeEvent(e);
    }
};


#endif //EDIQ_PLAINTEXTEDIT_H
