//
// Created by Andrzej Borucki on 2022-08-20
//

#ifndef EDIQ_SEARCHBAR_H
#define EDIQ_SEARCHBAR_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include "Popup.h"

class CodeEditor;

class SearchBar : public QWidget {
    CodeEditor *codeEditor;
    void addControls();
    Popup *popup;
    QPushButton *button;
public:
    explicit SearchBar(CodeEditor *editor);
};


#endif //EDIQ_SEARCHBAR_H
