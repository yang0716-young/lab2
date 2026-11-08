#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class AboutDialogClass;
}
QT_END_NAMESPACE

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private:
    Ui::AboutDialogClass *ui;
};

#endif // ABOUTDIALOG_H
