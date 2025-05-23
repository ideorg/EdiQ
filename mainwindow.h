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

#include "IEditorFactory.h"
#include "downloader.h"
#include "mrutabwidget.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void receivedMessage(int instanceId, QByteArray message);
    void openOrActivate(QString path);
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QMenu *recentMenu;
    void tabSelected(int n);
    IEditorFactory *editorFactory;
    void createMenus();
    MruTabWidget tabWidget;
    void newFile();
    void openFile();
    void setRecentFiles(const QStringList &fileNames);
    void saveFile();
    void saveAsFile();
    void closeFile();
    void closeAllFile();
    void findSearch();
    void findNextSearch();
    void findPreviousSearch();
    void onTextChanged();
    void activateTab(int index);
    bool eventFilter(QObject *target, QEvent *event) override;
    void insertDate();
    void insertTime();
    void insertBoth();
    void onDownloaded();
    QAction *downloadUpdateAct;
    void downloadUpdate();
    KSyntaxHighlighting::DoubleDownloader downloader;
};
#endif // MAINWINDOW_H
