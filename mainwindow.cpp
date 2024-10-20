#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _folderModel = new QFileSystemModel();
    ui->treeView->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_file_triggered()
{
    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName("New file");
    int index = ui->tabWidget->addTab(text, "New file");
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), "/home", tr("Code files (*.cpp *.h *.c *.hpp)"), new QString(),
                                 QFileDialog::ReadOnly);
    QFile *file = new QFile(filePath, this);
    if(!file->open(QFile::ReadOnly | QFile::ExistingOnly)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot open file", QMessageBox::Ok, this);
        connect(msg, SIGNAL(QMessageBox::buttonClicked(QAbstractButton*)), msg, SLOT(QMessageBox::close()));
        return;
    }

    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName(filePath);
    text->setText(file->readAll());
    file->close();
    delete file;
    ui->tabWidget->addTab(text, file->fileName().split('/').last());
}


void MainWindow::on_actionSave_file_triggered()
{
    QString filePath = ui->tabWidget->currentWidget()->objectName();

    if(filePath != "New file"){
        QFile file(filePath, this);
        file.open(QFile::WriteOnly);
        file.write(qobject_cast<QTextEdit*>(ui->tabWidget->currentWidget())->toPlainText().toUtf8());
        file.close();
        return;
    }
    QString dir = "/home";
    if(_folderModel->rootPath() != "")
        QString dir = _folderModel->rootPath();

    filePath = QFileDialog::getSaveFileName(this, tr("Set file directory"), dir, tr("Code files (*.cpp *.h *.c *.hpp)"));
    QFile file(filePath, this);
    file.open(QFile::WriteOnly);
    file.write(qobject_cast<QTextEdit*>(ui->tabWidget->currentWidget())->toPlainText().toUtf8());
    file.close();
    ui->tabWidget->currentWidget()->setObjectName(filePath);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), file.fileName().split('/').last());
}

