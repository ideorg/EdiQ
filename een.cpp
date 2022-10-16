//
// Created by Andrzej Borucki on 2022-10-16.
//
#include "een.h"
#include <botan/aes.h>
#include <botan/sha3.h>
#include <botan/argon2.h>
#include <QInputDialog>
#include <QMessageBox>

#pragma pack(push)
#pragma pack(1)
struct EenHeader{
    uint32_t Ident;
    uint32_t ID_number;
    uint32_t header_version;
    int32_t data_size;
    int32_t argonMemoryKiB;
    int32_t argonIterations;
    int32_t argonThreads;
    int32_t padding;
    unsigned char digestsha[32];
};
#pragma pack(pop)

const uint cIdent = 0x4E4545; //'EEN'#0
const uint cID_number = 0x1479A43A;

static bool derive_key(const std::string& password, unsigned char *digest, EenHeader& header, bool decrypt) {
    Botan::Argon2 argon2 = Botan::Argon2(2, header.argonMemoryKiB, header.argonIterations,
                                         header.argonThreads);
    argon2.derive_key(digest, 32,
                      password.data(),password.length(),
                      (unsigned char*)password.data(),password.length());
    Botan::SHA_3_256 sha3;
    sha3.update(digest,32);
    if (decrypt) {
        unsigned char digestsha[32];
        sha3.final(digestsha);
        return memcmp(digestsha,header.digestsha,32)==0;
    } else {
        sha3.final(header.digestsha);
        return true;
    }
}

QByteArray encrypt(const QByteArray &bytes, std::string &password) {
    EenHeader header;
    header.Ident = cIdent;
    header.ID_number = cID_number;
    header.data_size = bytes.count();
    header.argonIterations = 2;
    header.argonMemoryKiB = 256*1024;
    header.argonThreads = 8;
    header.header_version = 1;
    header.padding = 0;
    unsigned char digest[32];
    Botan::AES_256 aes256;
    int BSize= aes256.block_size();
    derive_key(password, digest, header, false);
    int nblocks = (bytes.count()+BSize-1)/BSize;
    auto* dataIn = new unsigned char[nblocks * BSize];
    auto* dataOut = new unsigned char[sizeof(EenHeader)+nblocks * BSize];
    memcpy(dataIn, bytes.data(), bytes.count());
    memset(dataIn + bytes.count(), 0, nblocks * BSize - bytes.count());
    memcpy(dataOut, &header, sizeof(header));

    aes256.set_key(digest,32);
    aes256.encrypt_n(dataIn, dataOut+sizeof(header), nblocks);
    QByteArray result((char*)dataOut, sizeof(EenHeader)+nblocks*BSize);
    return result;
}

QByteArray decrypt(const QByteArray &bytes, std::string &password) {
    EenHeader header;
    memcpy(&header, bytes.data(), sizeof(header));
    unsigned char digest[32];
    if (!derive_key(password, digest, header, true)) {
        return {};
    }
    Botan::AES_256 aes256;
    int BSize= aes256.block_size();
    int nblocks = (bytes.count()-sizeof(header)+BSize-1)/BSize;
    auto* dataIn = new unsigned char[nblocks * BSize];
    auto* dataOut = new unsigned char[nblocks * BSize];
    memcpy(dataIn, bytes.data()+sizeof(header), bytes.count()-sizeof(header));
    aes256.set_key(digest,32);
    aes256.decrypt_n(dataIn, dataOut, nblocks);
    QByteArray result((char*)dataOut, header.data_size);
    return result;
}

static bool isAlnum(const QChar c) {
    return c>='A' && c<='Z' ||
            c>='a' && c<='z' ||
            c>='0' && c<='9';
}

static bool isAlnum(const QString &pass) {
    for (auto c:pass) {
        if (!isAlnum(c))
            return false;
    }
    return true;
}

std::string setPassword() {
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Set password:",
                                         "Password:", QLineEdit::Password,
                                         "", &ok);
    if (!ok) {
        QMessageBox msgBox;
        msgBox.setText("Password not set");
        msgBox.exec();
        return "";
    }
    if (!isAlnum(text)) {
        QMessageBox msgBox;
        msgBox.setText("Password must contain only Ascii letters and digits");
        msgBox.exec();
        return "";
    }
    QString text1 = QInputDialog::getText(nullptr, "Repeat password:",
                                         "Password:", QLineEdit::Password,
                                         "", &ok);
    if (text1!=text) {
        QMessageBox msgBox;
        msgBox.setText("Two password inputs mismatch");
        msgBox.exec();
        return "";
    }
    return text.toUtf8().constData();
}

std::string getPassword() {
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Get password:",
                                         "Password:", QLineEdit::Password,
                                         "", &ok);
    if (ok)
        return text.toUtf8().constData();
    else
        return "";
}
