//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_EDITORFACTORY_H
#define EDIQ_EDITORFACTORY_H


#include <QTabWidget>
#include "IEditorFactory.h"

class EditorFactory : public IEditorFactory {
    QTabWidget *tabWidget;
public:
    explicit EditorFactory(QTabWidget *tabWidget):tabWidget(tabWidget){};
    IEditor *createEditorTab(const QString& path) override;
    IEditor *createEditorTab(int untitledId) override;
    int getEditorCount() override;
    IEditor *getEditor(int index) override;
    IEditor *getCurrentEditor() override;
};


#endif //EDIQ_EDITORFACTORY_H
