//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H


#include <QString>
#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit {
public:
    void openFile(const QString &fileName);
};


#endif //EDIQ_CODEEDITOR_H
