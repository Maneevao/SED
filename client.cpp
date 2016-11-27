#include "client.h"

void Client::ConnectToServer()
{
    md5Pass = Cryptography->Md5Pass(Pass);
    key = Cryptography->GenKey(md5Pass);
    ClientSocket = new QTcpSocket(this);
    ClientSocket->connectToHost(ServerIP,ServerPort);
    connect(ClientSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_SocketStatusChange(QAbstractSocket::SocketState)));
}

Client::Client(QObject *parent, QList<Doc*>* D, Crypto* C, Protocol* Proto, QString ip, int port, QString Login, QString Password) : QObject(parent)
{
    Docs = D;
    Cryptography = C;
    Prot = Proto;

    ServerIP = ip;
    ServerPort = port;

    Name = Login;
    Pass = Password;

    ConnectToServer();

    if (!QDir(Name).exists())
    {
        QDir().mkdir(Name);
    }
    if (!QDir(Name+"/Docs").exists())
    {
        QDir().mkdir(Name+"/Docs");
    }
}

void Client::SendMessage(QString message)
{
    TM->sendString("ChatMessage");
    TM->sendString(message);
}

void Client::MakeNewDocument(QString DocName, QString FilePath)
{
    QFile file(FilePath);
    if (file.exists())
    {
        TM->sendString("NewDocument");
        QFileInfo FI(FilePath);
        TM->sendString(DocName+":"+FI.fileName());
        file.open(QIODevice::ReadOnly);
        QByteArray buff = file.readAll();
        file.close();
        TM->sendArray(&buff);
    }
}

void Client::GiveReadPerm(unsigned int UserId, unsigned int DocId)
{
    TM->sendString("GiveReadPerm");
    TM->sendString(QString::number(UserId)+":"+QString::number(DocId));
}

void Client::RmReadPerm(unsigned int UserId, unsigned int DocId)
{
    TM->sendString("RmReadPerm");
    TM->sendString(QString::number(UserId)+":"+QString::number(DocId));
}

void Client::DownloadDoc(unsigned int DocId)
{
    TM->sendString("DownloadDoc");
    TM->sendString(QString::number(DocId));
}

QString Client::GetName()
{
    return Name;
}

void Client::on_SocketStatusChange(QAbstractSocket::SocketState SockState)
{
    //TODO: move it to tcpmessager
    if (PrevSockState == QAbstractSocket::ConnectingState ||
        PrevSockState == QAbstractSocket::UnconnectedState)
    {
        if(SockState == QAbstractSocket::ConnectedState)
        {
            Authentication();
        }
        else if (SockState == QAbstractSocket::UnconnectedState)
        {
            emit NoServer();
        }
    }
    else if (SockState == QAbstractSocket::UnconnectedState)
    {
        emit NoServer();
    }
    PrevSockState = SockState;
}

void Client::on_MakeNewDoc(QString DocName, QString FilePath)
{
    MakeNewDocument(DocName,FilePath);
}

void Client::on_GiveReadPerm(unsigned int UserId, unsigned int DocId)
{
    GiveReadPerm(UserId,DocId);
}

void Client::on_RmReadPerm(unsigned int UserId, unsigned int DocId)
{
    RmReadPerm(UserId,DocId);
}

void Client::on_NoConnection()
{
    emit NoServer();
}

void Client::on_NewMessage(QByteArray& message)
{
    //qDebug() << message;
    if (CommandFromServer == "")
    {
        CommandFromServer = TC->toUnicode(message);
        qDebug() << CommandFromServer;
    }
    else
    {
        if (CommandFromServer == "ChatMessage")
        {
            emit NewChatMessage(TC->toUnicode(message));
        }
        else if (CommandFromServer == "OnlineUsers")
        {
            emit OnlineUsersListChanged(TC->toUnicode(message).split(","));
        }
        else if (CommandFromServer == "UsersList")
        {
            emit UsersListChanged(TC->toUnicode(message).split(","));
        }
        else if (CommandFromServer == "SRD")
        {
            QStringList tmpList = TC->toUnicode(message).split(";");
            QStringList DocList = tmpList[0].split(",");
            QString DC = DocList[0];
            unsigned int DocCount = DC.toInt();
            if (DocCount != 0)
            {
                qDebug() << tmpList[0];
                emit NewDocList(tmpList[0]);
            }
            QStringList UserList = tmpList[1].split(",");
            QString UC = UserList[0];
            unsigned int UserCount = UC.toInt();
            if (UserCount != 0)
            {
                qDebug() << tmpList[1];
                emit NewUsersList(tmpList[1]);
            }
        }
        else if (CommandFromServer =="DocsList")
        {
            if (message!="")
            {
                emit NewDocsList(TC->toUnicode(message));
            }
        }
        else if (CommandFromServer == "File")
        {
            FileName = TC->toUnicode(message);
        }
        else if (CommandFromServer == "FileBuff")
        {
            QFile file(Name+"/Docs/"+FileName);
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(message);
            }
        }

        if (CommandFromServer == "File")
        {
            CommandFromServer = "FileBuff";
        }
        else
        {
            CommandFromServer = "";
        }
    }
    //TODO: another command*/
}

void Client::on_DownloadDoc(unsigned int DocId)
{
    DownloadDoc(DocId);
}

void Client::Authentication()
{
    disconnect(ClientSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_SocketStatusChange(QAbstractSocket::SocketState)));
    TM = new TcpMessager(this,ClientSocket,Cryptography,key);
    connect(TM,SIGNAL(NoConnection()),this,SLOT(on_NoConnection()));
    connect(TM,SIGNAL(NewMessage(QByteArray&)),this,SLOT(on_NewMessage(QByteArray&)));
    TM->sendString(Name+":"+md5Pass);
}
