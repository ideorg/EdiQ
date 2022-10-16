/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_IEDITOR_H
#define EDIQ_IEDITOR_H


#include <QString>

class IEditor {
public:
    enum ConsiderEnum {coCanClose, coCanSave, coSaveAs};
    enum CloseEnum {clClose, clCloseAllSave, clCloseAllDiscard, clNo, clCancel, clError};
    virtual QString getPath() = 0;
    virtual QString getTitle() = 0;
    virtual bool isModified() = 0;
    virtual bool isEmpty() = 0;
    virtual void openFile() = 0;
    //virtual void openFile(const QString &fileName) = 0;
    virtual bool save() = 0;
    virtual bool saveAs() = 0;
    virtual ConsiderEnum consider() = 0;
    virtual void askSaveChangesBeforeClosing(CloseEnum &canClose) = 0;
    virtual void find()  = 0;
    virtual void findNext()  = 0;
    virtual void findPrevious()  = 0;
    virtual void setPlainFocus()  = 0;
    virtual void insertDate()  = 0;
    virtual void insertTime()  = 0;
    virtual void insertBoth()  = 0;
};


#endif //EDIQ_IEDITOR_H
