#include "optionwindow.h"
#include "ui_optionwindow.h"

OptionWindow::OptionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionWindow)
{
    ui->setupUi(this);
    // Parsing styles from file
    QFile file(":/style/styles.json");
    file.open(QFile::ReadOnly);
    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    QJsonArray styles = object.value("styles").toArray();
    file.close();

    for(int i = 0; i < styles.size(); i++)
        if(styles[i].isObject())
            ui->comboBox->addItem(styles[i].toObject().value("Name").toString());

    ui->comboBox->setCurrentText(object.value("SelectedStyle").toString());
    currentStyle = object.value("SelectedStyle").toString();
    for(int i = 0; i < styles.size(); i++)
        if(styles[i].toObject().value("Name").toString() == ui->comboBox->currentText())
            ApplyWindowStyle(styles[i].toObject());
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

QJsonObject OptionWindow::getCurrentStyle()
{
    // Parsing styles from file
    QFile file(":/style/styles.json");
    file.open(QFile::ReadOnly);
    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    QJsonArray styles = object.value("styles").toArray();
    file.close();
    for(int i = 0; i < styles.size(); i++)
        if(styles[i].toObject().value("Name").toString() == ui->comboBox->currentText())
            return styles[i].toObject();
    return QJsonObject();
}

void OptionWindow::ApplyWindowStyle(QJsonObject applyingStyle)
{
    setStyleSheet("background-color: " + applyingStyle.value("MainWindow").toObject().value("background-color").toString() +
                  ";\ncolor: " + applyingStyle.value("TextEdit").toObject().value("text-color").toString());

    ui->pushButton->setStyleSheet("background-color: " +
                                  applyingStyle.value("MessageBox").toObject().value("button-color").toString() + ";\n" +
                                  "color: " + applyingStyle.value("MessageBox").toObject().value("button-text-color").toString());

    ui->pushButton_2->setStyleSheet("background-color: " +
                                    applyingStyle.value("MessageBox").toObject().value("button-color").toString() + ";\n" +
                                    "color: " + applyingStyle.value("MessageBox").toObject().value("button-text-color").toString());

    // Write style to the file
    QFile file(":/style/styles.json");
    file.open(QFile::ReadOnly);
    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    object.value("SelectedStyle") = applyingStyle.value("Name").toString();
    QJsonDocument doc;
    doc.setObject(object);
    qDebug() << applyingStyle.value("Name").toString();
    file.write(doc.toJson());
    file.close();
}

void OptionWindow::on_pushButton_clicked()
{
    if(currentStyle == ui->comboBox->currentText())
        return;

    currentStyle = ui->comboBox->currentText();
    QFile file(":/style/styles.json");
    file.open(QFile::ReadOnly);
    QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
    QJsonArray styles = object.value("styles").toArray();
    file.close();
    for(int i = 0; i < styles.size(); i++)
        if(styles[i].toObject().value("Name").toString() == currentStyle){

            ApplyWindowStyle(styles[i].toObject());
            emit ChangedStyle(styles[i].toObject());
        }

}

