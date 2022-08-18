//
// Created by Andrzej Borucki on 2022-08-17
//

#include "CodeEditor.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>

void CodeEditor::openFile() {
    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return;
    }
    setPlainText(QString::fromUtf8(f.readAll()));
}

bool CodeEditor::saveFile() {
    assert(!path.isEmpty());
    QString text = toPlainText();
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return false;
    }
    f.write(text.toUtf8());
    return true;
}

bool CodeEditor::saveAs() {
    QFileDialog dialog(this, tr("Save File"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        path = dialog.selectedFiles().first();
        return saveFile();
    }
    else
        return false;
}

bool CodeEditor::save() {
    if (path.isEmpty())
        return saveAs();
    else
        return saveFile();
}

QString CodeEditor::getTitle() {
    if (path.isEmpty())
        return "Untitled" + QString::number(untitledId);
    else {
        QFileInfo fileInfo(path);
        return fileInfo.fileName();
    }
}

bool CodeEditor::isModified() {
    return document()->isModified();
}

bool CodeEditor::isEmpty() {
    return document()->isEmpty();
}

IEditor::ConsiderEnum CodeEditor::consider() {
    if (!isModified() || isEmpty())
        return coCanClose;
    else if (!path.isEmpty())
        return coCanSave;
    else
        return coSaveAs;
}

void CodeEditor::askSaveChangesBeforeClosing(IEditor::CloseEnum &canClose) {
    if (isEmpty()) {
        canClose = clClose;
        return;
    }
    if (canClose == clCloseAllDiscard) return;
    if (canClose == clCloseAllSave) {
        if (!save())
            canClose = clCancel;
        return;
    }
    QMessageBox::StandardButton reply;
    QString message = "The text in the file " + getTitle() + " has been changed.\n"+
                               "Do you want to save the modifications? (No = close and discard changes)";

    reply = QMessageBox::question(this, "Warning", message,
                                  QMessageBox::Yes | QMessageBox::YesAll |
                                  QMessageBox::No | QMessageBox::NoToAll |
                                  QMessageBox::Cancel);
    switch(reply) {
        case QMessageBox::Yes: {
            if (save())
                canClose = clClose;
            else
                canClose = clNo;
            break;
        }
        case QMessageBox::YesToAll: {
            if (save())
                canClose = clCloseAllSave;
            else
                canClose = clNo;
            break;
        }
        case QMessageBox::No: canClose = clClose; break;
        case QMessageBox::NoToAll: canClose = clCloseAllDiscard; break;
        default: canClose = clCancel;
    }
}

