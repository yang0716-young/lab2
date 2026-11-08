#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ReplaceDialogClass;
}
QT_END_NAMESPACE

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog() override;

signals:
    void findRequested(const QString &text, bool caseSensitive, bool backward);
    void replaceRequested(const QString &findText, const QString &replaceText,
                          bool caseSensitive, bool backward);
    void replaceAllRequested(const QString &findText, const QString &replaceText,
                             bool caseSensitive);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::ReplaceDialogClass *ui;
};

#endif // REPLACEDIALOG_H
