#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

namespace Ui {
class OptionWindow;
}

class OptionWindow : public QDialog
{
    Q_OBJECT

signals:
    void ChangedStyle(QJsonObject applyingStyle);
public:
    explicit OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow();
    QJsonObject getCurrentStyle();
private slots:
    void on_pushButton_clicked();

private:
    QString currentStyle;
    void ApplyWindowStyle(QJsonObject applyingStyle);
    Ui::OptionWindow *ui;
};

#endif // OPTIONWINDOW_H
