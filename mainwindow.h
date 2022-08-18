#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CodeEditor.h"
#include "IEditorFactory.h"
#include "UntitledCounter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    IEditorFactory *editorFactory;
    UntitledCounter untitledCounter;
    void createMenus();
    QTabWidget tabWidget;
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
};
#endif // MAINWINDOW_H
