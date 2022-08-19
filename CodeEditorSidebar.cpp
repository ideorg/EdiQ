//
// Created by Andrzej Borucki on 2022-08-19
//

#include "CodeEditorSidebar.h"
#include "CodeEditor.h"

CodeEditorSidebar::CodeEditorSidebar(CodeEditor *editor)
        : QWidget(editor)
        , codeEditor(editor)
{
}

QSize CodeEditorSidebar::sizeHint() const {
    return QSize(codeEditor->sidebarWidth(), 0);
}
