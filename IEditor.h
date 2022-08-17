//
// Created by Andrzej Borucki on 2022-08-17
//

#ifndef EDIQ_IEDITOR_H
#define EDIQ_IEDITOR_H


#include <QString>

class IEditor {
public:
    virtual void openFile(const QString &fileName) = 0;
    virtual void save() = 0;
};


#endif //EDIQ_IEDITOR_H
