#ifndef USERCONNECTION_H
#define USERCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

class UserConnection : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    quint64 MesSize = 0;
public:
    UserConnection(QObject *parent,QTcpSocket* UserSocket);

signals:
    void NewUserData(QString login,QString pass,QTcpSocket* UserSocket);

private slots:
    void on_readyRead();

public slots:
};

#endif // USERCONNECTION_H
