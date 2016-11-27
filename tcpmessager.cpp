#include "tcpmessager.h"

TcpMessager::TcpMessager(QObject *parent, QTcpSocket* UserSocket, Crypto *Cryptography, quint64 UserKey) : QObject(parent)
{
    Crypt = Cryptography;
    key = UserKey;
    socket = UserSocket;
    connect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
    connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_SocketStatusChanged(QAbstractSocket::SocketState)));
}


void TcpMessager::sendString(QString message)
{
    QByteArray block;
    QDataStream sendStream(&block,QIODevice::ReadWrite);
    sendStream << quint64(0) << message;
    sendStream.device()->seek(0);
    //TODO:Crypt->Encrypt(key,&block);
    sendStream << (quint64)(block.size()-sizeof(quint64));
    socket->write(block);
}

void TcpMessager::sendArray(QByteArray* array)
{
    QByteArray block;
    QDataStream sendStream(&block,QIODevice::ReadWrite);
    sendStream << quint64(0) << *array;
    sendStream.device()->seek(0);
    sendStream << (quint64)(block.size()-sizeof(quint64));
    //TODO:Crypt->Encrypt(key,array);
    socket->write(block);
}

void TcpMessager::on_readyRead()
{
    QByteArray buff;
    QDataStream stream(socket);

    while(true)
    {
        if(MesSize==0)
        {
            if(socket->bytesAvailable()<sizeof(quint64))
            {
                return;
            }
            stream >> MesSize;
        }
        else
        {
            if(MesSize > socket->bytesAvailable())
            {
                return;
            }
            stream >> buff;
            MesSize=0;
            //TODO:Crypt->Decrypt(key,&buff);
            emit NewMessage(buff);
        }
    }
}

void TcpMessager::on_SocketStatusChanged(QAbstractSocket::SocketState SockState)
{
    if (SockState == QAbstractSocket::UnconnectedState)
    {
        emit NoConnection();
    }
    PrevSockState = SockState;
}
