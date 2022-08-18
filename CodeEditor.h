//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H


#include <QString>
#include <QPlainTextEdit>
#include <utility>
#include "IEditor.h"

class CodeEditor : public QPlainTextEdit, public IEditor {
    QString path;
public:
    explicit CodeEditor(QString path): path(std::move(path)) {}
    QString getTitle() override;
    void openFile() override;
    void save() override;
    void saveAs(const QString &fileName) override;
};


#endif //EDIQ_CODEEDITOR_H
