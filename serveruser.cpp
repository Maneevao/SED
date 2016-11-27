#include "serveruser.h"

void ServerUser::AddReadPerm(unsigned int DocId)
{
    if (ReadPerm.indexOf(DocId)==-1)
    {
        ReadPerm.push_back(DocId);
        SaveToFile();
    }
}

void ServerUser::DelReadPerm(unsigned int DocId)
{
    if (ReadPerm.indexOf(DocId)!=-1)
    {
        ReadPerm.removeOne(DocId);
        SaveToFile();
    }
}

ServerUser::ServerUser(QObject *parent, Crypto *C) : QObject(parent)
{
    Cryptography = C;
}

bool ServerUser::isAbleToRead(unsigned int DocId)
{
    if (ReadPerm.indexOf(DocId)!=-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ServerUser::LoadFromFile(QString path)
{
    qDebug() << "ServerUser::LoadFromFile("<<path<<")";
    QFile file(path);

    if (file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        QString ErrorStr;
        int ErrorLine;
        int ErrorColumn;
        if (doc.setContent(&file,false,&ErrorStr,&ErrorLine,&ErrorColumn))
        {
            QDomElement UserElem = doc.firstChildElement();
            for (QDomElement elem = UserElem.firstChildElement(); !elem.isNull(); elem=elem.nextSiblingElement())
            {
                qDebug() << elem.tagName() << elem.text();
                if (elem.tagName() == "id")
                {
                    SetUserId(elem.text().toInt());
                }
                else if (elem.tagName() == "Name")
                {
                    SetUserName(elem.text());
                }
                else if (elem.tagName() == "Pass")
                {
                    SetUserPass(elem.text());
                }
                else if (elem.tagName() == "Blocked")
                {
                    SetUserBlockValue(elem.text().toInt());
                }
                else if (elem.tagName() == "Perm")
                {
                    QStringList tmpList = elem.text().split(",");
                    foreach (QString tmpStr,tmpList)
                    {
                        AddReadPerm(tmpStr.toInt());
                    }
                }
            }
        }
        else
        {
            qDebug() << ErrorStr << ErrorLine << ErrorColumn;
        }
        file.close();
    }

}

void ServerUser::SaveToFile()
{
    QDomDocument doc("");
    QDomElement  UserElem = doc.createElement("User");
    doc.appendChild(UserElem);

    QDomElement  idElem = doc.createElement("id");
    QDomText     idText = doc.createTextNode(QString::number(id));
    idElem.appendChild(idText);

    QDomElement  NameElem = doc.createElement("Name");
    QDomText     NameText = doc.createTextNode(Name);
    NameElem.appendChild(NameText);

    QDomElement  PassElem = doc.createElement("Pass");
    QDomText     PassText = doc.createTextNode(Pass);
    PassElem.appendChild(PassText);

    QDomElement  BlockElem = doc.createElement("Blocked");
    QDomText     BlockText = doc.createTextNode(QString::number(Blocked));
    BlockElem.appendChild(BlockText);

    QDomElement  PermElem  = doc.createElement("Perm");
    QString tmpPerm;
    foreach (unsigned int DocId,ReadPerm)
    {
        if (tmpPerm == "")
        {
            tmpPerm = QString::number(DocId);
        }
        else
        {
            tmpPerm = tmpPerm + "," + QString::number(DocId);
        }
    }
    QDomText PermText = doc.createTextNode(tmpPerm);
    PermElem.appendChild(PermText);

    UserElem.appendChild(   idElem);
    UserElem.appendChild( NameElem);
    UserElem.appendChild( PassElem);
    UserElem.appendChild(BlockElem);
    UserElem.appendChild( PermElem);

    QFile file("Server/Users/"+QString::number(id)+".xml");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}

void ServerUser::SetUserName(QString NewName)
{
    Name = NewName;
}

QString ServerUser::GetUserName()
{
    return Name;
}

void ServerUser::SetUserPass(QString NewPass)
{
    Pass = NewPass;
    PassMd5Hash = Cryptography->Md5Pass(NewPass);
    key = Cryptography->GenKey(PassMd5Hash);
}

QString ServerUser::GetUserPass()
{
    return Pass;
}

QString ServerUser::GetUserMd5Hash()
{
    return PassMd5Hash;
}

quint64 ServerUser::GetUserKey()
{
    return key;
}

void ServerUser::SetUserId(unsigned int NewId)
{
    id = NewId;
}

unsigned int ServerUser::GetUserId()
{
    return id;
}

void ServerUser::SetUserBlockValue(int NewBlocked)
{
    Blocked = NewBlocked;
}

BlockStatus ServerUser::GetBlockValue()
{
    return Blocked;
}

bool ServerUser::isOnline()
{
    if (OS == OS_ONLINE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int ServerUser::Authorization(QString Login, QString Password, QTcpSocket* socket)
{
    qDebug() << "ServerUser::Authorization - " << Login << ":" << Password;
    if (Blocked == BS_NOT_BLOCKED && Login == Name && Password == PassMd5Hash)
    {
        TM = new TcpMessager(this,socket,Cryptography,key);
        connect(TM,SIGNAL(NewMessage(QByteArray&)),this,SLOT(on_NewMessage(QByteArray&)));
        connect(TM,SIGNAL(NoConnection()),this,SLOT(on_NoConnection()));
        OS = OS_ONLINE;
        emit UserConnect(Name);
        qDebug() << "ServerUser::Authorization - success";
    }
    else
    {
        qDebug() << "ServerUser::Authorization - failed";
        return 1;
    }
    return 0;
}

void ServerUser::SendOnlineUsers(QString OnlineUsers)
{
    TM->sendString("OnlineUsers");
    TM->sendString(OnlineUsers);
}

void ServerUser::SendUsersList(QString UsersList)
{
    TM->sendString("UsersList");
    TM->sendString(UsersList);
}

void ServerUser::SendSRD(QString DocList, QString UsersListWithPerm)
{
    TM->sendString("SRD");
    TM->sendString(DocList + ";" + UsersListWithPerm);
}

void ServerUser::SendDocsList(QString DocsList)
{
    TM->sendString("DocsList");
    TM->sendString(DocsList);
}

void ServerUser::SendMessage(QString message)
{
    TM->sendString("ChatMessage");
    TM->sendString(message);

}

void ServerUser::SendDoc(QString FileName, QByteArray FileBuff)
{
    TM->sendString("File");
    TM->sendString(FileName);
    TM->sendArray(&FileBuff);
}

void ServerUser::on_NewMessage(QByteArray& message)
{
    //TODO: work with message
    //qDebug() << message;
    if (CommandFromUser == "")
    {
        CommandFromUser = TC->toUnicode(message);
        qDebug() << CommandFromUser;
    }
    else
    {
        if (CommandFromUser=="ChatMessage")
        {
            emit NewChatMessage(Name+" : "+TC->toUnicode(message));
        }
        else if (CommandFromUser=="NewDocument")
        {
            qDebug() << "NewDocument";
            QStringList tmpList = TC->toUnicode(message).split(":");
            NewDocName     = tmpList[0];
            qDebug() << "DocName = " << NewDocName;
            NewDocFileName = tmpList[1];
            qDebug() << "FileName = " << NewDocFileName;
        }
        else if (CommandFromUser=="DocumentBuff")
        {
            qDebug() << "Document buff";
            emit MakeNewDoc(NewDocName,NewDocFileName,message,id);
        }
        else if (CommandFromUser == "GiveReadPerm")
        {
            QStringList tmpList = TC->toUnicode(message).split(":");
            emit GiveReadPerm(tmpList[0].toInt(),tmpList[1].toInt());
        }
        else if (CommandFromUser == "RmReadPerm")
        {
            QStringList tmpList = TC->toUnicode(message).split(":");
            emit RmReadPerm(tmpList[0].toInt(),tmpList[1].toInt());
        }
        else if (CommandFromUser == "DownloadDoc")
        {
            emit DownloadDoc(id,TC->toUnicode(message).toInt());
        }

        //Clean CommandFromUser or Set new value
        if (CommandFromUser == "NewDocument")
        {
            CommandFromUser = "DocumentBuff";
        }
        else
        {
            CommandFromUser = "";
        }
    }
    //TODO: another command*/
}

void ServerUser::on_NoConnection()
{
    OS = OS_OFFLINE;
    delete TM;
    emit UserDisconnect(Name);
}
