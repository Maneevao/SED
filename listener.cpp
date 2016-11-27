#include "listener.h"

Listener::Listener(QObject *parent, int port) : QObject(parent)
{
    ServSocket = new QTcpServer(this);
    if(ServSocket->listen(QHostAddress::Any,port) == false)
    {
        ServSocket->close();
        emit CanNotConnect();
    }
    else
    {
        connect(ServSocket,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    }

}

void Listener::on_newConnection()
{
    QTcpSocket* NewUserSocket = ServSocket->nextPendingConnection();
    UserConnection* UC = new UserConnection(this,NewUserSocket);
    connect(UC,SIGNAL(NewUserData(QString,QString,QTcpSocket*)),this,SLOT(on_newUser(QString,QString,QTcpSocket*)));
}

void Listener::on_newUser(QString login, QString pass, QTcpSocket* UserSocket)
{
    UserConnection* UC = static_cast<UserConnection*>(sender());   
    emit NewUserConnection(login,pass,UserSocket);
    delete UC;
}
