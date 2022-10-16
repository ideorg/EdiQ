//
// Created by Andrzej Borucki on 2022-10-16.
//

#ifndef EDIQ_EEN_H
#define EDIQ_EEN_H

#include <QByteArray>

QByteArray encrypt(const QByteArray &bytes, std::string &password);
QByteArray decrypt(const QByteArray &bytes, std::string &password);

#endif //EDIQ_EEN_H
