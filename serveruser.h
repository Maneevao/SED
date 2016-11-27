#ifndef SERVERUSER_H
#define SERVERUSER_H

#include <QObject>
#include <QDebug>
#include <QtXml>

#include "crypto.h"
#include "tcpmessager.h"
#include "commonheader.h"

class ServerUser : public QObject
{
    Q_OBJECT
private:
    unsigned int id;
    QString CommandFromUser = "";
    QString Name;
    QString Pass;
    QString PassMd5Hash;
    quint64 key;
    Crypto* Cryptography;
    OnlineStatus OS = OS_OFFLINE;
    TcpMessager* TM;
    QList<unsigned int> ReadPerm; //TODO: make it
    BlockStatus Blocked;
    QTextCodec* TC = QTextCodec::codecForName("UTF-16BE");
    QString NewDocName, NewDocFileName;
public:
    ServerUser(QObject *parent, Crypto* C);
    bool isAbleToRead(unsigned int DocId);
    void AddReadPerm(unsigned int DocId);
    void DelReadPerm(unsigned int DocId);
    void LoadFromFile(QString path);
    void SaveToFile();
    void SetUserName(QString NewName);
    QString GetUserName();
    void SetUserPass(QString NewPass);
    QString GetUserPass();
    QString GetUserMd5Hash();
    quint64 GetUserKey();
    void SetUserId(unsigned int NewId);
    unsigned int GetUserId();
    void SetUserBlockValue(int NewBlocked);
    BlockStatus GetBlockValue();
    bool isOnline();
    int  Authorization(QString Login, QString Password, QTcpSocket *socket); // 0 - success, 1 - (Password != PassMd5Hash)
    void SendOnlineUsers(QString OnlineUsers);
    void SendUsersList(QString UsersList);
    void SendSRD(QString DocList,QString UsersListWithPerm);
    void SendDocsList(QString DocsList);
    void SendMessage(QString message);
    void SendDoc(QString FileName,QByteArray FileBuff);

signals:
    void MakeNewDoc(QString DocName,QString FileName,QByteArray buff,unsigned int MyId);
    void NewChatMessage(QString message);
    void UserDisconnect(QString UserName);
    void UserConnect(QString UserName);
    void GiveReadPerm(unsigned int UserId,unsigned int DocId);
    void RmReadPerm(unsigned int UserId,unsigned int DocId);
    void DownloadDoc(unsigned int UserId,unsigned int DocId);

private slots:
    void on_NewMessage(QByteArray& message);
    void on_NoConnection();
};

#endif // SERVERUSER_H
