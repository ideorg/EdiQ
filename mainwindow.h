/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

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
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    IEditorFactory *editorFactory;
    void createMenus();
    QTabWidget tabWidget;
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void closeFile();
    void closeAllFile();
    void findSearch();
    void findNextSearch();
    void findPreviousSearch();
};
#endif // MAINWINDOW_H
