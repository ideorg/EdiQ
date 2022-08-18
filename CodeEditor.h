//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H


#include <QString>
#include <QPlainTextEdit>
#include <utility>
#include "IEditor.h"
#include "QMessageBox"

class CodeEditor : public QPlainTextEdit, public IEditor {
    QString path;
    int untitledId = 0;
    bool saveFile();
public:
    friend class EditorFactory;
    explicit CodeEditor(QString path): path(std::move(path)) {}
    ~CodeEditor() {QMessageBox::warning(this, "info", "delete "+getTitle());}
    QString getTitle() override;
    bool isModified() override;
    bool isEmpty() override;
    void openFile() override;
    bool save() override;
    bool saveAs() override;
    ConsiderEnum consider() override;
    void askSaveChangesBeforeClosing(CloseEnum &canClose) override;
};


#endif //EDIQ_CODEEDITOR_H
