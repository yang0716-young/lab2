#include "replacedialog.h"
#include "ui_ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialogClass)
{
    ui->setupUi(this);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_pushButton_clicked()
{
    const QString findText = ui->findEdit->text();
    if (findText.isEmpty()) {
        return;
    }
    const bool caseSensitive = ui->checkBox->isChecked();
    const bool backward = ui->radioButton->isChecked();
    emit findRequested(findText, caseSensitive, backward);
}

void ReplaceDialog::on_pushButton_2_clicked()
{
    const QString findText = ui->findEdit->text();
    const QString replaceText = ui->replaceEdit->text();
    if (findText.isEmpty()) {
        return;
    }
    const bool caseSensitive = ui->checkBox->isChecked();
    const bool backward = ui->radioButton->isChecked();
    emit replaceRequested(findText, replaceText, caseSensitive, backward);
}

void ReplaceDialog::on_pushButton_3_clicked()
{
    const QString findText = ui->findEdit->text();
    const QString replaceText = ui->replaceEdit->text();
    if (findText.isEmpty()) {
        return;
    }
    const bool caseSensitive = ui->checkBox->isChecked();
    emit replaceAllRequested(findText, replaceText, caseSensitive);
}
