#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Prot  = new Protocol();
    Crypt = new Crypto();
    GetDocs();
    SD = new ServerDialog();
    connect(SD,SIGNAL(NewServerPort(int)),this,SLOT(on_ServerStart(int)));
    connect(SD,SIGNAL(NoServer()),this,SLOT(on_NoServer()));
    CD = new ClientDialog();
    connect(CD,SIGNAL(NewClient(QString,int,QString,QString)),this,SLOT(on_ClientStart(QString,int,QString,QString)));
    connect(CD,SIGNAL(NoClient()),this,SLOT(on_NoClient()));
    NUD = new NewUserDialog();
    NDD = new NewDocDialog();

    UCD = new UserControlDialog();
    DCD = new DocControlDialog();

    SRDD = new SRDDialog();
    DD = new DownloadDialog();
    connect(DD,SIGNAL(OpenDownloadDir()),this,SLOT(on_OpenDownloadDir()));

    emit SetCrypto(Crypt);
}

void MainWindow::GetDocs()
{

}

void MainWindow::SaveDocs()
{

}

MainWindow::~MainWindow()
{
    ASChange(APP_OFF);
    delete CD;
    delete SD;
    delete NUD;
    delete NDD;
    delete UCD;
    delete DCD;
    delete Crypt;
    delete Prot;
    delete SRDD;
    delete DD;
    SaveDocs();
    delete ui;
}

void MainWindow::ASChange(AppStatus NewAS)
{
    if (AS == APP_OFF)
    {
        if (NewAS == CLIENT_ON)
        {
            CD->exec();
        }
        else if(NewAS == SERVER_ON)
        {
            SD->exec();
        }
    }
    else if (NewAS == APP_OFF)
    {
        if (AS == CLIENT_ON)
        {
            delete Ct;
            ui->actionClient->setChecked(false);
            ui->actionClient->setText("Client: OFF");
            ui->actionServer->setEnabled(true);
            ui->menuClient->setEnabled(false);
            AS=APP_OFF;
        }
        else if (AS == SERVER_ON)
        {
            delete Sr;
            ui->actionServer->setChecked(false);
            ui->actionServer->setText("Server: OFF");
            ui->actionClient->setEnabled(true);
            ui->menuServer->setEnabled(false);
            AS=APP_OFF;
        }
    }
}

void MainWindow::on_actionClient_triggered()
{
    if (AS == CLIENT_ON)
    {
        ASChange(APP_OFF);
    }
    else if (AS == APP_OFF)
    {
        ASChange(CLIENT_ON);
    }
}

void MainWindow::on_actionServer_triggered()
{
    if (AS == SERVER_ON)
    {
        ASChange(APP_OFF);
    }
    else if (AS == APP_OFF)
    {
        ASChange(SERVER_ON);
    }
}

void MainWindow::on_actionExit_triggered()
{
    ASChange(APP_OFF);
    qApp->exit();
}

void MainWindow::on_ServerStart(int ServerPort)
{
    Sr = new Server(this,&Docs,Crypt,Prot,ServerPort);
    connect(NUD,SIGNAL(MakeNewUser(QString,QString)),Sr,SLOT(on_MakeNewUser(QString,QString)));
    connect(Sr,SIGNAL(DocsListChanged(QList<Doc*>*)),DCD,SLOT(on_DocListChanged(QList<Doc*>*)));
    connect(Sr,SIGNAL(UsersListChanged(QList<ServerUser*>*)),UCD,SLOT(on_UserListChange(QList<ServerUser*>*)));
    connect(Sr,SIGNAL(OnlineUsersListChanged(QStringList)),this,SLOT(on_OnlineUsersListChanged(QStringList)));
    connect(Sr,SIGNAL(ChatMessage(QString)),this,SLOT(on_NewChatMessage(QString)));
    ui->actionServer->setChecked(true);
    ui->actionServer->setText("Server: ON");
    ui->actionClient->setEnabled(false);
    ui->menuServer->setEnabled(true);
    AS=SERVER_ON;
    Sr->LoadUsers();
    Sr->LoadDocs();
    Sr->SendSRD();
}

void MainWindow::on_NoServer()
{
    ui->actionServer->setChecked(false);
}

void MainWindow::on_ClientStart(QString IP, int port, QString login, QString pass)
{
    Ct = new Client(this,&Docs,Crypt,Prot,IP,port,login,pass);
    connect(Ct,SIGNAL(NoServer()),this,SLOT(CanNotConnect()));
    connect(NDD,SIGNAL(MakeNewDoc(QString,QString)),Ct,SLOT(on_MakeNewDoc(QString,QString)));
    connect(Ct,SIGNAL(OnlineUsersListChanged(QStringList)),this,SLOT(on_OnlineUsersListChanged(QStringList)));
    connect(Ct,SIGNAL(NewChatMessage(QString)),this,SLOT(on_NewChatMessage(QString)));
    connect(Ct,SIGNAL(NewUsersList(QString)),SRDD,SLOT(on_NewUsersList(QString)));
    connect(Ct,SIGNAL(NewDocList(QString)),SRDD,SLOT(on_NewDocsList(QString)));
    connect(SRDD,SIGNAL(GiveReadPerm(uint,uint)),Ct,SLOT(on_GiveReadPerm(uint,uint)));
    connect(SRDD,SIGNAL(RmReadPerm(uint,uint)),Ct,SLOT(on_RmReadPerm(uint,uint)));
    connect(Ct,SIGNAL(NewDocsList(QString)),DD,SLOT(on_NewDocsList(QString)));
    connect(DD,SIGNAL(DownloadDoc(uint)),Ct,SLOT(on_DownloadDoc(uint)));
    ui->actionClient->setChecked(true);
    ui->actionClient->setText("Client: ON");
    ui->actionServer->setEnabled(false);
    ui->menuClient->setEnabled(true);
    AS=CLIENT_ON;
}

void MainWindow::on_NoClient()
{
    ui->actionClient->setChecked(false);
}

void MainWindow::on_MainWindow_destroyed()
{
    ASChange(APP_OFF);
}

void MainWindow::CanNotConnect()
{
    ASChange(APP_OFF);
}

void MainWindow::on_actionNew_User_triggered()
{
    NUD->exec();
}

void MainWindow::on_MakeNewUser(QString Name, QString pass)
{
    if (AS == SERVER_ON)
    {
        Sr->MakeNewUser(Name,pass);
    }
}

void MainWindow::on_MakeNewDoc(QString DocName, QString DocPath)
{
    qDebug() << "Im here2";
    if (AS == CLIENT_ON)
    {
        Ct->MakeNewDocument(DocName,DocPath);
    }
}

void MainWindow::on_actionControl_docs_triggered()
{
    DCD->exec();
}

void MainWindow::on_actionControl_users_triggered()
{
    UCD->exec();
}

void MainWindow::on_OnlineUsersListChanged(QStringList OnlineUsersList)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(OnlineUsersList);
}

void MainWindow::on_pushButton_clicked()
{
    if (AS==SERVER_ON)
    {
        QString m = "Server: " + ui->textEdit_2->toPlainText();
        Sr->SendMessageToAll(m);
        on_NewChatMessage(m);
        ui->textEdit_2->clear();
    }
    else if (AS==CLIENT_ON)
    {
        Ct->SendMessage(ui->textEdit_2->toPlainText());
        ui->textEdit_2->clear();
    }
}

void MainWindow::on_NewChatMessage(QString message)
{
    ui->textEdit->append(message);
}

void MainWindow::on_actionMake_New_Doc_triggered()
{
    NDD->exec();
}

void MainWindow::on_actionNew_document_triggered()
{
    SRDD->exec();
}

void MainWindow::on_actionDownload_documents_triggered()
{
    DD->exec();
}

void MainWindow::on_OpenDownloadDir()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()+"/"+Ct->GetName()+"/Docs"));
}
