#include "userconnection.h"

UserConnection::UserConnection(QObject *parent, QTcpSocket *UserSocket) : QObject(parent)
{
    socket = UserSocket;
    connect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
}

void UserConnection::on_readyRead()
{
    QString buff;
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
            QStringList TmpList = buff.split(":");
            disconnect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
            qDebug() << "UserConnection: " << TmpList[0] << ", md5pass: " << TmpList[1];
            emit NewUserData(TmpList[0],TmpList[1],socket);
        }
    }

}
