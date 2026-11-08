#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchDialogClass;
}
QT_END_NAMESPACE

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog() override;

signals:
    void findRequested(const QString &text, bool caseSensitive, bool backward);
    void findAllRequested(const QString &text, bool caseSensitive);

private slots:
    void on_pushButton_clicked();
    void on_pushButtonFindAll_clicked();

private:
    Ui::SearchDialogClass *ui;
};

#endif // SEARCHDIALOG_H
