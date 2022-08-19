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
#include "CodeEditorSidebar.h"

class CodeEditor : public QPlainTextEdit, public IEditor {
    QString path;
    int untitledId = 0;
    bool saveFile();
    CodeEditorSidebar *sideBar;
    void updateSidebarGeometry();
    void updateSidebarArea(const QRect &rect, int dy);
public:
    int sidebarWidth() const {return 20;};
    void sidebarPaintEvent(QPaintEvent *event);
    friend class EditorFactory;
    explicit CodeEditor(QString path);
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
