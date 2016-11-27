#ifndef DOC_H
#define DOC_H

#include <QObject>
#include <QtXml>
#include <QHash>
#include <QString>
#include <QByteArray>

class Doc : public QObject
{
    Q_OBJECT
private:
    unsigned int id;
    QString Name;
    unsigned int OwnerUserId;
    QString FileName;

    void SetId(unsigned int NewId);
    void SetName(QString NewName);
    void SetOwnerUserId(unsigned int NewOwnerUserId);
    void SetFileName(QString NewFileName);

public:
    Doc(QObject *parent = 0);
    void MakeNewDoc(unsigned int DocId,QString DocName, QString DocFileName,QByteArray buff,unsigned int OwnerId);
    void SaveXml();
    void LoadDocWithXml(QString path);
    unsigned int GetId();
    QString GetName();
    unsigned int GetOwnerUserId();
    QByteArray GetDoc();
    QString GetFileName();

signals:

public slots:
};

#endif // DOC_H
