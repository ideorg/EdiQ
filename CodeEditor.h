//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H


#include <QString>
#include <QPlainTextEdit>
#include "IEditor.h"

class CodeEditor : public QPlainTextEdit, public IEditor {
    QString path;
public:
    void openFile(const QString &fileName) override;
    void save() override;
    void saveAs(const QString &fileName) override;
};


#endif //EDIQ_CODEEDITOR_H
