#include "newdocdialog.h"
#include "ui_newdocdialog.h"

NewDocDialog::NewDocDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDocDialog)
{
    ui->setupUi(this);
}

NewDocDialog::~NewDocDialog()
{
    delete ui;
}

void NewDocDialog::on_pushButton_3_clicked()
{
    QFileDialog* FD = new QFileDialog();
    QString FilePath = FD->getOpenFileName(this,"","","");
    ui->lineEdit_2->setText(FilePath);
}

void NewDocDialog::on_pushButton_clicked()
{
    close();
}

void NewDocDialog::on_pushButton_2_clicked()
{
    emit MakeNewDoc(ui->lineEdit->text(),ui->lineEdit_2->text());
    close();
}
