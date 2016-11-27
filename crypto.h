#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

class Crypto
{
public:
    Crypto();
    void Encrypt(quint64 key,QByteArray* message);
    void Decrypt(quint64 key,QByteArray* cypher );
    QString Md5Pass(QString Password);
    quint64 GenKey(QString md5Pass);
};

#endif // CRYPTO_H
