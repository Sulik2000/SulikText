#ifndef FILECONTEXTMENU_H
#define FILECONTEXTMENU_H

#include <QMenu>
#include <QObject>
#include <QAction>
#include <QTabWidget>
#include <QFile>
#include <QTextEdit>

class FileContextMenu : public QMenu
{
    Q_OBJECT
public:
    FileContextMenu(QString path, QWidget* parent = nullptr);
private:
    QString pathToFile;
signals:
    void OpenFile(QString path);
    void DeleteFile(QString path);
    void RenameFile(QString path);
private slots:
    void OpenFile();
    void DeleteFile();
    void RenameFile();
};

#endif // FILECONTEXTMENU_H
