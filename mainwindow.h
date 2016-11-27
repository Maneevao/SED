#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "crypto.h"
#include "doc.h"
#include "protocol.h"
#include "client.h"
#include "server.h"

#include "serverdialog.h"
#include "clientdialog.h"
#include "newuserdialog.h"
#include "newdocdialog.h"
#include "doccontroldialog.h"
#include "usercontroldialog.h"
#include "srddialog.h"
#include "downloaddialog.h"

typedef int AppStatus;
#define APP_OFF   0
#define CLIENT_ON 1
#define SERVER_ON 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Protocol* Prot;
    Crypto* Crypt;
    QList<Doc*> Docs;

    Client* Ct;
    Server* Sr;

    ClientDialog*      CD;
    ServerDialog*      SD;
    NewUserDialog*     NUD;
    NewDocDialog*      NDD;
    DocControlDialog*  DCD;
    UserControlDialog* UCD;
    SRDDialog*         SRDD;
    DownloadDialog*    DD;


    AppStatus AS = APP_OFF;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionClient_triggered();

    void on_actionServer_triggered();

    void on_actionExit_triggered();

    void on_ServerStart(int ServerPort);
    void on_NoServer();

    void on_ClientStart(QString IP, int port, QString login, QString pass);
    void on_NoClient();

    void on_MainWindow_destroyed();

    void CanNotConnect();

    void on_actionNew_User_triggered();
    void on_MakeNewUser(QString Name, QString pass);
    void on_MakeNewDoc(QString DocName, QString DocPath);

    void on_actionControl_docs_triggered();

    void on_actionControl_users_triggered();

    void on_OnlineUsersListChanged(QStringList OnlineUsersList);

    void on_pushButton_clicked();
    void on_NewChatMessage(QString message);

    void on_actionMake_New_Doc_triggered();

    void on_actionNew_document_triggered();

    void on_actionDownload_documents_triggered();

    void on_OpenDownloadDir();

private:
    Ui::MainWindow *ui;
    void ASChange(AppStatus NewAS);
    void GetDocs();
    void SaveDocs();
signals:
    void SetCrypto(Crypto* C);
};

#endif // MAINWINDOW_H
