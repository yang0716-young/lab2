#include "searchdialog.h"
#include "ui_SearchDialog.h"

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchDialogClass)
{
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_pushButton_clicked()
{
    const QString text = ui->lineEdit->text();
    if (text.isEmpty()) {
        return;
    }
    const bool caseSensitive = ui->checkBox->isChecked();
    const bool backward = ui->radioButton->isChecked();
    emit findRequested(text, caseSensitive, backward);
}

void SearchDialog::on_pushButtonFindAll_clicked()
{
    const QString text = ui->lineEdit->text();
    if (text.isEmpty()) {
        return;
    }
    const bool caseSensitive = ui->checkBox->isChecked();
    emit findAllRequested(text, caseSensitive);
}
