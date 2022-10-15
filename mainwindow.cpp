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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
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
    fileMenu->addAction(newAct);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAct = new QAction(tr("&Save"), this);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveAsAct = new QAction(tr("save &As..."), this);
    fileMenu->addAction(saveAsAct);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

    QAction *closeAct = new QAction(tr("&Close"), this);
    fileMenu->addAction(closeAct);
    connect(closeAct, &QAction::triggered, this, &MainWindow::closeFile);

    QAction *closeAllAct = new QAction(tr("clos&E all"), this);
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
    IEditor *newEditor = editorFactory->createEditorTab("");
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        IEditor *newEditor = editorFactory->createEditorTab(dialog.selectedFiles().first());
        newEditor->openFile();
    }
}

void MainWindow::saveFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->save();
}

void MainWindow::saveAsFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->saveAs();
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

}

void MainWindow::findNextSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->findNext();
}

void MainWindow::findPreviousSearch() {

}
