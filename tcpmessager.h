#ifndef TCPMESSAGER_H
#define TCPMESSAGER_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>

#include "QTcpSocket"
#include "crypto.h"

class TcpMessager : public QObject
{
    Q_OBJECT
private:
    Crypto* Crypt;
    quint64 key;
    QTcpSocket* socket;
    unsigned int MesSize = 0;
    QAbstractSocket::SocketState PrevSockState = QAbstractSocket::UnconnectedState;
public:
    TcpMessager(QObject *parent,QTcpSocket* UserSocket,Crypto* Cryptography,quint64 key);
    void sendString(QString message);
    void sendArray(QByteArray* array);
signals:
    void NewMessage(QByteArray& message);
    void NoConnection();

private slots:
    void on_readyRead();
    void on_SocketStatusChanged(QAbstractSocket::SocketState SockState);
};

#endif // TCPMESSAGER_H
