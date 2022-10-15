/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_IEDITORFACTORY_H
#define EDIQ_IEDITORFACTORY_H

#include "IEditor.h"

class IEditorFactory {
public:
    virtual IEditor *createEditorTab(const QString& path) = 0;
    virtual int getEditorCount() = 0;
    virtual IEditor *getEditor(int index) = 0;
    virtual IEditor *getCurrentEditor() = 0;
    virtual IEditor *getEditorByPath(const QString &path) = 0;
    virtual bool tryCloseEditor(int index, IEditor::CloseEnum &canClose) = 0;
    virtual bool tryCloseCurrentEditor() = 0;
    virtual bool tryCloseAll() = 0;
    virtual void onTextChanged()  = 0;
};


#endif //EDIQ_IEDITORFACTORY_H
