#include "newuserdialog.h"
#include "ui_newuserdialog.h"

NewUserDialog::NewUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUserDialog)
{
    ui->setupUi(this);
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}

void NewUserDialog::on_pushButton_2_clicked()
{
    emit Cancel();
    close();
}

void NewUserDialog::on_pushButton_clicked()
{
    QString login = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    if (!(login.isEmpty() && pass.isEmpty()))
    {
        emit MakeNewUser(login,pass);
        close();
    }
}
