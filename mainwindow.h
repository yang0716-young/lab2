#pragma once

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_textEdit_textChanged();

    void doFind(const QString &text, bool caseSensitive, bool backward);
    void doFindAll(const QString &text, bool caseSensitive);
    void doReplace(const QString &findText, const QString &replaceText,
                   bool caseSensitive, bool backward);
    void doReplaceAll(const QString &findText, const QString &replaceText,
                      bool caseSensitive);

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionFont_triggered();

    void on_actionEditBgColor_triggered();

    void on_actionFontBgColor_triggered();

    void on_actionAutoWrap_triggered();

    void on_actionToolBar_triggered();

    void on_actionStatusBar_triggered();

private:
    bool saveToFile(const QString &filename);
    void updateStatusInfo();
    void updateCursorPosition();

protected:
    void closeEvent(QCloseEvent *event) override;

    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QString currentFileName;
    bool isModified = false;
};
