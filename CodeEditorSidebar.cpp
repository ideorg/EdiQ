/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-19
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "CodeEditorSidebar.h"
#include "CodeEditor.h"
#include "QTextBlock"

CodeEditorSidebar::CodeEditorSidebar(CodeEditor *editor)
        : QWidget(editor->plainEdit)
        , codeEditor(editor)
{
}

QSize CodeEditorSidebar::sizeHint() const {
    return QSize(codeEditor->sidebarWidth(), 0);
}

void CodeEditorSidebar::paintEvent(QPaintEvent *event)
{
    codeEditor->sidebarPaintEvent(event);
}

void CodeEditorSidebar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos().x() >= width() - codeEditor->fontMetrics().lineSpacing()) {
        auto block = codeEditor->blockAtPosition(event->pos().y());
        if (!block.isValid() || !codeEditor->isFoldable(block)) {
            return;
        }
        codeEditor->toggleFold(block);
    }
    QWidget::mouseReleaseEvent(event);
}
