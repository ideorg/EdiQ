/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-13
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "UntitledCounter.h"

int UntitledCounter::getNextId() {
    int cnt = bitArray->count();
    int result = cnt;
    for (int i=1; i<cnt; i++)
    {
        if (!bitArray->testBit(i)) {
            result = i;
            break;
        }
    };
    assert(bitArray->size()>=result);
    if (bitArray->size()==result)
        bitArray->resize(result+RESIZE);
    bitArray->setBit(result);
    return result;
}

bool UntitledCounter::releaseId(int id) {
    assert(id>=1);
    assert(bitArray->testBit(id));
    bitArray->clearBit(id);
    return true;
}

UntitledCounter::UntitledCounter() {
    bitArray = new QBitArray(RESIZE);
    bitArray->setBit(0); //not used for untitled
}
