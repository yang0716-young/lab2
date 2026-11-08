#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "codeeditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColorDialog>
#include <QFontDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length:" + QString::number(0) + "lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("赵阳"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    // 自动换行默认开启
    ui->actionAutoWrap->setChecked(true);

    // 工具栏、状态栏默认显示
    ui->actionToolBar->setChecked(true);
    ui->actionStatusBar->setChecked(true);

    // 连接信号更新状态栏信息
    connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateStatusInfo);
    connect(ui->textEdit, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPosition);

    // 初始化状态栏
    updateStatusInfo();
    updateCursorPosition();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg(this);
    connect(&dlg, &SearchDialog::findRequested, this, &MainWindow::doFind);
    connect(&dlg, &SearchDialog::findAllRequested, this, &MainWindow::doFindAll);
    dlg.exec();
    ui->textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this);
    connect(&dlg, &ReplaceDialog::findRequested, this, &MainWindow::doFind);
    connect(&dlg, &ReplaceDialog::replaceRequested, this, &MainWindow::doReplace);
    connect(&dlg, &ReplaceDialog::replaceAllRequested, this, &MainWindow::doReplaceAll);
    dlg.exec();
}

void MainWindow::doFind(const QString &text, bool caseSensitive, bool backward)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (backward) {
        flags |= QTextDocument::FindBackward;
    }
    bool found = ui->textEdit->find(text, flags);
    if (!found) {
        QMessageBox::information(this, "查找", "未找到");
    }
}

void MainWindow::doFindAll(const QString &text, bool caseSensitive)
{
    if (text.isEmpty()) {
        return;
    }

    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    QList<QTextEdit::ExtraSelection> selections;

    QTextDocument *doc = ui->textEdit->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);

    int count = 0;
    QTextCursor firstMatch;
    while (true) {
        cursor = doc->find(text, cursor, flags);
        if (cursor.isNull()) {
            break;
        }

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format.setBackground(QColor(Qt::yellow));
        sel.format.setForeground(QColor(Qt::black));
        selections.append(sel);

        if (count == 0) {
            firstMatch = cursor;
        }
        count++;
    }

    ui->textEdit->setExtraSelections(selections);

    if (count == 0) {
        QMessageBox::information(this, "查找全部", "未找到");
    } else {
        ui->textEdit->setTextCursor(firstMatch);
        QMessageBox::information(this, "查找全部", QString("共找到 %1 处").arg(count));
    }
}

void MainWindow::doReplace(const QString &findText, const QString &replaceText,
                           bool caseSensitive, bool backward)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (backward) {
        flags |= QTextDocument::FindBackward;
    }

    QTextCursor cursor = ui->textEdit->textCursor();

    if (cursor.hasSelection() && cursor.selectedText() == findText) {
        cursor.insertText(replaceText);
        ui->textEdit->setTextCursor(cursor);
        return;
    }

    bool found = ui->textEdit->find(findText, flags);
    if (found) {
        QTextCursor foundCursor = ui->textEdit->textCursor();
        foundCursor.insertText(replaceText);
        ui->textEdit->setTextCursor(foundCursor);
    } else {
        QTextCursor resetCursor = ui->textEdit->textCursor();
        resetCursor.movePosition(backward ? QTextCursor::End : QTextCursor::Start);
        ui->textEdit->setTextCursor(resetCursor);
        found = ui->textEdit->find(findText, flags);
        if (found) {
            QTextCursor foundCursor = ui->textEdit->textCursor();
            foundCursor.insertText(replaceText);
            ui->textEdit->setTextCursor(foundCursor);
        } else {
            QMessageBox::information(this, "替换", "未找到");
        }
    }
}

void MainWindow::doReplaceAll(const QString &findText, const QString &replaceText,
                              bool caseSensitive)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    int count = 0;
    QTextCursor cursor(ui->textEdit->document());
    cursor.movePosition(QTextCursor::Start);
    ui->textEdit->setTextCursor(cursor);

    while (ui->textEdit->find(findText, flags)) {
        QTextCursor foundCursor = ui->textEdit->textCursor();
        foundCursor.insertText(replaceText);
        count++;
    }

    if (count == 0) {
        QMessageBox::information(this, "全部替换", "未找到");
    } else {
        QMessageBox::information(this, "全部替换", QString("已替换 %1 处").arg(count));
    }
}

void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();
    currentFileName.clear();
    isModified = false;
    this->setWindowTitle(tr("新建文本文件-编辑器"));
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files(*.txt);;All(*.*)"));
    if (filename.isEmpty()) {
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "提示", "打开文件失败");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    file.close();

    currentFileName = filename;
    isModified = false;
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFileName.isEmpty()) {
        on_actionSaveAs_triggered();
        return;
    }
    if (saveToFile(currentFileName)) {
        isModified = false;
        this->setWindowTitle(QFileInfo(currentFileName).absoluteFilePath());
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "另存为", currentFileName.isEmpty() ? "." : currentFileName, tr("Text files(*.txt);;All(*.*)"));
    if (filename.isEmpty()) {
        return;
    }
    if (saveToFile(filename)) {
        currentFileName = filename;
        isModified = false;
        this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    }
}

bool MainWindow::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "提示", "保存文件失败");
        return false;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
    return true;
}


void MainWindow::on_textEdit_textChanged()
{
    isModified = true;

    QString title;
    if (currentFileName.isEmpty()) {
        title = tr("新建文本文件-编辑器");
    } else {
        title = QFileInfo(currentFileName).absoluteFilePath();
    }
    if (isModified) {
        title += " *";
    }
    this->setWindowTitle(title);
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->font(), this, "选择字体");
    if (ok) {
        ui->textEdit->setFont(font);
    }
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
    if (color.isValid()) {
        QTextCursor cursor = ui->textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat fmt;
            fmt.setForeground(color);
            cursor.mergeCharFormat(fmt);
            ui->textEdit->setTextCursor(cursor);
        } else {
            QTextCursor all(ui->textEdit->document());
            all.select(QTextCursor::Document);
            QTextCharFormat fmt;
            fmt.setForeground(color);
            all.mergeCharFormat(fmt);
        }
    }
}


void MainWindow::on_actionFontBgColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择字体背景色");
    if (color.isValid()) {
        QTextCursor cursor = ui->textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat fmt;
            fmt.setBackground(color);
            cursor.mergeCharFormat(fmt);
            ui->textEdit->setTextCursor(cursor);
        } else {
            QTextCursor all(ui->textEdit->document());
            all.select(QTextCursor::Document);
            QTextCharFormat fmt;
            fmt.setBackground(color);
            all.mergeCharFormat(fmt);
        }
    }
}


void MainWindow::on_actionEditBgColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择编辑器背景色");
    if (color.isValid()) {
        QPalette palette = ui->textEdit->palette();
        palette.setColor(QPalette::Base, color);
        ui->textEdit->setPalette(palette);
    }
}


void MainWindow::on_actionAutoWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if ( mode == QPlainTextEdit::NoWrap ) {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWrap->setChecked(true);
    } else {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWrap->setChecked(false);
    }
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}


void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}


void MainWindow::updateStatusInfo()
{
    int length = ui->textEdit->document()->characterCount() - 1;
    if (length < 0) {
        length = 0;
    }
    int lines = ui->textEdit->document()->blockCount();
    statusLabel.setText("length: " + QString::number(length) + "  lines: " + QString::number(lines));
}


void MainWindow::updateCursorPosition()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.positionInBlock() + 1;
    statusCursorLabel.setText("Ln: " + QString::number(line) + "  Col: " + QString::number(col));
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!isModified) {
        event->accept();
        return;
    }

    QMessageBox::StandardButton ret = QMessageBox::warning(
        this, "退出", "文档尚未保存，是否保存？",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (ret == QMessageBox::Save) {
        if (currentFileName.isEmpty()) {
            on_actionSaveAs_triggered();
        } else {
            on_actionSave_triggered();
        }
        if (isModified) {
            event->ignore();
            return;
        }
        event->accept();
    } else if (ret == QMessageBox::Discard) {
        event->accept();
    } else {
        // Cancel
        event->ignore();
    }
}

