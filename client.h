#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

#include "commonheader.h"
#include "doc.h"
#include "protocol.h"
#include "crypto.h"
#include "tcpmessager.h"

class Client : public QObject
{
    Q_OBJECT
private:
    QList<Doc*>*       Docs;
    Crypto*              Cryptography;
    Protocol*            Prot;

    QString ServerIP;
    int ServerPort;
    QString CommandFromServer = "";

    QString Name;
    QString Pass;
    QString md5Pass;
    quint64 key;

    TcpMessager* TM;
    QTcpSocket*  ClientSocket;
    QAbstractSocket::SocketState PrevSockState = QAbstractSocket::UnconnectedState;
    QString FileName;

    void ConnectToServer();
    void Authentication();

    QTextCodec* TC = QTextCodec::codecForName("UTF-16BE");
public:
    Client(QObject *parent, QList<Doc*>* D, Crypto* C, Protocol* Proto, QString ip, int port, QString Login, QString Password);
    void SendMessage(QString message);
    void MakeNewDocument(QString DocName,QString FilePath);
    void GiveReadPerm(unsigned int UserId,unsigned int DocId);
    void RmReadPerm(unsigned int UserId,unsigned int DocId);
    void DownloadDoc(unsigned int DocId);
    QString GetName();
signals:
    void NoServer();
    void OnlineUsersListChanged(QStringList OnlineUsers);
    void UsersListChanged(QStringList UsersList);
    void NewChatMessage(QString message);
    void NewDocList(QString DocsList);
    void NewUsersList(QString UsersList);
    void NewDocsList(QString DocsList);
public slots:
    void on_SocketStatusChange(QAbstractSocket::SocketState SockState);
    void on_MakeNewDoc(QString DocName,QString FilePath);
    void on_GiveReadPerm(unsigned int UserId,unsigned int DocId);
    void on_RmReadPerm(unsigned int UserId,unsigned int DocId);
private slots:
    void on_NoConnection();
    void on_NewMessage(QByteArray &message);
    void on_DownloadDoc(unsigned int DocId);
};

#endif // CLIENT_H
