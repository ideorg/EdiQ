//
// Created by Andrzej Borucki on 2022-08-17
//

#include "CodeEditor.h"
#include <QFile>
#include <QDebug>

void CodeEditor::openFile(const QString &fileName) {
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open" << fileName << ":" << f.errorString();
        return;
    }
    setPlainText(QString::fromUtf8(f.readAll()));
}