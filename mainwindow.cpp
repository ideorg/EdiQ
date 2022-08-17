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
    QAction *openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        QFileInfo fileInfo(dialog.selectedFiles().first());
        IEditor *newEditor = editorFactory->createTab(fileInfo.fileName()); //only name part without directory
        newEditor->openFile(dialog.selectedFiles().first());
    }
}
