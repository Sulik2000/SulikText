#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>

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

private:
    Ui::MainWindow *ui;
    QFileSystemModel* _folderModel;
};
#endif // MAINWINDOW_H
