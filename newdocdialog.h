#ifndef NEWDOCDIALOG_H
#define NEWDOCDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class NewDocDialog;
}

class NewDocDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDocDialog(QWidget *parent = 0);
    ~NewDocDialog();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NewDocDialog *ui;
signals:
    void MakeNewDoc(QString DocName,QString DocPath);
};

#endif // NEWDOCDIALOG_H
