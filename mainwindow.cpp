#include "mainwindow.h"
#include "EditorFactory.h"
#include <QMenuBar>
#include <QFileDialog>

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

    QAction *saveAsAct = new QAction(tr("save&As..."), this);
    fileMenu->addAction(saveAsAct);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

    QAction *closeAct = new QAction(tr("&Close"), this);
    fileMenu->addAction(closeAct);
    connect(closeAct, &QAction::triggered, this, &MainWindow::closeFile);

    QAction *closeAllAct = new QAction(tr("Clos&e all"), this);
    fileMenu->addAction(closeAllAct);
    connect(closeAllAct, &QAction::triggered, this, &MainWindow::closeAllFile);
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
