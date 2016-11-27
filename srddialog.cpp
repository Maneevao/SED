#include "srddialog.h"
#include "ui_srddialog.h"

SRDDialog::SRDDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SRDDialog)
{
    ui->setupUi(this);
}

SRDDialog::~SRDDialog()
{
    delete ui;
}

void SRDDialog::on_tableWidget_cellClicked(int row, int column)
{

}

void SRDDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
    qDebug() << row << ":" << column;
    QString UserName;
    unsigned int UserId,DocId;

    QTableWidgetItem* itmH = ui->tableWidget->horizontalHeaderItem(column);
    qDebug() << itmH->text();
    UserId = itmH->text().split(":")[0].toInt();
    qDebug() << UserId;
    UserName = itmH->text().split(":")[1];
    qDebug() << UserName;
    QTableWidgetItem* itmV = ui->tableWidget->verticalHeaderItem(row);
    qDebug() << itmV->text();
    DocId = itmV->text().split(":")[0].toInt();
    qDebug() << DocId;
    QTableWidgetItem* itm  = ui->tableWidget->item(row,column);
    qDebug() << itm->text();
    if (itm->text() == "R")
    {
        emit RmReadPerm(UserId,DocId);
        itm->setText("");
    }
    else
    {
        emit GiveReadPerm(UserId,DocId);
        itm->setText("R");
    }
}

void SRDDialog::on_NewUsersList(QString UsersList)
{
    qDebug() << UsersList;
    QStringList tmpList = UsersList.split(",");
    unsigned int UsersCount = tmpList[0].toInt();
    ui->tableWidget->setColumnCount(UsersCount);
    for (int i = 1;i<=UsersCount;i++)
    {
        QStringList tmp = tmpList[i].split(":");
        QTableWidgetItem* itm = new QTableWidgetItem();
        itm->setText(tmp[0]+":"+tmp[1]);
        ui->tableWidget->setHorizontalHeaderItem(i-1,itm);
        for(int j = 2; j<tmp.count(); j++)
        {
            QTableWidgetItem* itm2 = new QTableWidgetItem();
            if (tmp[j].split("-")[1] == "R")
            {
                itm2->setText("R");
            }
            else
            {
                itm2->setText("");
            }
            ui->tableWidget->setItem(j-2,i-1,itm2);
        }
    }
}

void SRDDialog::on_NewDocsList(QString DocsList)
{
    qDebug() << DocsList;
    QStringList tmpList = DocsList.split(",");
    unsigned int DocsCount = tmpList[0].toInt();
    ui->tableWidget->setRowCount(DocsCount);
    qDebug() << DocsCount;
    for (int i = 1; i<=DocsCount; i++)
    {
        QStringList tmp = tmpList[i].split(":");
        QTableWidgetItem* itm = new QTableWidgetItem();
        itm->setText(tmpList[i]);
        ui->tableWidget->setVerticalHeaderItem(i-1,itm);
    }
}
