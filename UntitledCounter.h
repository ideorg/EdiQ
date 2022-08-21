/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-13
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_UNTITLECOUNTER_H
#define EDIQ_UNTITLECOUNTER_H


#include <QBitArray>

/* Named editors will have id=0, untitled from 1,2... */
class UntitledCounter {
    const int RESIZE = 32;
    QBitArray *bitArray;
public:
    UntitledCounter();
    ~UntitledCounter() {delete bitArray;};
    int getNextId();
    bool releaseId(int id);
};


#endif //EDIQ_UNTITLECOUNTER_H
