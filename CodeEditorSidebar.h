//
// Created by Andrzej Borucki on 2022-08-19
//

#ifndef EDIQ_CODEEDITORSIDEBAR_H
#define EDIQ_CODEEDITORSIDEBAR_H

#include <QWidget>

class CodeEditor;

class CodeEditorSidebar : public QWidget {
Q_OBJECT
    CodeEditor *codeEditor;
public:
    explicit CodeEditorSidebar(CodeEditor *editor);
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};


#endif //EDIQ_CODEEDITORSIDEBAR_H
