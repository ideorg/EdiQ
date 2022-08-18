//
// Created by andrzej on 8/17/22.
//

#include "EditorFactory.h"
#include "CodeEditor.h"

IEditor *EditorFactory::createEditorTab(const QString& title) {
    CodeEditor *editor = new CodeEditor();
    tabWidget->addTab(editor, title);
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
