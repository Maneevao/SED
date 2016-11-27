#include "clientdialog.h"
#include "ui_clientdialog.h"

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::on_buttonBox_rejected()
{
    emit NoClient();
}

void ClientDialog::on_buttonBox_accepted()
{
    QString IP    = ui->lineEdit->text();
    int port      = ui->lineEdit_2->text().toInt();
    QString login = ui->lineEdit_3->text();
    QString pass  = ui->lineEdit_4->text();
    emit NewClient(IP,port,login,pass);
}
