#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CodeEditor.h"
#include "IEditorFactory.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    IEditorFactory *editorFactory;
    void createMenus();
    QTabWidget tabWidget;
    void openFile();
};
#endif // MAINWINDOW_H
