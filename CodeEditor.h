/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_CODEEDITOR_H
#define EDIQ_CODEEDITOR_H

#include <repository.h>
#include <QString>
#include <QPlainTextEdit>
#include <utility>
#include "IEditor.h"
#include "QMessageBox"
#include "CodeEditorSidebar.h"
#include "SearchBar.h"
#include "PlainTextEdit.h"

namespace KSyntaxHighlighting
{
    class SyntaxHighlighter;
}

class CodeEditor : public QWidget, public IEditor {
    QString path;
    std::string eenPassword;
    int untitledId = 0;
    bool saveFile();
    CodeEditorSidebar *sideBar;
    SearchBar *searchBar;
    KSyntaxHighlighting::Repository repository;
    KSyntaxHighlighting::SyntaxHighlighter *highlighter;
    void updateSidebarGeometry();
    void updateSidebarArea(const QRect &rect, int dy);
    void highlightCurrentLine();
    void setTheme(const KSyntaxHighlighting::Theme &theme);
public:
    explicit CodeEditor(QString path);
    PlainTextEdit *plainEdit;
    void clearSearch();
    void search(const QString &searchString);
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
public:
    int sidebarWidth() const;
    void sidebarPaintEvent(QPaintEvent *event);
    QTextBlock blockAtPosition(int y) const;
    bool isFoldable(const QTextBlock &block) const;
    bool isFolded(const QTextBlock &block) const;
    void toggleFold(const QTextBlock &startBlock);
    friend class EditorFactory;
    QString getTitle() override;
    bool isModified() override;
    bool isEmpty() override;
    void openFile() override;
    bool save() override;
    bool saveAs() override;
    ConsiderEnum consider() override;
    QString getPath() override;
    void askSaveChangesBeforeClosing(CloseEnum &canClose) override;
    void find() override;
    void findNext() override;
    void findPrevious() override;
    void setPlainFocus() override;
    void findNextPrevious(bool previous);
    void insertDate() override;
    void insertTime() override;
    void insertBoth() override;
};


#endif //EDIQ_CODEEDITOR_H
