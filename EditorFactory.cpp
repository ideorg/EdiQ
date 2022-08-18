//
// Created by andrzej on 8/17/22.
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

bool EditorFactory::tryCloseEditor(int index) {
    if (index<0 || index>=tabWidget->count())
        throw EdiException("tryCloseEditor: bad index");
    auto* editor = dynamic_cast<CodeEditor *>(tabWidget->widget(index));
    tabWidget->removeTab(index);
    untitledCounter.releaseId(editor->untitledId);
    delete editor;
    return true;
}

bool EditorFactory::tryCloseCurrentEditor() {
    return tryCloseEditor(tabWidget->currentIndex());
}

