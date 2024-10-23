#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameFileDialog;
}

class RenameFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameFileDialog(QString label, QString title, QString path, QString fileName, QWidget *parent = nullptr);
    ~RenameFileDialog();
signals:
    void SetFileName(QString path, QString newFileName);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::RenameFileDialog *ui;
    QString pathToFile;
};

#endif // FILEDIALOG_H
