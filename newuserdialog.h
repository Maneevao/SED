#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserDialog(QWidget *parent = 0);
    ~NewUserDialog();

private:
    Ui::NewUserDialog *ui;
signals:
    void MakeNewUser(QString name,QString pass);
    void Cancel();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // NEWUSERDIALOG_H
