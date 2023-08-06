#include "PlainTextEdit.h"
#include "CodeEditor.h"

void PlainTextEdit::contextMenuEvent(QContextMenuEvent *event) {
    dynamic_cast<CodeEditor*>(parent())->contextMenuEvent(event);
}
