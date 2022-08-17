#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(&editor);
}

MainWindow::~MainWindow()
{
}

