#include "downloaddialog.h"
#include "ui_downloaddialog.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::on_NewDocsList(QString DocsList)
{
    ui->treeWidget->clear();
    QStringList tmpList = DocsList.split(",");
    foreach(QString str,tmpList)
    {
        QStringList tmp = str.split(":");
        QTreeWidgetItem* itm = new QTreeWidgetItem();
        itm->setText(0,tmp[0]);
        itm->setText(1,tmp[1]);
        itm->setText(2,tmp[2]);
        ui->treeWidget->addTopLevelItem(itm);
    }
}

void DownloadDialog::on_pushButton_2_clicked()
{
    close();
}

void DownloadDialog::on_pushButton_3_clicked()
{
    emit OpenDownloadDir();
}

void DownloadDialog::on_pushButton_clicked()
{
    if (ui->treeWidget->currentItem()->isSelected())
    {
        emit DownloadDoc(ui->treeWidget->currentItem()->text(0).toInt());
    }
}
