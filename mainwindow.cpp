#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->hide();
    connect(ui->treeView, SIGNAL(OpenFile(QModelIndex)), this, SLOT(OpenFile(QModelIndex)));
    connect(ui->treeView, SIGNAL(ContextMenuRootDir(QPoint)), this, SLOT(ContextMenuRootDir(QPoint)));
    optionWindow = new OptionWindow(this);
    SetStyle(optionWindow->getCurrentStyle());
    connect(optionWindow, SIGNAL(ChangedStyle(QJsonObject)), this, SLOT(SetStyle(QJsonObject)));
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
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), "/home", tr(""), new QString(),
                                 QFileDialog::ReadOnly);
    OpenFile(filePath);
}


void MainWindow::on_actionSave_file_triggered()
{
    saveFile();
}


void MainWindow::on_actionOpen_folder_triggered()
{
    if(!_folderModel)
        delete _folderModel;
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

void MainWindow::DeleteFile(QString path)
{
    if(!QFile::moveToTrash(path)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot delete this file",
                                           QMessageBox::Ok, this);
        connect(msg, SIGNAL(accepted()), msg, SLOT(close()));
        return;
    }

    for(int i = 0; i < ui->tabWidget->count(); i++)
        if(ui->tabWidget->widget(i)->objectName() == path)
            ui->tabWidget->removeTab(i);
}

void MainWindow::RenameFile(QString path)
{
    RenameFileDialog *msg = new RenameFileDialog("Enter new name of the file", "Rename file", path, path.split('/').last(), this);
    connect(msg, SIGNAL(SetFileName(QString,QString)), this, SLOT(SetNewFileName(QString,QString)));
    msg->exec();
}

void MainWindow::SetNewFileName(QString path, QString NewFileName)
{
    int index = -1;
    for(int i = 0; i < ui->tabWidget->count(); i++)
        if(ui->tabWidget->widget(i)->objectName() == path){
            index = i;
            break;
        }

    QFile *file = new QFile(path);
    QStringList l = path.split('/');
    l.removeLast();
    l.append(NewFileName);
    QString buf_Name = NewFileName;
    NewFileName = "";

    for(int i = 1; i < l.size(); i++)
        NewFileName += '/' + l[i];

    file->open(QFile::ReadWrite);
    if(!file->rename(NewFileName)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot rename this file",
                                           QMessageBox::Ok, this);
        connect(msg, SIGNAL(accepted()), msg, SLOT(close()));
        msg->exec();
        return;
    }

    if(index != -1){
        ui->tabWidget->widget(index)->setObjectName(file->fileName());
        if(ui->tabWidget->tabText(index).endsWith('*'))
            buf_Name += '*';
        ui->tabWidget->setTabText(index, buf_Name);
    }
    file->close();
    delete file;
}

void MainWindow::AddNewFile(QString path, QString fileName)
{
    QFile *file = new QFile(path + '/' + fileName);
    file->open(QFile::WriteOnly | QFile::NewOnly);

    file->close();
    delete file;
    OpenFile(path + '/' + fileName);
}

void MainWindow::RemoveFolder(QString path)
{
    QDir *dir = new QDir(path);
    if(!dir->removeRecursively()){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot delete this folder",
                                           QMessageBox::Ok, this);
        connect(msg, SIGNAL(accepted()), msg, SLOT(close()));
        msg->exec();
        return;
    }
    for(int i = 0; i < ui->tabWidget->count(); i++)
        if(_isFileInDir(path, ui->tabWidget->widget(i)->objectName())){
            ui->tabWidget->setCurrentIndex(i);
            on_actionClose_file_triggered();
            i--;
        }
}

void MainWindow::AddFolder(QString path)
{
    RenameFileDialog *msg = new RenameFileDialog("Enter name of the new folder", "New folder", path, "", this);
    connect(msg, SIGNAL(SetFileName(QString,QString)), this, SLOT(AddNewFolder(QString,QString)));
    msg->exec();
}

void MainWindow::AddNewFolder(QString path, QString dirName)
{
    QDir dir(path);
    if(!dir.mkdir(dirName)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot create this folder",
                                           QMessageBox::Ok, this);
        connect(msg, SIGNAL(accepted()), msg, SLOT(close()));
        msg->exec();
    }
}

void MainWindow::ContextMenuRootDir(QPoint pos)
{
    QString dirPath = _folderModel->filePath(ui->treeView->rootIndex());
    DirContextMenu* menu = new DirContextMenu(dirPath, this);
    connect(menu, SIGNAL(AddNewFile(QString)), this, SLOT(AddNewFile(QString)));
    connect(menu, SIGNAL(RemoveFolder(QString)), this, SLOT(RemoveFolder(QString)));
    connect(menu, SIGNAL(AddNewFolder(QString)), this, SLOT(AddFolder(QString)));
    menu->exec(QCursor::pos());
}

bool MainWindow::_isFileInDir(QString dirPath, QString filePath) const
{
    QStringList d = dirPath.split('/');
    QStringList f = filePath.split('/');
    if(f.size() <= d.size())
        return false;

    for(int i = 0; i < d.size(); i++){
        if(d[i] != f[i])
            return false;
    }
    return true;
}

void MainWindow::AddNewFile(QString path)
{
    RenameFileDialog *msg = new RenameFileDialog("Enter name of the new file", "New file", path, "", this);
    connect(msg, SIGNAL(SetFileName(QString,QString)), this, SLOT(AddNewFile(QString,QString)));
    msg->exec();
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
    filePath = QFileDialog::getSaveFileName(this, tr("Set file directory"), dir);
    QFile file(filePath, this);
    file.open(QFile::WriteOnly);
    file.write(qobject_cast<QTextEdit*>(ui->tabWidget->currentWidget())->toPlainText().toUtf8());
    file.close();
    ui->tabWidget->currentWidget()->setObjectName(filePath);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), file.fileName().split('/').last());
}

void MainWindow::OpenFile(QString path)
{
    // Check if file already is opened
    for(int i = 0; i < ui->tabWidget->count(); i++)
        if(ui->tabWidget->widget(i)->objectName() == path){
            ui->tabWidget->setCurrentIndex(i);
            return;
        }

    QFile *file = new QFile(path, this);
    // If causes error we show messsage box with an error
    if(!file->open(QFile::ReadOnly | QFile::ExistingOnly)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot open file", QMessageBox::Ok, this);
        connect(msg, SIGNAL(QMessageBox::buttonClicked(QAbstractButton*)), msg, SLOT(QMessageBox::close()));
        msg->exec();
        return;
    }
    // Creating new tab in the tab widget
    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName(path);
    QString plainText = file->readAll();
    text->setText(plainText);
    file->close();

    connect(text, SIGNAL(textChanged()), this, SLOT(on_TextEdit_change()));
    int index = ui->tabWidget->addTab(text,
                          file->fileName().split('/').last());
    ui->tabWidget->setCurrentIndex(index);
    delete file;
}

void MainWindow::OpenFile(QModelIndex index)
{
    QFileInfo fileInfo = _folderModel->fileInfo(index);
    if(!fileInfo.isFile())
        return;

    QString path = fileInfo.absoluteFilePath();
    // Check if file already is opened
    for(int i = 0; i < ui->tabWidget->count(); i++)
        if(ui->tabWidget->widget(i)->objectName() == path){
            ui->tabWidget->setCurrentIndex(i);
            return;
        }

    QFile *file = new QFile(path, this);
    // If causes an error we show messsage box with an error
    if(!file->open(QFile::ReadOnly | QFile::ExistingOnly)){
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Error", "Cannot open file", QMessageBox::Ok, this);
        connect(msg, SIGNAL(QMessageBox::buttonClicked(QAbstractButton*)), msg, SLOT(QMessageBox::close()));
        msg->exec();
        return;
    }
    // Creating new tab in the tab widget
    QTextEdit *text = new QTextEdit(ui->tabWidget);
    text->setObjectName(path);
    QString plainText = file->readAll();
    text->setText(plainText);
    file->close();

    connect(text, SIGNAL(textChanged()), this, SLOT(on_TextEdit_change()));
    int tabIndex = ui->tabWidget->addTab(text,
                                      file->fileName().split('/').last());
    ui->tabWidget->setCurrentIndex(tabIndex);
    delete file;
}


void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    QFileInfo file = _folderModel->fileInfo(index);
    // If object under cursor is a directory
    if(file.isDir()){
        DirContextMenu* menu = new DirContextMenu(file.absoluteFilePath(), this);
        connect(menu, SIGNAL(AddNewFile(QString)), this, SLOT(AddNewFile(QString)));
        connect(menu, SIGNAL(RemoveFolder(QString)), this, SLOT(RemoveFolder(QString)));
        connect(menu, SIGNAL(AddNewFolder(QString)), this, SLOT(AddFolder(QString)));
        menu->exec(QCursor::pos());
        return;
    }
    // If object under cursor is a file
    if(file.isFile()){
        FileContextMenu *menu = new FileContextMenu(file.absoluteFilePath(), this);
        connect(menu, SIGNAL(OpenFile(QString)), this, SLOT(OpenFile(QString)));
        connect(menu, SIGNAL(DeleteFile(QString)), this, SLOT(DeleteFile(QString)));
        connect(menu, SIGNAL(RenameFile(QString)), this, SLOT(RenameFile(QString)));
        menu->exec(QCursor::pos());
    }
}


void MainWindow::on_actionOptions_triggered()
{
    if(optionWindow){
        optionWindow->show();
        optionWindow->setFocus();
        return;
    }
    optionWindow = new OptionWindow(this);
    connect(optionWindow, SIGNAL(ChangedStyle(QJsonObject)), this, SLOT(SetStyle(QJsonObject)));
    optionWindow->exec();
}

void MainWindow::SetStyle(QJsonObject applyingStyle)
{
    setStyleSheet("background-color: " + applyingStyle.value("MainWindow").toObject().value("background-color").toString());
    ui->treeView->setStyleSheet("background-color: " +
                                applyingStyle.value("TextEdit").toObject().value("background-color").toString() +
                                ";\ncolor: " + applyingStyle.value("TextEdit").toObject().value("text-color").toString() +
                                ";\nborder: 1px solid " +
                                    applyingStyle.value("TextEdit").toObject().value("border-color").toString() + ';');

    ui->tabWidget->setStyleSheet("background-color: " +
                                 applyingStyle.value("TextEdit").toObject().value("background-color").toString() +
                                 ";\ncolor: " + applyingStyle.value("TextEdit").toObject().value("text-color").toString() +
                                 ";\nborder: 1px solid " +
                                 applyingStyle.value("TextEdit").toObject().value("border-color").toString() + ';');
}

