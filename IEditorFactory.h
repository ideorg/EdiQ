//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_IEDITORFACTORY_H
#define EDIQ_IEDITORFACTORY_H


#include "IEditor.h"

class IEditorFactory {
public:
    virtual IEditor *createTab(const QString& title) = 0;
    virtual int getEditorCount() = 0;
    virtual IEditor *getEditor(int index) = 0;
    virtual IEditor *getCurrentEditor() = 0;
};


#endif //EDIQ_IEDITORFACTORY_H
