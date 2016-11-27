#include "serverdialog.h"
#include "ui_serverdialog.h"

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::on_buttonBox_accepted()
{
    int ServerPort;
    ServerPort = ui->lineEdit->text().toInt();
    emit NewServerPort(ServerPort);
}

void ServerDialog::on_buttonBox_rejected()
{
    emit NoServer();
}
