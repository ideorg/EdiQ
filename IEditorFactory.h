//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_IEDITORFACTORY_H
#define EDIQ_IEDITORFACTORY_H


#include "IEditor.h"

class IEditorFactory {
public:
    virtual IEditor *createEditorTab(const QString& path) = 0;
    virtual IEditor *createEditorTab(int untitledId) = 0;
    virtual int getEditorCount() = 0;
    virtual IEditor *getEditor(int index) = 0;
    virtual IEditor *getCurrentEditor() = 0;
};


#endif //EDIQ_IEDITORFACTORY_H
