//
// Created by Andrzej Borucki on 2022-08-17
//

#include "EditorFactory.h"
#include "CodeEditor.h"
#include "EdiException.h"

IEditor *EditorFactory::createEditorTab(const QString& path) {
    auto *editor = new CodeEditor(path);
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
    untitledCounter.releaseId(editor->untitledId);
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
