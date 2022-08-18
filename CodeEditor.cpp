//
// Created by Andrzej Borucki on 2022-08-17
//

#include "CodeEditor.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>

void CodeEditor::openFile() {
    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return;
    }
    setPlainText(QString::fromUtf8(f.readAll()));
}

void CodeEditor::save() {
    assert(!path.isEmpty());
    QString text = toPlainText();
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Failed to open" << path << ":" << f.errorString();
        return;
    }
    f.write(text.toUtf8());
}

void CodeEditor::saveAs(const QString &fileName) {
    path = fileName;
    save();
}

QString CodeEditor::getTitle() {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}
