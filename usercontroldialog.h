#ifndef USERCONTROLDIALOG_H
#define USERCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QList>

#include "serveruser.h"
#include "commonheader.h"
#include "crypto.h"

namespace Ui {
class UserControlDialog;
}

class UserControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserControlDialog(QWidget *parent = 0);
    ~UserControlDialog();

private:
    Ui::UserControlDialog *ui;
    QList<ServerUser*>* UsersCopy;
    Crypto* CryptoCopy;
public slots:
    void on_UserListChange(QList<ServerUser*>* UsersList);
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_SetCrypto(Crypto* C);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_pushButton_3_clicked();
};

#endif // USERCONTROLDIALOG_H
