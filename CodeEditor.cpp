//
// Created by Andrzej Borucki on 2022-08-17
//

#include "CodeEditor.h"
#include <definition.h>
#include <syntaxhighlighter.h>
#include <theme.h>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QPainter>
#include <QTextBlock>
#include <QMenu>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>

void CodeEditor::openFile() {
    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return;
    }
    const auto def = repository.definitionForFileName(path);
    highlighter->setDefinition(def);
    plainEdit->setPlainText(QString::fromUtf8(f.readAll()));
}

bool CodeEditor::saveFile() {
    assert(!path.isEmpty());
    QString text = plainEdit->toPlainText();
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return false;
    }
    f.write(text.toUtf8());
    return true;
}

bool CodeEditor::saveAs() {
    QFileDialog dialog(this, tr("Save File"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().first();
        return saveFile();
    }
    else
        return false;
}

bool CodeEditor::save() {
    if (path.isEmpty())
        return saveAs();
    else
        return saveFile();
}

QString CodeEditor::getTitle() {
    if (path.isEmpty())
        return "Untitled" + QString::number(untitledId);
    else {
        QFileInfo fileInfo(path);
        return fileInfo.fileName();
    }
}

bool CodeEditor::isModified() {
    return plainEdit->document()->isModified();
}

bool CodeEditor::isEmpty() {
    return plainEdit->document()->isEmpty();
}

IEditor::ConsiderEnum CodeEditor::consider() {
    if (!isModified() || isEmpty())
        return coCanClose;
    else if (!path.isEmpty())
        return coCanSave;
    else
        return coSaveAs;
}

void CodeEditor::askSaveChangesBeforeClosing(IEditor::CloseEnum &canClose) {
    if (isEmpty()) {
        canClose = clClose;
        return;
    }
    if (canClose == clCloseAllDiscard) return;
    if (canClose == clCloseAllSave) {
        if (!save())
            canClose = clCancel;
        return;
    }
    QMessageBox::StandardButton reply;
    QString message = "The text in the file " + getTitle() + " has been changed.\n"+
                               "Do you want to save the modifications? (No = close and discard changes)";

    reply = QMessageBox::question(this, "Warning", message,
                                  QMessageBox::Yes | QMessageBox::YesAll |
                                  QMessageBox::No | QMessageBox::NoToAll |
                                  QMessageBox::Cancel);
    switch(reply) {
        case QMessageBox::Yes: {
            if (save())
                canClose = clClose;
            else
                canClose = clNo;
            break;
        }
        case QMessageBox::YesToAll: {
            if (save())
                canClose = clCloseAllSave;
            else
                canClose = clNo;
            break;
        }
        case QMessageBox::No: canClose = clClose; break;
        case QMessageBox::NoToAll: canClose = clCloseAllDiscard; break;
        default: canClose = clCancel;
    }
}

void CodeEditor::updateSidebarGeometry() {
    sideBar->setFixedWidth(sidebarWidth());
}

CodeEditor::CodeEditor(QString path) : path(std::move(path)) {
    plainEdit = new PlainTextEdit(this);
    sideBar = new CodeEditorSidebar(this);
    searchBar = new SearchBar(this);
    auto *vLayout = new QVBoxLayout;
    auto *hLayout = new QHBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    hLayout->addWidget(sideBar);
    hLayout->addWidget(plainEdit);
    vLayout->addWidget(searchBar);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);
    highlighter = new KSyntaxHighlighting::SyntaxHighlighter(plainEdit->document());
    plainEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setTheme((palette().color(QPalette::Base).lightness() < 128) ? repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                 : repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    connect(plainEdit, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateSidebarGeometry);
    connect(plainEdit, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateSidebarArea);
    connect(plainEdit, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateSidebarGeometry();
    highlightCurrentLine();
}

void CodeEditor::sidebarPaintEvent(QPaintEvent *event) {
    QPainter painter(sideBar);
    painter.fillRect(event->rect(), highlighter->theme().editorColor(KSyntaxHighlighting::Theme::IconBorder));

    auto block = plainEdit->firstVisibleBlock_pub();
    auto blockNumber = block.blockNumber();
    int top = plainEdit->blockBoundingGeometry_pub(block).translated(plainEdit->contentOffset_pub()).top();
    int bottom = top + plainEdit->blockBoundingRect_pub(block).height();
    const int currentBlockNumber = plainEdit->textCursor().blockNumber();

    const auto foldingMarkerSize = fontMetrics().lineSpacing();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const auto number = QString::number(blockNumber + 1);
            painter.setPen(highlighter->theme().editorColor((blockNumber == currentBlockNumber) ? KSyntaxHighlighting::Theme::CurrentLineNumber             : KSyntaxHighlighting::Theme::LineNumbers));
            painter.drawText(2, top, sideBar->width(), fontMetrics().height(), Qt::AlignLeft, number);
        }

        // folding marker
        if (block.isVisible() && isFoldable(block)) {
            QPolygonF polygon;
            if (isFolded(block)) {
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.25);
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.75);
                polygon << QPointF(foldingMarkerSize * 0.8, foldingMarkerSize * 0.5);
            } else {
                polygon << QPointF(foldingMarkerSize * 0.25, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.75, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.5, foldingMarkerSize * 0.8);
            }
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CodeFolding)));
            painter.translate(sideBar->width() - foldingMarkerSize, top);
            painter.drawPolygon(polygon);
            painter.restore();
        }

        block = block.next();
        top = bottom;
        bottom = top + plainEdit->blockBoundingRect_pub(block).height();
        ++blockNumber;
    }
}

void CodeEditor::updateSidebarArea(const QRect &rect, int dy)
{
    if (dy) {
        sideBar->scroll(0, dy);
    } else {
        sideBar->update(0, rect.y(), sideBar->width(), rect.height());
    }
}

int CodeEditor::sidebarWidth() const
{
    int digits = 1;
    auto count = plainEdit->blockCount();
    while (count >= 10) {
        ++digits;
        count /= 10;
    }
    return 4 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + fontMetrics().lineSpacing();
}

void CodeEditor::highlightCurrentLine() {
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(Qt::cyan);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = plainEdit->textCursor();
    selection.cursor.clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    plainEdit->setExtraSelections(extraSelections);
}

void CodeEditor::setTheme(const KSyntaxHighlighting::Theme &theme)
{
    auto pal = qApp->palette();
    if (theme.isValid()) {
        pal.setColor(QPalette::Base, theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
        pal.setColor(QPalette::Highlight, theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
    }
    setPalette(pal);

    highlighter->setTheme(theme);
    highlighter->rehighlight();
    highlightCurrentLine();;
}

bool CodeEditor::isFoldable(const QTextBlock &block) const
{
    return highlighter->startsFoldingRegion(block);
}

bool CodeEditor::isFolded(const QTextBlock &block) const
{
    if (!block.isValid()) {
        return false;
    }
    const auto nextBlock = block.next();
    if (!nextBlock.isValid()) {
        return false;
    }
    return !nextBlock.isVisible();
}

void CodeEditor::toggleFold(const QTextBlock &startBlock)
{
    // we also want to fold the last line of the region, therefore the ".next()"
    const auto endBlock = highlighter->findFoldingRegionEnd(startBlock).next();

    if (isFolded(startBlock)) {
        // unfold
        auto block = startBlock.next();
        while (block.isValid() && !block.isVisible()) {
            block.setVisible(true);
            block.setLineCount(block.layout()->lineCount());
            block = block.next();
        }

    } else {
        // fold
        auto block = startBlock.next();
        while (block.isValid() && block != endBlock) {
            block.setVisible(false);
            block.setLineCount(0);
            block = block.next();
        }
    }

    // redraw document
    plainEdit->document()->markContentsDirty(startBlock.position(), endBlock.position() - startBlock.position() + 1);

    // update scrollbars
    Q_EMIT plainEdit->document()->documentLayout()->documentSizeChanged(plainEdit->document()->documentLayout()->documentSize());
}

QTextBlock CodeEditor::blockAtPosition(int y) const
{
    auto block = plainEdit->firstVisibleBlock_pub();
    if (!block.isValid()) {
        return QTextBlock();
    }

    int top = plainEdit->blockBoundingGeometry_pub(block).translated(plainEdit->contentOffset_pub()).top();
    int bottom = top + plainEdit->blockBoundingRect_pub(block).height();
    do {
        if (top <= y && y <= bottom) {
            return block;
        }
        block = block.next();
        top = bottom;
        bottom = top + plainEdit->blockBoundingRect_pub(block).height();
    } while (block.isValid());
    return QTextBlock();
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *event) {
    auto menu = plainEdit->createStandardContextMenu(event->pos());

    // syntax selection
    auto hlActionGroup = new QActionGroup(menu);
    hlActionGroup->setExclusive(true);
    auto hlGroupMenu = menu->addMenu(QStringLiteral("Syntax"));
    QMenu *hlSubMenu = hlGroupMenu;
    QString currentGroup;
    for (const auto &def : repository.definitions()) {
        if (def.isHidden()) {
            continue;
        }
        if (currentGroup != def.section()) {
            currentGroup = def.section();
            hlSubMenu = hlGroupMenu->addMenu(def.translatedSection());
        }

        Q_ASSERT(hlSubMenu);
        auto action = hlSubMenu->addAction(def.translatedName());
        action->setCheckable(true);
        action->setData(def.name());
        hlActionGroup->addAction(action);
        if (def.name() == highlighter->definition().name()) {
            action->setChecked(true);
        }
    }
    connect(hlActionGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        const auto defName = action->data().toString();
        const auto def = repository.definitionForName(defName);
        highlighter->setDefinition(def);
    });

    // theme selection
    auto themeGroup = new QActionGroup(menu);
    themeGroup->setExclusive(true);
    auto themeMenu = menu->addMenu(QStringLiteral("Theme"));
    for (const auto &theme : repository.themes()) {
        auto action = themeMenu->addAction(theme.translatedName());
        action->setCheckable(true);
        action->setData(theme.name());
        themeGroup->addAction(action);
        if (theme.name() == highlighter->theme().name()) {
            action->setChecked(true);
        }
    }
    connect(themeGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        const auto themeName = action->data().toString();
        const auto theme = repository.theme(themeName);
        setTheme(theme);
    });

    menu->exec(event->globalPos());
    delete menu;
}
