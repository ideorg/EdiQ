//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_EDITORFACTORY_H
#define EDIQ_EDITORFACTORY_H


#include <QTabWidget>
#include "IEditorFactory.h"
#include "UntitledCounter.h"

class EditorFactory : public IEditorFactory {
    QTabWidget *tabWidget;
    UntitledCounter untitledCounter;
public:
    explicit EditorFactory(QTabWidget *tabWidget):tabWidget(tabWidget){};
    IEditor *createEditorTab(const QString& path) override;
    int getEditorCount() override;
    IEditor *getEditor(int index) override;
    IEditor *getCurrentEditor() override;
    bool tryCloseEditor(int index, IEditor::CloseEnum &canClose) override;
    bool tryCloseCurrentEditor() override;
private:
    void closeEditor(int index);
    void askBeforeClosing(int index, IEditor::CloseEnum &canClose);
};


#endif //EDIQ_EDITORFACTORY_H
