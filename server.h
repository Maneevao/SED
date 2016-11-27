#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "listener.h"
#include "serveruser.h"
#include "crypto.h"
#include "protocol.h"
#include "doc.h"
#include "commonheader.h"

class Server : public QObject
{
    Q_OBJECT
private:
    Listener*            ServerListener;
    QList<ServerUser*>   Users;
    QStringList          OnlineUsers;
    unsigned int         MaxUserId = 0;
    unsigned int         MaxDocsId = 0;
    QList<Doc*>*         Docs;
    Crypto*              Cryptography;
    Protocol*            Prot;
    int port;
    void InitListener(int port);
    void ParseUserInfo();
    void LoadDocXml(QString FileName);
    void SaveDocXml(Doc* d,QString FileName);
public:
    explicit Server(QObject *parent, QList<Doc*>* D,Crypto* C,Protocol* Proto, int ServerPort);
    void MakeNewUser(QString NewName, QString NewPass);
    void MakeNewDoc(QString DocName, QString DocFileName,QByteArray buff,unsigned int OwnerId);
    void LoadUsers();
    void LoadDocs();
    void SendOnlineUsers();
    void SendSRD();
    void SendDocsList();
    void SendUsersList();
    void SendMessageToAll(QString message);
signals:
    void UsersListChanged(QList<ServerUser*>* UsersList);
    void DocsListChanged(QList<Doc*>* DocsList);
    void OnlineUsersListChanged(QStringList OnlineUsersList);
    void ChatMessage(QString message);
private slots:
    void on_GiveReadPerm(unsigned int UserId, unsigned int DocId);
    void on_RmReadPerm(unsigned int UserId, unsigned int DocId);
    void on_DownloadDoc(unsigned int UserId,unsigned int DocId);
public slots:
    void on_NewUserConnection(QString login,QString pass,QTcpSocket* socket);
    void on_MakeNewUser(QString NewName, QString NewPass);
    void on_MakeNewDoc(QString DocName,QString FileName,QByteArray buff,unsigned int OwnerId);
    void on_NewMessageToChat(QString message);
    void on_UserConnect(QString UserName);
    void on_UserDisconnect(QString UserName);
};

#endif // SERVER_H
