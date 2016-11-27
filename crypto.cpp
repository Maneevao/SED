#include "crypto.h"

Crypto::Crypto()
{

}

void Crypto::Encrypt(quint64 key, QByteArray* message)
{
    //TODO:
}

void Crypto::Decrypt(quint64 key, QByteArray* cypher)
{
    //TODO:
}

QString Crypto::Md5Pass(QString Password)
{
    return QString(QCryptographicHash::hash(Password.toUtf8(),QCryptographicHash::Md5).toHex());
}

quint64 Crypto::GenKey(QString md5Pass)
{
    quint64 NewKey = 0;
    QByteArray BA = md5Pass.toUtf8();
    for (int i=0;i<BA.size();i+=BA.size()/8)
    {
        NewKey = NewKey*0x100 + BA[i];
    }
    return NewKey;
}
