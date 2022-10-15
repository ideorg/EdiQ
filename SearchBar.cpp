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
    connect(textToFind, &QLineEdit::textChanged, this, &SearchBar::search);
    connect(eraseSearchButton, &QToolButton::clicked, [=](){
        textToFind->setText("");
        search();
    });
    connect(caseSensitiveButton, &QToolButton::clicked, this, &SearchBar::search);
    connect(wholeWordsButton, &QToolButton::clicked, this, &SearchBar::search);
    connect(regExpButton, &QToolButton::clicked, this, &SearchBar::search);
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
    eraseSearchButton = new QToolButton(toolBar1);
    eraseSearchButton->setText("x");
    toolBar1->addWidget(eraseSearchButton);
    caseSensitiveButton = new QToolButton(toolBar1);
    caseSensitiveButton->setCheckable(true);
    caseSensitiveButton->setText("Cc");
    toolBar1->addWidget(caseSensitiveButton);
    wholeWordsButton = new QToolButton(toolBar1);
    wholeWordsButton->setCheckable(true);
    wholeWordsButton->setText("W");
    toolBar1->addWidget(wholeWordsButton);
    regExpButton = new QToolButton(toolBar1);
    regExpButton->setCheckable(true);
    regExpButton->setText(".*");
    toolBar1->addWidget(regExpButton);
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

void SearchBar::search() {
    int flags = 0;
    if (caseSensitiveButton->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if (wholeWordsButton->isChecked())
        flags |= QTextDocument::FindWholeWords;
    auto p = codeEditor->search(textToFind->text(), QTextDocument::FindFlag(flags), regExpButton->isChecked());
    if (p.second==0)
        resultsCount->setText("0 results");
    else
        resultsCount->setText(QString::number(p.first+1)+"/"+QString::number(p.second));
}
