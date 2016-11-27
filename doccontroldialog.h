#ifndef DOCCONTROLDIALOG_H
#define DOCCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "doc.h"

namespace Ui {
class DocControlDialog;
}

class DocControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocControlDialog(QWidget *parent = 0);
    ~DocControlDialog();
public slots:
    void on_DocListChanged(QList<Doc*>* DocsList);

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::DocControlDialog *ui;
    QList<Doc*>* DocsCopy;
};

#endif // DOCCONTROLDIALOG_H
