#include "usercontroldialog.h"
#include "ui_usercontroldialog.h"

UserControlDialog::UserControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserControlDialog)
{
    ui->setupUi(this);
}

UserControlDialog::~UserControlDialog()
{
    delete ui;
}

void UserControlDialog::on_UserListChange(QList<ServerUser*>* UsersList)
{
    UsersCopy = UsersList;
    ui->treeWidget->clear();
    foreach(ServerUser* u, *UsersList)
    {
        QTreeWidgetItem* i = new QTreeWidgetItem();
        i->setText(0,QString::number(u->GetUserId()));
        i->setText(1,u->GetUserName());
        ui->treeWidget->addTopLevelItem(i);
    }
}

void UserControlDialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    unsigned int UserId = item->text(0).toInt();
    foreach (ServerUser* u, *UsersCopy) {
        if (u->GetUserId() == UserId)
        {
            ui->lineEdit->setText(QString::number(u->GetUserId()));
            ui->lineEdit_2->setText(u->GetUserName());
            ui->lineEdit_3->setText(u->GetUserPass());
            QString KeyS;
            KeyS.setNum(u->GetUserKey(),16);
            ui->lineEdit_4->setText(KeyS);
            ui->lineEdit_5->setText(u->GetUserMd5Hash());
            if (u->GetBlockValue() == BS_BLOCKED)
            {
                ui->checkBox->setChecked(true);
            }
            else
            {
                ui->checkBox->setChecked(false);
            }
            break;
        }
    }
}

void UserControlDialog::on_SetCrypto(Crypto *C)
{
    CryptoCopy = C;
}

void UserControlDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    QString md5 = CryptoCopy->Md5Pass(arg1);
    QString KeyS;
    KeyS.setNum(CryptoCopy->GenKey(md5),16);
    ui->lineEdit_4->setText(KeyS);
    ui->lineEdit_5->setText(md5);
}

void UserControlDialog::on_pushButton_3_clicked()
{
    unsigned int UserId = ui->lineEdit->text().toInt();
    foreach (ServerUser* u, *UsersCopy) {
        if (u->GetUserId() == UserId)
        {
            u->SetUserName(ui->lineEdit_2->text());
            u->SetUserPass(ui->lineEdit_3->text());
            if (ui->checkBox->isChecked())
            {
                u->SetUserBlockValue(BS_BLOCKED);
            }
            else
            {
                u->SetUserBlockValue(BS_NOT_BLOCKED);
            }
            u->SaveToFile();
            break;
        }
    }
}
