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

#include "repository.h"
#include <QString>
#include <QPlainTextEdit>
#include <utility>
#include "IEditor.h"
#include "IEditorFactory.h"
#include "QMessageBox"
#include "CodeEditorSidebar.h"
#include "SearchBar.h"
#include "PlainTextEdit.h"

namespace KSyntaxHighlighting
{
    class SyntaxHighlighter;
}

class CodeEditor : public QWidget, public IEditor {
Q_OBJECT
    QString path;
    IEditorFactory *editorFactory;
    std::string eenPassword;
    int untitledId = 0;
    bool saveFile();
    CodeEditorSidebar *sideBar;
    SearchBar *searchBar;
    KSyntaxHighlighting::Repository* repository = nullptr;
    KSyntaxHighlighting::SyntaxHighlighter *highlighter = nullptr;
    void updateSidebarGeometry();
    void updateSidebarArea(const QRect &rect, int dy);
    void highlightCurrentLine();
    void setTheme(const KSyntaxHighlighting::Theme &theme);
public:
    explicit CodeEditor(QString path, IEditorFactory *editorFactory);
    PlainTextEdit *plainEdit;
    void clearSearch();
    void search(const QString &searchString);
    void setRepository(KSyntaxHighlighting::Repository *repository, QString themeName) override;
    void setTheme(QString themeName) override;
public:
    void contextMenuEvent(QContextMenuEvent *event) override;
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
signals:
    void refreshRepository();
    void setThemeName(QString themeName);
};


#endif //EDIQ_CODEEDITOR_H
