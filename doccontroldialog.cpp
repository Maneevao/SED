#include "doccontroldialog.h"
#include "ui_doccontroldialog.h"

DocControlDialog::DocControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocControlDialog)
{
    ui->setupUi(this);
}

DocControlDialog::~DocControlDialog()
{
    delete ui;
}

void DocControlDialog::on_DocListChanged(QList<Doc *> *DocsList)
{
    DocsCopy = DocsList;
    ui->treeWidget->clear();
    foreach(Doc* d, *DocsList)
    {
        QTreeWidgetItem* i = new QTreeWidgetItem();
        i->setText(0,QString::number(d->GetId()));
        i->setText(1,d->GetName());
        i->setText(2,d->GetFileName());
        ui->treeWidget->addTopLevelItem(i);
    }
}

void DocControlDialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    unsigned int DocId = item->text(0).toInt();
    foreach (Doc* d, *DocsCopy) {
        if (d->GetId() == DocId)
        {
            ui->lineEdit->setText(d->GetName());
            ui->lineEdit_2->setText(d->GetFileName());
            ui->lineEdit_3->setText(QString::number(d->GetId()));
            ui->lineEdit_4->setText(QString::number(d->GetOwnerUserId()));
            break;
        }
    }
}
