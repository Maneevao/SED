#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = 0);
    ~DownloadDialog();

public slots:
    void on_NewDocsList(QString DocsList);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::DownloadDialog *ui;
signals:
    void OpenDownloadDir();
    void DownloadDoc(unsigned int DocId);
};

#endif // DOWNLOADDIALOG_H
