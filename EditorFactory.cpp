//
// Created by andrzej on 8/17/22.
//

#include "EditorFactory.h"
#include "CodeEditor.h"

IEditor *EditorFactory::createEditorTab(const QString& path) {
    auto *editor = new CodeEditor(path);
    tabWidget->addTab(editor, editor->getTitle());
    return editor;
}

IEditor *EditorFactory::createEditorTab(int untitledId) {
    auto *editor = new CodeEditor("");
    editor->untitledId = untitledId;
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

