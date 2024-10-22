#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_file_triggered()
{
    // Creating new tab in the tab widget
    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName("New file");
    connect(text, SIGNAL(textChanged()), this, SLOT(on_TextEdit_change()));
    int index = ui->tabWidget->addTab(text, "New file");
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::on_actionOpen_file_triggered()
{
    // Get path to file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), "/home", tr("Code files (*.cpp *.h *.c *.hpp)"), new QString(),
                                 QFileDialog::ReadOnly);
    QFile *file = new QFile(filePath, this);
    // If causes error we show messsage box with an error
    if(!file->open(QFile::ReadOnly | QFile::ExistingOnly)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot open file", QMessageBox::Ok, this);
        connect(msg, SIGNAL(QMessageBox::buttonClicked(QAbstractButton*)), msg, SLOT(QMessageBox::close()));
        return;
    }
    // Creating new tab in the tab widget
    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName(filePath);
    QString plainText = file->readAll();
    text->setText(plainText);
    file->close();

    connect(text, SIGNAL(textChanged()), this, SLOT(on_TextEdit_change()));
    ui->tabWidget->addTab(text,
                          file->fileName().split('/').last());
    delete file;
}


void MainWindow::on_actionSave_file_triggered()
{
    saveFile();
}


void MainWindow::on_actionOpen_folder_triggered()
{
    QString dirLoc = QFileDialog::getExistingDirectory(this, "Select folder to open", "/home");
    if(dirLoc.isNull())
        return;
    // Initialize file system model
    _folderModel = new QFileSystemModel();
    // Setup treeview
    ui->treeView->setModel(_folderModel);
    ui->treeView->setRootIndex(_folderModel->setRootPath(dirLoc));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    ui->treeView->setVisible(true);
}


void MainWindow::on_actionClose_folder_triggered()
{
    ui->treeView->setModel(nullptr);
    ui->treeView->setVisible(false);
    // Free memory
    delete _folderModel;
}


void MainWindow::on_actionClose_file_triggered()
{
    QString name = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    // If file is unsaved
    if(name.endsWith('*')){
        QDialogButtonBox *msg = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::No);
        QLabel *label = new QLabel(msg);
        msg->setWindowTitle("File is unsaved");
        msg->setGeometry(msg->geometry().x(), msg->geometry().y(), 430, 60);
        label->setText("File " + name.removeLast() + " is unsaved. Do you want to save it?");
        label->adjustSize();
        label->show();
        connect(msg, SIGNAL(accepted()), this, SLOT(saveAndCloseFile()));
        connect(msg, SIGNAL(rejected()), this, SLOT(closeFileImmediately()));
        connect(msg, SIGNAL(accepted()), msg, SLOT(close()));
        connect(msg, SIGNAL(rejected()), msg, SLOT(close()));
        msg->show();
        return;
    }

    closeFileImmediately();
}

void MainWindow::on_TextEdit_change()
{

    int index = ui->tabWidget->currentIndex();
    if(ui->tabWidget->tabText(index).endsWith('*'))
        return;

    ui->tabWidget->setTabText(index, ui->tabWidget->tabText(index) + '*');
}

void MainWindow::closeFileImmediately()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::saveAndCloseFile()
{
    saveFile();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::saveFile()
{
    QString filePath = ui->tabWidget->currentWidget()->objectName();
    // If file path is defined
    if(filePath != "New file"){
        QFile file(filePath, this);
        file.open(QFile::WriteOnly);
        file.write(qobject_cast<QTextEdit*>(ui->tabWidget->currentWidget())->toPlainText().toUtf8());
        if(ui->tabWidget->tabText(ui->tabWidget->currentIndex()).endsWith('*'))
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),
                                      ui->tabWidget->tabText(ui->tabWidget->currentIndex()).removeLast());
        file.close();
        return;
    }

    QString dir;
    // If folder are opened
    if(!_folderModel)
        dir = "/home";
    else
        dir = _folderModel->filePath(ui->treeView->rootIndex());
    // Get path for new file
    filePath = QFileDialog::getSaveFileName(this, tr("Set file directory"), dir, tr("Code files (*.cpp *.h *.c *.hpp)"));
    QFile file(filePath, this);
    file.open(QFile::WriteOnly);
    file.write(qobject_cast<QTextEdit*>(ui->tabWidget->currentWidget())->toPlainText().toUtf8());
    file.close();
    ui->tabWidget->currentWidget()->setObjectName(filePath);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), file.fileName().split('/').last());
}

