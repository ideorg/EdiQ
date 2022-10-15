/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-20
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_SEARCHBAR_H
#define EDIQ_SEARCHBAR_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QToolButton>
#include "Popup.h"

class CodeEditor;

class SearchBar : public QWidget {
    CodeEditor *codeEditor;
    void addControls();
    Popup *popup;
    QPushButton *button;
    QLineEdit *textToFind;
    QLabel *resultsCount;
    QToolButton *caseSensitiveButton;
    QToolButton *wholeWordsButton;
    void search();
public:
    explicit SearchBar(CodeEditor *editor);
};


#endif //EDIQ_SEARCHBAR_H
