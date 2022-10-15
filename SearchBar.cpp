/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-20
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <QPalette>
#include <QApplication>
#include "SearchBar.h"
#include "CodeEditor.h"
#include "Popup.h"

SearchBar::SearchBar(CodeEditor *editor)
        : QWidget(editor->plainEdit)
        , codeEditor(editor)
{
    setContentsMargins(0,0,0,0);
    addControls();
    popup = new Popup(editor);
    popup->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
        );
    connect(textToFind, &QLineEdit::textChanged, [=](){
        //QPoint bottomLeft = mapToGlobal(button->geometry().bottomLeft());
        //popup->popup(bottomLeft);
        search(textToFind->text());
    });
    qApp->installEventFilter(this);
}

void SearchBar::addControls() {
    auto *hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    button = new QPushButton("");
    button->setContentsMargins(0,0,0,0);
    button->setIcon(QIcon(":/svg/Magnifying_glass.svg"));
    hLayout->addWidget(button);
    textToFind = new QLineEdit(this);
    hLayout->addWidget(textToFind);
    auto *toolBar1 = new QToolBar(this);
    auto *btn1 = new QToolButton(toolBar1);
    btn1->setCheckable(true);
    btn1->setText("Cc");
    toolBar1->addWidget(btn1);
    auto *btn2 = new QToolButton(toolBar1);
    btn2->setCheckable(true);
    btn2->setText("W");
    toolBar1->addWidget(btn2);
    auto *btn3 = new QToolButton(toolBar1);
    btn3->setCheckable(true);
    btn3->setText(".*");
    toolBar1->addWidget(btn3);
    hLayout->addWidget(toolBar1);
    resultsCount = new QLabel(this);
    resultsCount->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultsCount->setText("0 results");
    hLayout->addWidget(resultsCount);
    auto *toolBar2 = new QToolBar(this);
    auto *btn4 = new QToolButton(toolBar2);
    btn4->setIcon(QIcon(":/svg/Arrow_up.svg"));
    btn4->setMaximumWidth(3*btn4->height()/4);
    toolBar2->addWidget(btn4);
    auto *btn5 = new QToolButton(toolBar2);
    btn5->setIcon(QIcon(":/svg/Arrow_down.svg"));
    btn5->setMaximumWidth(3*btn5->height()/4);
    toolBar2->addWidget(btn5);
    auto *btn6 = new QToolButton(toolBar2);
    toolBar2->addWidget(btn6);
    hLayout->addWidget(toolBar2);
    auto *toolBar3 = new QToolBar(this);
    hLayout->addWidget(toolBar3);
    auto *btn7 = new QToolButton(toolBar3);
    toolBar3->addWidget(btn7);
    btn7->setText("x");
    setLayout(hLayout);
}

void SearchBar::search(const QString &text) {
    int n = codeEditor->search(text);
    if (n==1)
        resultsCount->setText("1 result");
    else
        resultsCount->setText(QString::number(n)+" results");
}
