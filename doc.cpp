#include "doc.h"

Doc::Doc(QObject *parent) : QObject(parent)
{

}

void Doc::MakeNewDoc(unsigned int DocId,QString DocName, QString DocFileName, QByteArray buff, unsigned int OwnerId)
{
    SetId(DocId);
    SetName(DocName);
    SetFileName(DocFileName);
    SetOwnerUserId(OwnerId);
    SaveXml();
    QFile file("Server/Docs/"+FileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(buff);
    }
}

void Doc::SaveXml()
{
    QDomDocument doc("");
    QDomElement  DocElem = doc.createElement("Document");
    doc.appendChild(DocElem);

    QDomElement  idElem = doc.createElement("id");
    QDomText     idText = doc.createTextNode(QString::number(id));
    idElem.appendChild(idText);

    QDomElement  NameElem = doc.createElement("Name");
    QDomText     NameText = doc.createTextNode(Name);
    NameElem.appendChild(NameText);

    QDomElement  OwnerIdElem = doc.createElement("OwnerUserId");
    QDomText     OwnerIdText = doc.createTextNode(QString::number(OwnerUserId));
    OwnerIdElem.appendChild(OwnerIdText);

    QDomElement  PathElem = doc.createElement("FileName");
    QDomText     PathText = doc.createTextNode(FileName);
    PathElem.appendChild(PathText);

    DocElem.appendChild(     idElem);
    DocElem.appendChild(   NameElem);
    DocElem.appendChild(OwnerIdElem);
    DocElem.appendChild(   PathElem);

    QFile file("Server/Docs/Info/"+QString::number(id)+".xml");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}

void Doc::LoadDocWithXml(QString path)
{
    qDebug() << "Doc::LoadDocWithXml("<<path<<")";
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
                    SetId(elem.text().toInt());
                }
                else if (elem.tagName() == "Name")
                {
                    SetName(elem.text());
                }
                else if (elem.tagName() == "OwnerUserId")
                {
                    SetOwnerUserId(elem.text().toInt());
                }
                else if (elem.tagName() == "FileName")
                {
                    SetFileName(elem.text());
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

unsigned int Doc::GetId()
{
    return id;
}

void Doc::SetId(unsigned int NewId)
{
    id = NewId;
}

QString Doc::GetName()
{
    return Name;
}

void Doc::SetName(QString NewName)
{
    Name = NewName;
}

unsigned int Doc::GetOwnerUserId()
{
    return OwnerUserId;
}

void Doc::SetOwnerUserId(unsigned int NewOwnerUserId)
{
    OwnerUserId = NewOwnerUserId;
}

QByteArray Doc::GetDoc()
{
    QByteArray result;
    QFile file("Server/Docs/"+FileName);
    if (file.open(QIODevice::ReadOnly))
    {
        result = file.readAll();
    }
    return result;
}

void Doc::SetFileName(QString NewFileName)
{
    FileName = NewFileName;
}

QString Doc::GetFileName()
{
    return FileName;
}

