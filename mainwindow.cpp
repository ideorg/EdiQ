/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "mainwindow.h"
#include "CodeEditor.h"
#include "EditorFactory.h"
#include "SearchBar.h"
#include "libpaths.h"
#include "raise.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget.setTabsClosable(true);
    tabWidget.setMovable(true);
    tabWidget.setTabPosition(QTabWidget::South);
    setCentralWidget(&tabWidget);
    editorFactory = new EditorFactory(&tabWidget);
    createMenus();
    connect(&tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        IEditor::CloseEnum canClose = IEditor::clClose;
        editorFactory->tryCloseEditor(index, canClose);
    });
    connect(&tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
    QGuiApplication::instance()->installEventFilter(this);
    connect(&downloader, &KSyntaxHighlighting::DoubleDownloader::done, this, &MainWindow::onDownloaded);
    downloader.setPath(KSyntaxHighlighting::LibPaths::data());
    if (downloader.mustDownload()) {
        downloadUpdateAct->setEnabled(false);
        downloader.start();
    }
}

MainWindow::~MainWindow()
{
    delete (EditorFactory*)editorFactory;
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(QKeySequence("ctrl+n"));
    fileMenu->addAction(newAct);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence("ctrl+o"));
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    recentMenu = fileMenu->addMenu(tr("&Recent files"));
    auto mru = editorFactory->getMRU();
    auto items = mru->items();
    setRecentFiles(items);
    connect(mru, &MRU::setItems, this, &MainWindow::setRecentFiles);
    connect(mru, &MRU::setItems, this, &MainWindow::setRecentFiles);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence("ctrl+s"));
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveAsAct = new QAction(tr("save &As..."), this);
    fileMenu->addAction(saveAsAct);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

    QAction *closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(QKeySequence("ctrl+F4"));
    fileMenu->addAction(closeAct);
    connect(closeAct, &QAction::triggered, this, &MainWindow::closeFile);

    QAction *closeAllAct = new QAction(tr("clos&E all"), this);
    closeAllAct->setShortcut(QKeySequence("shift+ctrl+F4"));
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

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    QAction *insertDateAct = new QAction(tr("insert &Date"), this);
    toolsMenu->addAction(insertDateAct);
    connect(insertDateAct, &QAction::triggered, this, &MainWindow::insertDate);

    QAction *insertTimeAct = new QAction(tr("insert &Time"), this);
    toolsMenu->addAction(insertTimeAct);
    connect(insertTimeAct, &QAction::triggered, this, &MainWindow::insertTime);

    QAction *insertBothAct = new QAction(tr("insert &Both date and time"), this);
    toolsMenu->addAction(insertBothAct);
    connect(insertBothAct, &QAction::triggered, this, &MainWindow::insertBoth);

    toolsMenu->addSeparator();

    downloadUpdateAct = new QAction(tr("download &Update"), this);
    toolsMenu->addAction(downloadUpdateAct);
    connect(downloadUpdateAct, &QAction::triggered, this, &MainWindow::downloadUpdate);
}

void MainWindow::newFile() {
    CodeEditor *editor = (CodeEditor *)editorFactory->createEditorTab("");
    tabWidget.setCurrentWidget(editor);
}

void MainWindow::openOrActivate(QString path) {
    CodeEditor *editor = (CodeEditor *)editorFactory->getEditorByPath(path);
    if (editor) {
        auto plainEdit = editor->plainEdit;
        QTextCursor cursor = plainEdit->textCursor();
        int position = cursor.position();
        editor->openFile();
        cursor.setPosition(position, QTextCursor::MoveAnchor);
        plainEdit->setTextCursor(cursor);
        editor->activateWindow();
        plainEdit->document()->setModified(false);
        editorFactory->onTextChanged();
    }
    else {
        editor = (CodeEditor *) editorFactory->createEditorTab(path);
        editor->openFile();
        editorFactory->onTextChanged();
        editorFactory->getMRU()->takeItem(path);
    }
    tabWidget.setCurrentWidget(editor);
    editor->setPlainFocus();
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        QString path = dialog.selectedFiles().first();
        openOrActivate(path);
    }
}

void MainWindow::saveFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->save();
    editorFactory->onTextChanged();
}

void MainWindow::saveAsFile() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->saveAs();
    tabWidget.setTabText(tabWidget.currentIndex(), editor->getTitle());
    editorFactory->onTextChanged();
    tabSelected(tabWidget.currentIndex());
}

void MainWindow::closeFile() {
    editorFactory->tryCloseCurrentEditor();
}

void MainWindow::closeAllFile() {
    editorFactory->tryCloseAll();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (editorFactory->tryCloseAll()) {
      event->accept();
    } else
        event->ignore();
}

void MainWindow::findSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->find();
}

void MainWindow::findNextSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->findNext();
}

void MainWindow::findPreviousSearch() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->findPrevious();
}

void MainWindow::receivedMessage(int instanceId, QByteArray message) {
    QString argLine = QString(message);
    QStringList args = argLine.split(' ');
    args.removeFirst();
    openOrActivate(args.join(' '));
    raiseThis();
}

void MainWindow::activateTab(int index) {
    assert(index>=0);
    if (index==tabWidget.currentIndex()) return;
    if (index>=tabWidget.count()) return;
    tabWidget.setCurrentWidget(tabWidget.widget(index));
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
    if (event->type() == QEvent::KeyPress)
    {
        int key = keyEvent->key();
        if (key >= '0' && key <= '9' && keyEvent->modifiers() & Qt::AltModifier) {
            int n = key > '0'? key-'1': 9;
            activateTab(n);
            return true;
        } else if (key == Qt::Key_Escape) {
            IEditor* editor = editorFactory->getCurrentEditor();
            if (editor)
                editor->setPlainFocus();
            return QObject::eventFilter(target, event);
        }
    }
    return QObject::eventFilter(target, event);
}

void MainWindow::insertDate() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->insertDate();
}

void MainWindow::insertTime() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->insertTime();
}

void MainWindow::insertBoth() {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    editor->insertBoth();
}

void MainWindow::tabSelected(int n) {
    IEditor *editor = editorFactory->getCurrentEditor();
    if (!editor) return;
    this->setWindowTitle(editor->getPath());
}

void MainWindow::onDownloaded() {
    editorFactory->onRefreshRepository();
    downloadUpdateAct->setEnabled(true);
}

void MainWindow::downloadUpdate() {
    downloadUpdateAct->setEnabled(false);
    downloader.start();
}

void MainWindow::setRecentFiles(const QStringList &fileNames) {
  recentMenu->clear();
  for (auto &fileName :fileNames) {
    auto *action = new QAction(fileName, this);
    recentMenu->addAction(action);
    connect(action, &QAction::triggered, this, [this, fileName]() {
      openOrActivate(fileName);
    });
  }
}
