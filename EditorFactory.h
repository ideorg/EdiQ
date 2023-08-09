/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_EDITORFACTORY_H
#define EDIQ_EDITORFACTORY_H


#include <QTabWidget>
#include <repository.h>
#include "IEditorFactory.h"
#include "UntitledCounter.h"

class EditorFactory : public QObject, public IEditorFactory {
    QTabWidget *tabWidget;
    UntitledCounter untitledCounter;
    KSyntaxHighlighting::Repository* repository;
    QString themeName = "";
public:
    explicit EditorFactory(QTabWidget *tabWidget);
    ~EditorFactory() {delete repository;}
    IEditor *createEditorTab(const QString& path) override;
    int getEditorCount() override;
    IEditor *getEditor(int index) override;
    IEditor *getCurrentEditor() override;
    IEditor *getEditorByPath(const QString &path) override;
    bool tryCloseEditor(int index, IEditor::CloseEnum &canClose) override;
    bool tryCloseCurrentEditor() override;
    bool tryCloseAll() override;
    void onTextChanged() override;
    void onRefreshRepository();
    void onChangeThemeName(QString newName);
private:
    void closeEditor(int index);
};


#endif //EDIQ_EDITORFACTORY_H
