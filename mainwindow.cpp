/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "mainwindow.h"
#include "EditorFactory.h"
#include "SearchBar.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QCloseEvent>
#include "raise.h"
#include "CodeEditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    tabWidget.setTabsClosable(true);
    tabWidget.setMovable(true);
    tabWidget.setTabPosition(QTabWidget::South);
    setCentralWidget(&tabWidget);
    editorFactory = new EditorFactory(&tabWidget);
}

MainWindow::~MainWindow()
{
    delete editorFactory;
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(QKeySequence("ctrl+n"));
    fileMenu->addAction(newAct);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence("ctrl+o"));
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence("ctrl+s"));
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveAsAct = new QAction(tr("save &As..."), this);
    fileMenu->addAction(saveAsAct);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

    QAction *closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(QKeySequence("ctrl+F4"));
    fileMenu->addAction(closeAct);
    connect(closeAct, &QAction::triggered, this, &MainWindow::closeFile);

    QAction *closeAllAct = new QAction(tr("clos&E all"), this);
    closeAct->setShortcut(QKeySequence("shift+ctrl+F4"));
    fileMenu->addAction(closeAllAct);
    connect(closeAllAct, &QAction::triggered, this, &MainWindow::closeAllFile);

    QMenu *searchMenu = menuBar()->addMenu(tr("&Search"));
    QAction *findAct = new QAction(tr("&Find..."), this);
    findAct->setShortcut(QKeySequence("ctrl+f"));
    searchMenu->addAction(findAct);
    connect(findAct, &QAction::triggered, this, &MainWindow::findSearch);

    QAction *findNextAct = new QAction(tr("find &Next"), this);
    findNextAct->setShortcut(QKeySequence("f3"));
    searchMenu->addAction(findNextAct);
    connect(findNextAct, &QAction::triggered, this, &MainWindow::findNextSearch);

    QAction *findPreviousAct = new QAction(tr("find &Previous"), this);
    findPreviousAct->setShortcut(QKeySequence("shift+f3"));
    searchMenu->addAction(findPreviousAct);
    connect(findPreviousAct, &QAction::triggered, this, &MainWindow::findPreviousSearch);
}

void MainWindow::newFile() {
    CodeEditor *editor = (CodeEditor *)editorFactory->createEditorTab("");
    tabWidget.setCurrentWidget(editor);
}

void MainWindow::openOrActivate(QString path) {
    CodeEditor *editor = (CodeEditor *)editorFactory->getEditorByPath(path);
    if (editor) {
        auto plainEdit = editor->plainEdit;
        QTextCursor cursor = plainEdit->textCursor();
        int position = cursor.position();
        editor->openFile();
        cursor.setPosition(position, QTextCursor::MoveAnchor);
        plainEdit->setTextCursor(cursor);
        editor->activateWindow();
        plainEdit->document()->setModified(false);
        editorFactory->onTextChanged();
    }
    else {
        editor = (CodeEditor *) editorFactory->createEditorTab(path);
        editor->openFile();
        editorFactory->onTextChanged();
    }
    tabWidget.setCurrentWidget(editor);
    editor->setFocus();
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        QString path = dialog.selectedFiles().first();
        openOrActivate(path);
    }
}

void MainWindow::saveFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->save();
    editorFactory->onTextChanged();
}

void MainWindow::saveAsFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->saveAs();
    editorFactory->onTextChanged();
}

void MainWindow::closeFile() {
    editorFactory->tryCloseCurrentEditor();
}

void MainWindow::closeAllFile() {
    editorFactory->tryCloseAll();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (editorFactory->tryCloseAll())
        event->accept();
    else
        event->ignore();
}

void MainWindow::findSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->find();
}

void MainWindow::findNextSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->findNext();
}

void MainWindow::findPreviousSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->findPrevious();
}

void MainWindow::receivedMessage(int instanceId, QByteArray message) {
    QString argLine = QString(message);
    QStringList args = argLine.split(' ');
    for (int i=1; i<args.size(); i++)
        openOrActivate(args[i]);
    raiseThis();
}
