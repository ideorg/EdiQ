#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void closeFile();
};
#endif // MAINWINDOW_H
