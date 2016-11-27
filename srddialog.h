#ifndef SRDDIALOG_H
#define SRDDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class SRDDialog;
}

class SRDDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SRDDialog(QWidget *parent = 0);
    ~SRDDialog();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

public slots:
    void on_NewUsersList(QString UsersList);
    void on_NewDocsList(QString DocsList);

private:
    Ui::SRDDialog *ui;
signals:
    void RmReadPerm(unsigned int UserId,unsigned int DocId);
    void GiveReadPerm(unsigned int UserId,unsigned int DocId);
};

#endif // SRDDIALOG_H
