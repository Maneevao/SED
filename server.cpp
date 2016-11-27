#include "server.h"

Server::Server(QObject *parent, QList<Doc*>* D, Crypto *C, Protocol *Proto,int ServerPort) : QObject(parent)
{
    Docs = D;
    Cryptography = C;
    Prot = Proto;
    port = ServerPort;
    InitListener(ServerPort);
    if (!QDir("Server").exists())
    {
        QDir().mkdir("Server");
    }
    if (!QDir("Server/Users").exists())
    {
        QDir().mkdir("Server/Users");
    }
    if (!QDir("Server/Docs").exists())
    {
        QDir().mkdir("Server/Docs");
    }
    if (!QDir("Server/Docs/Info").exists())
    {
        QDir().mkdir("Server/Docs/Info");
    }
}

void Server::LoadUsers()
{
    QDir dir("Server/Users");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList FileList = dir.entryList();
    foreach (QString filePath,FileList)
    {
        qDebug() << "Server::server - New user from " << filePath;
        ServerUser* NewUser = new ServerUser(this,Cryptography);
        NewUser->LoadFromFile("Server/Users/" + filePath);
        Users.push_back(NewUser);
        MaxUserId++;
    }
    emit UsersListChanged(&Users);
}

void Server::LoadDocs()
{
    QDir dir("Server/Docs/Info");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList FileList = dir.entryList();
    foreach (QString filePath,FileList)
    {
        qDebug() << "Server::server - New doc from " << filePath;
        Doc* NewDoc = new Doc(this);
        NewDoc->LoadDocWithXml("Server/Docs/Info/"+filePath);
        Docs->push_back(NewDoc);
        MaxDocsId++;
    }
    emit DocsListChanged(Docs);
}

void Server::SendOnlineUsers()
{
    QString message;
    foreach (QString s,OnlineUsers)
    {
        if (message!="")
        {
            message = message + "," + s;
        }
        else
        {
            message = s;
        }
    }
    foreach (ServerUser* u,Users)
    {
        if (u->isOnline())
        {
            u->SendOnlineUsers(message);
        }
    }
}

void Server::SendSRD()
{
    foreach (ServerUser* U,Users)
    {
        if (U->isOnline())
        {
            QString DocList="";
            unsigned int DocCount = 0;
            QString UserListWithPerm="";
            unsigned int UsersCount = 0;
            foreach (Doc* D,*Docs)
            {
                if (D->GetOwnerUserId() == U->GetUserId())
                {
                    DocList = DocList + "," + QString::number(D->GetId()) + ":" + D->GetName();
                    DocCount = DocCount + 1;
                }
            }
            DocList = QString::number(DocCount) + DocList;
            foreach (ServerUser* SendU,Users)
            {
                UserListWithPerm = UserListWithPerm + "," + QString::number(SendU->GetUserId()) + ":" + SendU->GetUserName();
                foreach (Doc* D, *Docs) {
                    if (D->GetOwnerUserId()==U->GetUserId())
                    {
                        if (SendU->isAbleToRead(D->GetId()))
                        {
                            UserListWithPerm = UserListWithPerm + ":" + QString::number(D->GetId()) + "-R";
                        }
                        else
                        {
                            UserListWithPerm = UserListWithPerm + ":" + QString::number(D->GetId()) + "-0";
                        }
                    }
                }
                UsersCount = UsersCount + 1;
            }
            UserListWithPerm = QString::number(UsersCount) + UserListWithPerm;
            U->SendSRD(DocList,UserListWithPerm);
        }
    }
    SendDocsList();
}

void Server::SendDocsList()
{
    foreach(ServerUser* U,Users)
    {
        if (U->isOnline())
        {
            QString DocsList = "";
            foreach (Doc* D, *Docs)
            {
                if (U->isAbleToRead(D->GetId()))
                {
                    if (DocsList == "")
                    {
                        DocsList = QString::number(D->GetId())+":"+D->GetName()+":"+D->GetFileName();
                    }
                    else
                    {
                        DocsList = DocsList + "," + QString::number(D->GetId())+":"+D->GetName()+":"+D->GetFileName();
                    }
                }
            }
            U->SendDocsList(DocsList);
        }
    }
}

void Server::SendUsersList()
{
    QString message;
    foreach (ServerUser* U,Users)
    {
        if (message!="")
        {
            message = message + "," + U->GetUserId() + ":" + U->GetUserName();
        }
        else
        {
            message = U->GetUserId() + ":" + U->GetUserName();
        }
    }
    foreach (ServerUser* u,Users)
    {
        if (u->isOnline())
        {
            u->SendUsersList(message);
        }
    }
}

void Server::SendMessageToAll(QString message)
{
    foreach (ServerUser* u,Users)
    {
        if (u->isOnline())
        {
            u->SendMessage(message);
        }
    }
}

void Server::on_GiveReadPerm(unsigned int UserId, unsigned int DocId)
{
    foreach (ServerUser* U, Users)
    {
        if (U->GetUserId() == UserId)
        {
            U->AddReadPerm(DocId);
            break;
        }
    }
    SendSRD();
}

void Server::on_RmReadPerm(unsigned int UserId, unsigned int DocId)
{
    foreach (ServerUser* U, Users)
    {
        if (U->GetUserId() == UserId)
        {
            U->DelReadPerm(DocId);
            break;
        }
    }
    SendSRD();
}

void Server::on_DownloadDoc(unsigned int UserId, unsigned int DocId)
{
    foreach (ServerUser* U, Users)
    {
        if (U->GetUserId() == UserId)
        {
            if (U->isAbleToRead(DocId))
            {
                foreach (Doc* D,*Docs)
                {
                    if (D->GetId() == DocId)
                    {
                        U->SendDoc(D->GetFileName(),D->GetDoc());
                        break;
                    }
                }
            }
            break;
        }
    }
}

void Server::MakeNewUser(QString NewName, QString NewPass)
{
    qDebug() << "Server::MakeNewUser - " <<NewName <<":" <<NewPass;
    bool BadTry = false;
    foreach (ServerUser* u, Users) {
        if (u->GetUserName() == NewName)
        {
            BadTry = true;
        }
    }
    if (!BadTry)
    {
        ServerUser* NewUser = new ServerUser(this,Cryptography);
        MaxUserId = MaxUserId + 1;
        NewUser->SetUserId(MaxUserId);
        NewUser->SetUserName(NewName);
        NewUser->SetUserPass(NewPass);
        NewUser->SetUserBlockValue(BS_NOT_BLOCKED);
        NewUser->SaveToFile();
        Users.push_back(NewUser);
        SendSRD();
        emit UsersListChanged(&Users);
    }
    else
    {
        //TODO: write error this user is in the system
    }
}

void Server::MakeNewDoc(QString DocName, QString DocFileName, QByteArray buff, unsigned int OwnerId)
{
    Doc* NewDoc = new Doc();
    MaxDocsId = MaxDocsId + 1;
    NewDoc->MakeNewDoc(MaxDocsId,DocName,DocFileName,buff,OwnerId);
    Docs->push_back(NewDoc);
    foreach (ServerUser* U,Users)
    {
        if (U->GetUserId()==OwnerId)
        {
            U->AddReadPerm(MaxDocsId);
            break;
        }
    }
    SendSRD();
    emit DocsListChanged(Docs);
}

void Server::on_NewUserConnection(QString login, QString pass, QTcpSocket* socket)
{
    bool BadTry = true;
    ServerUser* user;
    foreach (ServerUser* u, Users) {
        if (u->GetUserName() == login)
        {
            if (!u->isOnline() && u->GetBlockValue()!=BS_BLOCKED)
            {
                user = u;
                BadTry = false;
            }
            break;
        }
    }
    if (BadTry)
    {
        socket->disconnectFromHost();
    }
    else
    {
        connect(user,SIGNAL(UserConnect(QString)),this,SLOT(on_UserConnect(QString)),Qt::UniqueConnection);
        connect(user,SIGNAL(UserDisconnect(QString)),this,SLOT(on_UserDisconnect(QString)),Qt::UniqueConnection);
        connect(user,SIGNAL(NewChatMessage(QString)),this,SLOT(on_NewMessageToChat(QString)),Qt::UniqueConnection);
        connect(user,SIGNAL(MakeNewDoc(QString,QString,QByteArray,uint)),this,SLOT(on_MakeNewDoc(QString,QString,QByteArray,uint)),Qt::UniqueConnection);
        connect(user,SIGNAL(GiveReadPerm(uint,uint)),this,SLOT(on_GiveReadPerm(uint,uint)),Qt::UniqueConnection);
        connect(user,SIGNAL(RmReadPerm(uint,uint)),this,SLOT(on_RmReadPerm(uint,uint)),Qt::UniqueConnection);
        connect(user,SIGNAL(DownloadDoc(uint,uint)),this,SLOT(on_DownloadDoc(uint,uint)),Qt::UniqueConnection);
        int AuthTry = user->Authorization(login,pass,socket);
        if (AuthTry == 1)
        {
            socket->disconnectFromHost();
        }
        SendSRD();
    }
}

void Server::on_MakeNewUser(QString NewName, QString NewPass)
{
    MakeNewUser(NewName,NewPass);
}

void Server::on_NewMessageToChat(QString message)
{
    emit ChatMessage(message);
    SendMessageToAll(message);
}

void Server::on_UserConnect(QString UserName)
{
    OnlineUsers.append(UserName);
    SendOnlineUsers();
    emit OnlineUsersListChanged(OnlineUsers);
}

void Server::on_UserDisconnect(QString UserName)
{
    OnlineUsers.removeOne(UserName);
    SendOnlineUsers();
    emit OnlineUsersListChanged(OnlineUsers);
}

void Server::InitListener(int port)
{
    ServerListener = new Listener(this,port);
    connect(ServerListener,SIGNAL(NewUserConnection(QString,QString,QTcpSocket*)),this,SLOT(on_NewUserConnection(QString,QString,QTcpSocket*)));
}

void Server::on_MakeNewDoc(QString DocName, QString FileName, QByteArray buff, unsigned int OwnerId)
{
    MakeNewDoc(DocName,FileName,buff,OwnerId);
}
