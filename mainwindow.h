#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QLabel>

#include "ContextMenu/dircontextmenu.h"
#include "ContextMenu/filecontextmenu.h"
#include "ContextMenu/filedialog.h"
#include "optionwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_file_triggered();

    void on_actionOpen_file_triggered();

    void on_actionSave_file_triggered();

    void on_actionOpen_folder_triggered();

    void on_actionClose_folder_triggered();

    void on_actionClose_file_triggered();

    void on_TextEdit_change();

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void saveFile();
    void OpenFile(QString path);
    void OpenFile(QModelIndex index);
    void closeFileImmediately();
    void saveAndCloseFile();
    void DeleteFile(QString path);
    void RenameFile(QString path);
    void SetNewFileName(QString path, QString NewFileName);
    void AddNewFile(QString path);
    void AddNewFile(QString path, QString fileName);
    void RemoveFolder(QString path);
    void AddFolder(QString path);
    void AddNewFolder(QString path, QString dirName);
    void ContextMenuRootDir(QPoint pos);
    void on_actionOptions_triggered();
    void SetStyle(QJsonObject applyingStyle);
private:
    bool _isFileInDir(QString dirPath, QString filePath) const;
    Ui::MainWindow *ui;
    QFileSystemModel* _folderModel = nullptr;
    OptionWindow *optionWindow = nullptr;
};
#endif // MAINWINDOW_H
