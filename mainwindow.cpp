#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    setCentralWidget(&editor);
}

MainWindow::~MainWindow()
{
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
        QString fileName = dialog.selectedFiles().first();
        QFile f(fileName);
        if (!f.open(QFile::ReadOnly)) {
            qWarning() << "Failed to open" << fileName << ":" << f.errorString();
            return;
        }
        editor.setPlainText(QString::fromUtf8(f.readAll()));
    }
}
