#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QString>

class Protocol: public QObject
{
    Q_OBJECT
public:
    Protocol();
    QString Authentication(QString str);
};

#endif // PROTOCOL_H
