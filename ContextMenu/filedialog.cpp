#include "filedialog.h"
#include "ui_filedialog.h"

RenameFileDialog::RenameFileDialog(QString label, QString title, QString path, QString fileName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RenameFileDialog)
{
    ui->setupUi(this);
    pathToFile = path;
    ui->lineEdit->setText(fileName);
    ui->label->setText(label);
    setWindowTitle(title);
}

RenameFileDialog::~RenameFileDialog()
{
    delete ui;
}

void RenameFileDialog::on_buttonBox_accepted()
{
    emit SetFileName(pathToFile, ui->lineEdit->text());
}

