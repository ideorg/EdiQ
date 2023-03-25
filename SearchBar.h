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
#include <QTextDocument>
#include "Popup.h"

class CodeEditor;

struct SearchState {
    int currResult = 0;
    int resCount = 0;
    QTextDocument::FindFlag findFlag;
    bool isRegExp;
};

class SearchBar : public QWidget {
Q_OBJECT
    CodeEditor *codeEditor;
    void addControls();
    Popup *popup;
    QPushButton *button;
    QLabel *resultsCount;
    QToolButton *eraseSearchButton;
    QToolButton *caseSensitiveButton;
    QToolButton *wholeWordsButton;
    QToolButton *regExpButton;
    QToolButton *closeButton;
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent( QShowEvent* event ) override;
public:
    explicit SearchBar(CodeEditor *editor);
    SearchState searchState;
    QLineEdit *textToFind;
    void search();
    void closeSearch();
signals:
    void onTextChanged();
};


#endif //EDIQ_SEARCHBAR_H
