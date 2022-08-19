//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H

#include <repository.h>
#include <QString>
#include <QPlainTextEdit>
#include <utility>
#include "IEditor.h"
#include "QMessageBox"
#include "CodeEditorSidebar.h"

namespace KSyntaxHighlighting
{
    class SyntaxHighlighter;
}

class CodeEditor : public QPlainTextEdit, public IEditor {
    QString path;
    int untitledId = 0;
    bool saveFile();
    CodeEditorSidebar *sideBar;
    KSyntaxHighlighting::Repository repository;
    KSyntaxHighlighting::SyntaxHighlighter *highlighter;
    void updateSidebarGeometry();
    void updateSidebarArea(const QRect &rect, int dy);
    void highlightCurrentLine();
    void setTheme(const KSyntaxHighlighting::Theme &theme);
protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    int sidebarWidth() const;
    void sidebarPaintEvent(QPaintEvent *event);
    QTextBlock blockAtPosition(int y) const;
    bool isFoldable(const QTextBlock &block) const;
    bool isFolded(const QTextBlock &block) const;
    void toggleFold(const QTextBlock &startBlock);
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
