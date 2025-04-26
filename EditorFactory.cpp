/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include <QStandardPaths>
#include "EditorFactory.h"
#include "CodeEditor.h"
#include "EdiException.h"

IEditor *EditorFactory::createEditorTab(const QString& path) {
    auto *editor = new CodeEditor(path, this);
    editor->setRepository(repository, themeName);
    connect(editor->plainEdit, &PlainTextEdit::textChanged, this, &EditorFactory::onTextChanged);
    connect(editor->searchBar, &SearchBar::onTextChanged, this, &EditorFactory::onTextChanged);
    connect(editor, &CodeEditor::refreshRepository, this,  &EditorFactory::onRefreshRepository);
    connect(editor, &CodeEditor::setThemeName, this,  &EditorFactory::onChangeThemeName);
    editor->untitledId = untitledCounter.getNextId();
    tabWidget->addTab(editor, editor->getTitle());
    return editor;
}

int EditorFactory::getEditorCount() {
    return tabWidget->count();
}

IEditor *EditorFactory::getEditor(int index) {
    return dynamic_cast<CodeEditor *>(tabWidget->widget(index));
}

IEditor *EditorFactory::getCurrentEditor() {
    return dynamic_cast<CodeEditor *>(tabWidget->currentWidget());
}

void EditorFactory::closeEditor(int index) {
    auto* editor = dynamic_cast<CodeEditor *>(tabWidget->widget(index));
    tabWidget->removeTab(index);
    if (editor->path.isEmpty())
      untitledCounter.releaseId(editor->untitledId);
    else
      config->mru->add(editor->path);
    delete editor;
}

bool EditorFactory::tryCloseEditor(int index, IEditor::CloseEnum &canClose) {
    if (index==-1 && tabWidget->count()==0)
        return true; //true because stay no tab
    if (index<0 || index>=tabWidget->count())
        throw EdiException("tryCloseEditor: bad index");

    IEditor::ConsiderEnum consider = getEditor(index)->consider();
    if (consider==IEditor::coCanClose) {
        closeEditor(index);
        return true;
    }
    else {
        getEditor(index)->askSaveChangesBeforeClosing(canClose);
        if (canClose==IEditor::clClose || canClose==IEditor::clCloseAllSave || canClose==IEditor::clCloseAllDiscard) {
            closeEditor(index);
            return true;
        }
    }
    return false;
}

bool EditorFactory::tryCloseCurrentEditor() {
    IEditor::CloseEnum canClose = IEditor::clClose;
    return tryCloseEditor(tabWidget->currentIndex(), canClose);
}

bool EditorFactory::tryCloseAll() {
    IEditor::CloseEnum canClose = IEditor::clClose;
    for (int i=tabWidget->count()-1; i>=0; i--)
        if (!tryCloseEditor(tabWidget->currentIndex(), canClose)) return false;
    return true;
}

IEditor *EditorFactory::getEditorByPath(const QString &path) {
    for (int i=0; i<tabWidget->count(); i++) {
        auto* editor = dynamic_cast<IEditor *>(tabWidget->widget(i));
        if (editor->getPath()==path)
            return editor;
    }
    return nullptr;
}

void EditorFactory::onTextChanged() {
    IEditor* editor = getCurrentEditor();
    if (!editor) return;
    bool modified = editor->isModified();
    QColor color = modified? Qt::red : Qt::black;
    tabWidget->tabBar()->setTabTextColor(tabWidget->currentIndex(),color);
}

void EditorFactory::onRefreshRepository() {
    delete repository;
    repository = new KSyntaxHighlighting::Repository();
    for (int i=0; i<getEditorCount(); i++) {
        IEditor* editor = getEditor(i);
        editor->setRepository(repository, themeName);
    }
}

void EditorFactory::onChangeThemeName(QString newName) {
    themeName = newName;
    for (int i=0; i<getEditorCount(); i++) {
        IEditor* editor = getEditor(i);
        editor->setTheme(themeName);
    }
}

EditorFactory::EditorFactory(MruTabWidget *tabWidget):tabWidget(tabWidget) {
    repository = new KSyntaxHighlighting::Repository();
    config = new Config("EdiQ");
}

MRU *EditorFactory::getMRU() {
  return config->mru;
}
