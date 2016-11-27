#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "userconnection.h"

class Listener : public QObject
{
    Q_OBJECT
private:
    QTcpServer* ServSocket;
public:
    explicit Listener(QObject *parent,int port);

signals:
    void CanNotConnect();
    void NewUserConnection(QString login, QString pass, QTcpSocket* UserSocket);

private slots:
    void on_newConnection();
    void on_newUser(QString login, QString pass, QTcpSocket* UserSocket);
};

#endif // LISTENER_H
